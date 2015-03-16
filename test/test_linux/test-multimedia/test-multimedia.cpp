#include <QCoreApplication>
#include <libmultimedia/common_sound_device.hpp>
#include <QAudioFormat>

#include <iostream>
#include <cassert>
extern "C"
{
#include <libavcodec/avcodec.h>
//#include <libswscale/swscale.h>
#include <libavformat/avformat.h>
}


int main(int argc, char** argv)
{
	QCoreApplication app(argc, argv);

	if(argc != 2)
	{
		std::cout << "Usage: "<< argv[0] <<" file_name" << std::endl;
		return -1;
	}

	instek::SoundOutputDevice snd_dev;

	avcodec_register_all();
	av_register_all();
	AVFormatContext* formatContext = avformat_alloc_context();

	// Open audio file
	if(avformat_open_input(&formatContext, argv[1], NULL, NULL)!=0)
	{
		std::cout << "Couldn't open file" << std::endl;
		return -1; // Couldn't open file
	}

	// Retrieve stream information
	if(avformat_find_stream_info(formatContext, NULL)<0)
	{
		std::cout << "Couldn't find stream information" << std::endl;
		return -1; // Couldn't find stream information
	}

	// Dump information about file onto standard error
	av_dump_format(formatContext, 0, argv[1], 0);

	// Find the first audio stream
	int audioStream = -1;
	AVCodecContext *codecContext = NULL;

	for( unsigned int i=0; i < formatContext->nb_streams; i++ )
	{
		if( formatContext->streams[i]->codec->codec_type == AVMEDIA_TYPE_AUDIO )
		{
			audioStream = i;
			break;
		}
	}
	if( audioStream == -1 )
	{
		std::cout << "Didn't find a audio stream" << std::endl;
		return -1;
	}

	codecContext = formatContext->streams[audioStream]->codec;

	// Prepare sound device
	QAudioFormat::SampleType temp_type = QAudioFormat::Unknown;
	int temp_sample_size = 0;

	switch(codecContext->sample_fmt)
	{
		case AV_SAMPLE_FMT_U8:
		case AV_SAMPLE_FMT_U8P:
			temp_type = QAudioFormat::UnSignedInt;
			temp_sample_size = 8;
			break;
		case AV_SAMPLE_FMT_S16:
		case AV_SAMPLE_FMT_S16P:
			temp_type = QAudioFormat::SignedInt;
			temp_sample_size = 16;
			break;
		case AV_SAMPLE_FMT_S32:
		case AV_SAMPLE_FMT_S32P:
			temp_type = QAudioFormat::SignedInt;
			temp_sample_size = 32;
			break;
		case AV_SAMPLE_FMT_FLT:
		case AV_SAMPLE_FMT_FLTP:
			temp_type = QAudioFormat::Float;
			temp_sample_size = sizeof(float)*8;
			break;
		case AV_SAMPLE_FMT_DBL:
		case AV_SAMPLE_FMT_DBLP:
			temp_type = QAudioFormat::Float;
			temp_sample_size = sizeof(double)*8;
			break;
		default:
			temp_type = QAudioFormat::Unknown;
			temp_sample_size = 0;
			break;
	}

	snd_dev.CreateAudioDevice(codecContext->sample_rate, temp_sample_size, codecContext->channels, temp_type, QAudioFormat::LittleEndian, "audio/pcm", 80000, true);
	snd_dev.Start();


	// Find the decoder for the audio stream
	AVCodec *codec = NULL;
	codec = avcodec_find_decoder(codecContext->codec_id);
	if(codec == NULL)
	{
		std::cout << "Unsupported codec!" << std::endl;
		return -1; // Codec not found
	}

	// Open codec
	if(avcodec_open2(codecContext, codec, NULL) < 0)
	{
		std::cout << "Could not open codec" << std::endl;
		return -1; // Could not open codec
	}

	AVPacket packet;
	av_init_packet(&packet);
	AVFrame *frame = avcodec_alloc_frame();
	short* resample_out_buf = (short*) av_malloc(AVCODEC_MAX_AUDIO_FRAME_SIZE + FF_INPUT_BUFFER_PADDING_SIZE);
	//ReSampleContext* re_sample_context = NULL;

	while( av_read_frame(formatContext, &packet) == 0 )
	{
		//qint64 written=0;
		if( packet.stream_index == audioStream )
		{
			std::cout << "size = " << packet.size << std::endl;
			while(packet.size > 0)
			{
				int len, data_size = AVCODEC_MAX_AUDIO_FRAME_SIZE;
				//decode
				int got_frame = 0;
				avcodec_get_frame_defaults(frame);
				len = avcodec_decode_audio4(codecContext, frame, &got_frame, &packet);

				if( len < 0 )
				{
					std::cout << "Error while decoding" << std::endl;
					return -1;
				}

				if (got_frame)
				{
					data_size = av_samples_get_buffer_size(NULL, codecContext->channels, frame->nb_samples, codecContext->sample_fmt, 1);

#if 0
					if(codecContext->sample_fmt != AV_SAMPLE_FMT_S16 || codecContext->channels != 1 || codecContext->sample_rate != 8000)
					{
						if(!re_sample_context)
						{
							re_sample_context = av_audio_resample_init(1, codecContext->channels, 8000, codecContext->sample_rate, AV_SAMPLE_FMT_S16, codecContext->sample_fmt, 16, 10, 0, 1.5);
							assert(re_sample_context);
						}
printf("======== %u\n", frame->linesize[0]);
						//data_size = audio_resample(re_sample_context, resample_out_buf, (short*)frame->data[0], data_size/2);
						//data_size = audio_resample(re_sample_context, resample_out_buf, (short*)frame->data[0], data_size / (codecContext->channels * 2));
						data_size = audio_resample(re_sample_context, resample_out_buf, (short*)frame->data[0], data_size / (codecContext->channels * 2));
						if(data_size > 0)
							snd_dev.PushAudioToOuputDevice((unsigned char*)resample_out_buf, data_size);
					}
					else
					{
						if( data_size > 0 )
							snd_dev.PushAudioToOuputDevice((unsigned char*)frame->data[0], data_size);
					}
#else
					if( data_size > 0 )
					{
						snd_dev.PushAudioToOuputDevice((const char*)frame->data[0], data_size);
					}
#endif

				}
				packet.size -= len;
				packet.data += len;
				//usleep(142857);
				usleep(143000);
			}

		}

	}

	//audio_resample_close(re_sample_context);
	avformat_free_context(formatContext);
	av_free(resample_out_buf);
	av_free(frame);

	sleep(5);

	snd_dev.Stop();

	return app.exec();
}

#include <libav/decoder.hpp>
#include <libstreaming/streaming_client.hpp>
#include <cstdlib>

class StreamingDecoder
{
	private:
	instek::StreamingClient* streaming_client_;
	instek::Decoder* decoder_;
	bool decoder_ok_;

	bool live_streaming_callback(int pkt_type, unsigned char* data, size_t size)
	{
		if (pkt_type == 0)
		{
			printf("no data....................\n");
			return false;
		}
		if (pkt_type == 7) //csh packet
		{
			printf("csh-------------------\n");
			decoder_->Stop(false);

			//packet time stamp;
			uint32_t sec = *((uint32_t*)(data - 16));
			uint32_t msec = *((uint32_t*)(data - 12));
			printf("sec: %u usec: %u type: %d size: %lu\n",
					sec, msec, pkt_type, size);

			decoder_ok_ = decoder_->Start(instek::Decoder::PacketInfo(data, size, sec, msec),
					boost::bind(&StreamingDecoder::live_decode_callback,
						this, _1, _2, _3), false);
			//decoder_->ThrowBFrames(1);

		}
		else if (decoder_ok_)
		{
			/*
			   unsigned int sec = *((unsigned int*)(data + size));
			   if (last_sec_ != sec)
			   {
			   last_sec_ = sec;
			//int msec = *((int*)(data + size + 4));
			if (!camera_name_.empty())
			sprintf(osdbuf_,"%s   ", camera_name_.c_str());

			char* timebuf = osdbuf_ + strlen(osdbuf_);
			ctime_r((time_t* )&sec, timebuf);
			//printf(" %s %d\n", timebuf, strlen(timebuf));
			}
			*/
			//packet timestamp
			int sec = *((int*)(data + size));
			int msec = *((int*)(data + size + 4));
			printf("sec: %d usec: %d type: %d size: %lu\n", sec, msec, pkt_type, size);

			//live
			if (pkt_type == 2) //I frame
			{
				printf("I------------------------------------\n");
				decoder_->InputVideoData(data, size);
			}
			else if (pkt_type < 6) //ignore OSD packet -> value: 8
			{
				decoder_->InputVideoData(data, size);
			}
			/*else if (pkt_type == 8)
			  {
			  short length;
			  memcpy(&length, data + 2, 2);
			  data[size-1] = '\0';
			  camera_name_.assign(data + 4);
			  printf("----> size: %d length %d %s\n", size, length, data + 4);
			  }
			  */
		}

		return true;
	}

	void live_decode_callback(int, unsigned char** data, size_t size)
	{
		printf("size: %lu %d %d\n", size, decoder_->GetWidth(), decoder_->GetHeight());
	}

	public:
	StreamingDecoder(const instek::StreamingClient::StreamingInfo& s_info):decoder_ok_(false)
	{
		decoder_ = new instek::Decoder;
		streaming_client_ = new instek::StreamingClient(s_info);
		streaming_client_->SetCallbackFunc(boost::bind(&StreamingDecoder::live_streaming_callback, this, _1, _2, _3));
		streaming_client_->SetReconnectTime(5);
		streaming_client_->Start();
	}

	~StreamingDecoder()
	{
		streaming_client_->Stop();
		decoder_->Stop(true);

		delete streaming_client_;
		delete decoder_;
	}

	void Seek()
	{
		streaming_client_->Seek();
	}
	void Stop()
	{
		streaming_client_->Stop();
	}
	void ToContinue(unsigned int sec = 0)
	{
		streaming_client_->ToContinue(sec);
	}
};

int main()
{
	instek::StreamingClient::StreamingInfo s_info;
	s_info.username = "admin";
	s_info.password = "id";
	s_info.remote_ip = "172.16.2.166";//"vincent-ubuntu.local";//"172.16.2.130";
	s_info.camera_id = 1;


	s_info.streaming_mode = 0; //live: 0, playback: 1, local: 2

	// Live
	s_info.live.remote_port = "3514";
	s_info.live.streaming_profile = 6; //best resolution
	s_info.live.width = 704;
	s_info.live.height = 480;

	// Local file
	s_info.local.data_rate = 1; // 0, 1, 2, 4, 8
	s_info.local.direction = 0; // forward: 0 , backward = 1;
	s_info.local.sec = 0;//::time(NULL); // seek to the begin
	s_info.local.data_interval = 0; // pass 0 to play, Pass data_rate =0,data_interval=integer to get I frame only
	s_info.local.ivf_path = "./Cam.ivf"; // The file path of iavf
	s_info.local.output_path = "/tmp/Cam.idb"; // The output file path of idb

	StreamingDecoder test(s_info);

	printf("type 'q' to quit.\n");
	printf("=======================\n");
	printf("1: Seek.\n");
	printf("2: Stop.\n");
	printf("3: To continue.\n");
	printf("=======================\n");
	char quit_char;
	do
	{
		scanf("%c",&quit_char);

		// Test seek function
		switch(atoi(&quit_char))
		{
			case 1:
				test.Seek();
				break;
			case 2:
				test.Stop();
				break;
			case 3:
				test.ToContinue();
				break;
		}

	} while (quit_char != 'q');
}

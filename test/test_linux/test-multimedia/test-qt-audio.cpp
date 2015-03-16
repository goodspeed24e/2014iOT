#include <QCoreApplication>
#include <QFile>
#include <QAudioFormat>
#include <QAudioOutput>
#include <iostream>

int main(int argc, char** argv)
{
	// Input #0, wav, from 'byebye.wav':
	//   Duration: 00:00:03.79, bitrate: 176 kb/s
	//       Stream #0:0: Audio: pcm_u8 ([1][0][0][0] / 0x0001), 22050 Hz, 1 channels, u8, 176 kb/s
	//
	QCoreApplication app(argc, argv);

	if(argc != 2)
	{
		std::cout << "Usage: "<< argv[0] <<" file_name" << std::endl;
		return -1;
	}

	QFile inputFile;
	inputFile.setFileName(argv[1]);
	inputFile.open(QIODevice::ReadOnly);

	QAudioFormat format;
	format.setFrequency(22050);
	format.setChannels(1);
	format.setSampleSize(8);
	format.setCodec("audio/pcm");
	format.setByteOrder(QAudioFormat::LittleEndian);
	format.setSampleType(QAudioFormat::UnSignedInt);
	QAudioOutput *audio = new QAudioOutput(format, 0);
	audio->start(&inputFile);

	return app.exec();
}

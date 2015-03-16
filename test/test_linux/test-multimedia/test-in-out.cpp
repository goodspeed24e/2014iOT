#include <QCoreApplication>
#include <libmultimedia/common_sound_device.hpp>
#include <QAudioFormat>

#include <iostream>

instek::SoundOutputDevice out_dev;
instek::SoundInputDevice in_dev;

void DataReady(const char* data, size_t size)
{
	out_dev.PushAudioToOuputDevice(data, size);
	std::cout << "callback ========" << std::endl;
}

int main(int argc, char** argv)
{
	QCoreApplication app(argc, argv);
	out_dev.CreateAudioDevice(8000, 16, 1, QAudioFormat::SignedInt, QAudioFormat::LittleEndian, "audio/pcm", 80000, true);
	out_dev.Start();

	in_dev.CreateAudioDevice(8000, 16, 1, QAudioFormat::SignedInt, QAudioFormat::LittleEndian, "audio/pcm");
	in_dev.RegisterDataFromInputDeviceCallback(&DataReady);
	in_dev.Start();

	int ret = app.exec();

	in_dev.Stop();
	out_dev.Stop();
	return ret;
}

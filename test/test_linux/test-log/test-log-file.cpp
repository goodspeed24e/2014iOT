#include <libcommon/logger.hpp>

#include <cstdio>

#define NUM_OF_TIMES 21

int main(int /*argc*/, char**argv)
{
#ifdef WIN32
	if(!instek::Logger::OpenLog(argv[0], "\\log_dir", "prefix"))
		printf("Can not open log file.\n");
#else // WIN32
	if(!instek::Logger::OpenLog(argv[0], "./log_dir", "prefix"))
		printf("Can not open log file.\n");
#endif // WIN32

	for(int i = 0; i < NUM_OF_TIMES; ++i)
	{
		instek::Logger::Log("%s, %d, %s\n", "testString", i, "===============================");
	}

	return 0;
}


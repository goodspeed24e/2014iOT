#include <libstreaming/streaming_client.hpp>
#include <cstdio>
#include <cstdlib>
#include <string>
#include <climits>
#include <cerrno>
#include <boost/thread.hpp>
#include "streaming-downloader.hpp"

static boost::mutex wait_complete_mutex_;
static boost::condition_variable complete_condition_;
static bool is_completed_ = false;
// The value of result_ should be between 0 and 255 because the return code of subprocess in the python only support that.
static int result_ = 0;


static void DownloadStatusCallbackFunc(int status)
{
	FILE *temp_out = ( (status == 0) ? stdout : stderr );
	fprintf(temp_out, "(In main) callback---------->%d\n", status);

	result_ = -status;
	boost::mutex::scoped_lock lock(wait_complete_mutex_);
	is_completed_ = true;
	complete_condition_.notify_all();
}

static void DownloadProgressCallbackFunc(unsigned int time)
{
	printf("=====>>>>>> %u\n", time);
}

static bool StringToUnsignedInteger(const char* str, unsigned long& value)
{
	char *endptr = NULL;
	unsigned long val = 0;

	errno = 0;
	val = strtoul(str, &endptr, 10);

	/* Check for various possible errors */

	if ((errno == ERANGE && val == ULONG_MAX) || (errno != 0 && val == 0))
	{
		perror("strtoul");
		return false;
	}

	if (endptr == str)
	{
		fprintf(stderr, "No digits were found\n");
		return false;
	}

	value = val;
	return true;
}

int main(int argc, char** argv)
{
	// DownloadIavf.exe IP Port Camera_ID Start_Time End_Time IAVF_Output_Path IDB_Output_Path Output_Filename Split_File_Size Time_Zone Username Password
	//
	if(argc != 13)
	{
		fprintf(stderr, "Usage: %s IP Port Camera_ID Start_Time End_Time IAVF_Output_Path IDB_Output_Path Output_Filename Split_File_Size Time_Zone Username Password\n", argv[0]);
		fprintf(stderr, "ex: ./DownloadIavf 172.16.3.38 60006 1 1342057957 1342060237 ./output_iavf ./output_idb test 104857600 8 admin id\n");
		return (result_ = 255);
	}

	enum { IP_Index = 1, Port_Index, Camera_ID_Index, Start_Time_Index, End_Time_Index, IAVF_Output_Path, IDB_Output_Path, Output_Filename_Index, Split_File_Size_Index, Time_Zone_Index, Username_Index, Password_Index };

	unsigned long start_time = 0;
	unsigned long end_time = 0;
	std::string iavf_output_path = argv[IAVF_Output_Path];
	std::string idb_output_path = argv[IDB_Output_Path];
	std::string output_filename = argv[Output_Filename_Index];
	unsigned long split_file_size = 0;
	int time_zone = atoi(argv[Time_Zone_Index]);
	unsigned long camera_id = 0;

	if( (iavf_output_path.length() == 0) || (idb_output_path.length() == 0) || (output_filename.length() == 0) )
	{
		fprintf(stderr, "Output path or filename is invalid.\n");
		return (result_ = 255);
	}

	if(!StringToUnsignedInteger(argv[Camera_ID_Index], camera_id))
		return (result_ = 255);

	if(!StringToUnsignedInteger(argv[Start_Time_Index], start_time))
		return (result_ = 255);

	if(!StringToUnsignedInteger(argv[End_Time_Index], end_time))
		return (result_ = 255);

	if(!StringToUnsignedInteger(argv[Split_File_Size_Index], split_file_size))
		return (result_ = 255);

	if(start_time > end_time)
	{
		fprintf(stderr, "start_time must be less than end_time\n");
		return (result_ = 255);
	}

	if(split_file_size < 10485760)
	{
		fprintf(stderr, "split_file_size must be great than 10485760 bytes (10MB).\n");
		return (result_ = 255);
	}
	if(time_zone > 12 || time_zone < -12)
	{
		fprintf(stderr, "time_zone must be in [-12,12]\n");
		return (result_ = 255);
	}

	instek::StreamingClient::SetThreadNum(1);
	instek::StreamingClient::StreamingInfo s_info;
	s_info.username = argv[Username_Index];
	s_info.password = argv[Password_Index];
	s_info.remote_ip = argv[IP_Index];
	s_info.camera_id = camera_id;

	s_info.streaming_mode = 1; // (async) live: 0, playback: 1, local: 2; (blocking) live:10, playback: 11, local: 12

	// Playback
	s_info.playback.sec = start_time;
	s_info.playback.data_interval = 0;
	s_info.playback.data_rate = 1;
	//s_info.playback.direction = 3; // forward: 0 , backward: 1, archive with log: 2, archive only: 3 (2 and 3 for hdvr r2881 above)
	s_info.playback.direction = 0; // For compatibility, we use mode 0 now.
	s_info.playback.remote_port = argv[Port_Index];

	StreamingDownloader downloader(s_info,
			end_time,
			iavf_output_path,
			idb_output_path,
			output_filename,
			split_file_size,
			time_zone*60,
			DownloadStatusCallbackFunc,
			DownloadProgressCallbackFunc);

	if(s_info.streaming_mode >= 10)
		downloader.Download(); // For blocking mode

	// Wait for download complete.
	boost::mutex::scoped_lock lock(wait_complete_mutex_);
	if(!is_completed_)
		complete_condition_.wait(lock);

	return result_;
}


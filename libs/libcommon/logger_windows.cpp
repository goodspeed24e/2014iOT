#include <libcommon/logger.hpp>

#include <cstring>
#include <ctime>
#include <cstdlib>

#include <fstream>
#include <iostream>
#include <boost/iostreams/filtering_streambuf.hpp>
#include <boost/iostreams/copy.hpp>
#include <boost/iostreams/filter/gzip.hpp>

#define MAX_INDEX_OF_LOG 7
#define MAX_LOG_FILENAME_LEN 32
#define MAX_LOG_FILENAME_BUF_LEN (MAX_LOG_FILENAME_LEN + 1)

#define LOG_POSTFIX ".log"
#define COMPRESSED_POSTFIX ".gz"

#define MAX_BYTES_OF_LOG (2L*1024L*1024L)
//#define MAX_BYTES_OF_LOG (100L)

#ifdef WIN32
#define PATH_SYMBOL "\\"
#else // WIN32
#include <syslog.h>
#define PATH_SYMBOL "/"
#endif // WIN32

namespace FBi
{

bool Logger::is_register_exit_func_ = false;
FILE* Logger::log_file_ = NULL;
boost::mutex Logger::mutex_;
std::string Logger::current_log_dir_;
std::string Logger::current_log_filename_;
std::string Logger::log_filename_format_;
std::string Logger::compressed_log_filename_format_;


/**
 * @brief Function to translate the priority number to string. (For Windows)
 *
 * @param[in] priority The priority of log.
 * @return The string of priority.
 */
const char* Logger::priority_to_str(int priority)
{
	switch(priority)
	{
	case LOG_EMERG:
		return "EME";
	case LOG_ALERT:
		return "ALE";
	case LOG_CRIT:
		return "CRI";
	case LOG_ERR:
		return "ERR";
	case LOG_WARNING:
		return "WAR";
	case LOG_NOTICE:
		return "NOT";
	case LOG_INFO:
		return "INF";
	case LOG_DEBUG:
		return "DEB";
	default:
		return "NON";
	}
}

/// Function to close log file and clear string of directory without using mutex. (For Windows)
void Logger::unsafe_close_log()
{
	if(log_file_)
	{
		fflush(log_file_);
		fclose(log_file_);
	}
	log_file_ = NULL;
	current_log_dir_.clear();
	current_log_filename_.clear();
	log_filename_format_.clear();
	compressed_log_filename_format_.clear();

}

/**
 * @brief Function to rotate the logs. (For Windows)
 *
 * @return true, if it is successful and false, it is failed.
 */
bool Logger::unsafe_log_rotate()
{
	char temp_buf_1[MAX_LOG_FILENAME_BUF_LEN] = {'\0'};
	char temp_buf_2[MAX_LOG_FILENAME_BUF_LEN] = {'\0'};
	char *old_name = NULL;
	char *new_name = NULL;
	bool retval = true;
	std::string old_name_str;
	std::string new_name_str;
	std::string temp_compressed_path_format;
	std::string temp_log_path_format;

	temp_compressed_path_format = PATH_SYMBOL;
	temp_compressed_path_format += compressed_log_filename_format_;

	temp_log_path_format = PATH_SYMBOL;
	temp_log_path_format += log_filename_format_;


	// Initialize the new name of oldest log.
	_snprintf_s(temp_buf_2, MAX_LOG_FILENAME_LEN, temp_compressed_path_format.c_str(), MAX_INDEX_OF_LOG);
	old_name = temp_buf_1;
	new_name = temp_buf_2;

	for(int i = MAX_INDEX_OF_LOG - 1; i >= 2; --i)
	{
		// Generate the old name of log.
		_snprintf(old_name, MAX_LOG_FILENAME_LEN, temp_compressed_path_format.c_str(), i);

		// Concatenate directory and filename.
		old_name_str = current_log_dir_ + old_name;
		new_name_str = current_log_dir_ + new_name;

		// Rename.
		if(rename(old_name_str.c_str(), new_name_str.c_str()) != 0)
			retval = false;

		// Swap buffer.
		char *temp_ptr = new_name;
		new_name = old_name;
		old_name = temp_ptr;
	}

	// Generate the filename with index 1 and compressed filename with index 2
	_snprintf(old_name, MAX_LOG_FILENAME_LEN, temp_log_path_format.c_str(), 1);
	_snprintf(new_name, MAX_LOG_FILENAME_LEN, temp_compressed_path_format.c_str(), 2);
	old_name_str = current_log_dir_ + old_name;
	new_name_str = current_log_dir_ + new_name;

	// If file exists and the size of file is not zero then we compress it.
	FILE* tempfile = fopen(old_name_str.c_str(), "r");
	if(tempfile)
	{
		long temp_size;
		fseek(tempfile, 0, SEEK_END);
		temp_size = ftell(tempfile);
		fclose(tempfile);
		if(temp_size > 0)
		{
			try
			{
				// Compress file
				std::ifstream in(old_name_str.c_str(), std::ios_base::in | std::ios_base::binary);
				std::ofstream file(new_name_str.c_str(), std::ios_base::out | std::ios_base::binary);
				boost::iostreams::filtering_streambuf<boost::iostreams::output> out;
				out.push(boost::iostreams::gzip_compressor(boost::iostreams::zlib::best_compression));
				out.push(file);
				boost::iostreams::copy(in, out);
			}
			catch(...)
			{
				retval = false;
			}
		}
	}


	// Generate the filename with index 1 and current log filename.
	_snprintf(new_name, MAX_LOG_FILENAME_LEN, temp_log_path_format.c_str(), 1);
	old_name_str = current_log_dir_ + PATH_SYMBOL;
	old_name_str += current_log_filename_;
	new_name_str = current_log_dir_ + new_name;

	// Close current log.
	if(log_file_)
	{
		fflush(log_file_);
		fclose(log_file_);
	}

	// Rename current log.
	if(rename(old_name_str.c_str(), new_name_str.c_str()) != 0)
		retval = false;

	// Open new current log.
	log_file_ = fopen(old_name_str.c_str(), "w");
	if(log_file_ == NULL)
	{
		unsafe_close_log();
		retval = false;
	}

	return retval;
}

/**
 * @brief Function to open the log file and assign the directory of log. (For Windows)
 *
 * @param[in] ident The identification for syslog of Linux.
 * @param[in] log_dir The path that the log files will be placed for Windows.
 * @param[in] log_filename_prefix The prefix of filename of log for Windows.
 * @return true, if it is successful and false, it is failed.
 */
bool Logger::OpenLog(const char* /*ident*/, const char* log_dir, const char* log_filename_prefix)
{
	if(log_dir == NULL) return false;

	boost::mutex::scoped_lock lock(mutex_);

	unsafe_close_log();

	current_log_dir_ = log_dir;

	current_log_filename_ = log_filename_prefix;
	current_log_filename_ += LOG_POSTFIX;
	log_filename_format_ = current_log_filename_ + ".%d";
	compressed_log_filename_format_ = log_filename_format_ + COMPRESSED_POSTFIX;


	std::string current_log_filepath = log_dir;
	current_log_filepath += PATH_SYMBOL;
	current_log_filepath += current_log_filename_;
	log_file_ = fopen(current_log_filepath.c_str(), "a");

	if(log_file_ == NULL)
	{
		unsafe_close_log();
		return false;
	}

	if(!is_register_exit_func_)
		if(::atexit(Logger::close_log) == 0)
			is_register_exit_func_ = true;

	return true;
}

/// Function to close the log. (For Windows)
void Logger::close_log()
{
	boost::mutex::scoped_lock lock(mutex_);
	unsafe_close_log();
}

/**
 * @brief Function to write the log file and rotate the log automatically. (For Windows)
 *
 * @param[in] priority The priority of log.
 * @param[in] fmt The format likes that of printf function.
 * @param[in] ap The arguments like those of vprintf function.
 * @return true, if the log rotation is successful and false, the log rotation is failed.
 */
bool Logger::VLog(int priority, const char* fmt, va_list ap)
{
	if(fmt == NULL) return false;

	bool retval = true;
	char time_str[64] = {'\0'};
	std::time_t temp_time = std::time(NULL);
	struct tm* timeinfo = NULL;
	FILE *stream = NULL;

	boost::mutex::scoped_lock lock(mutex_);

	timeinfo = std::localtime(&temp_time);
	if(strftime(time_str, 64, "%Y.%m.%d %H:%M:%S", timeinfo) == 0)
		memset(time_str, '\0', 64);

	if(log_file_)
	{
		long temp_size = 0;
		temp_size = ftell(log_file_);
		if(temp_size > MAX_BYTES_OF_LOG)
			retval = unsafe_log_rotate();

		stream = log_file_;
	}
	else
	{
		stream = stderr;
	}

	fprintf(stream, "[%s][%-3s]: ", time_str, priority_to_str(priority));
	vfprintf(stream, fmt, ap);
	fflush(stream);

	return retval;
}

/**
 * @brief Function to write the log file and rotate the log automatically. (For Windows)
 *
 * @param[in] priority The priority of log.
 * @param[in] fmt The format likes that of printf function.
 * @param[in] ... The arguments like those of printf function.
 * @return true, if the log rotation is successful and false, the log rotation is failed.
 */
bool Logger::Log(int priority, const char* fmt, ...)
{
	if(fmt == NULL) return false;
	
	bool retval = true;
	va_list ap;
	va_start(ap, fmt);
	retval = VLog(priority, fmt, ap);
	va_end(ap);

	return retval;
}

/**
 * @brief Function to write the log file and rotate the log automatically. The default priority is LOG_WARNING. (For Windows)
 *
 * @param[in] fmt The format likes that of printf function.
 * @param[in] ... The arguments like those of printf function.
 * @return true, if the log rotation is successful and false, the log rotation is failed.
 */
bool Logger::Log(const char* fmt, ...)
{
	if(fmt == NULL) return false;

	bool retval = true;
	va_list ap;
	va_start(ap, fmt);
	retval = VLog(LOG_WARNING, fmt, ap);
	va_end(ap);

	return retval;
}

} // namespace FBi

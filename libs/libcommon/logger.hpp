#ifndef LOGGER_HPP
#define LOGGER_HPP

#include <libcommon/libcommon_config.h>
#include <cstdarg>

#ifdef WIN32
#include <boost/thread/mutex.hpp>
#include <string>
#include <cstdio>
#endif // WIN32

#ifdef WIN32
#define LOG_EMERG 0
#define LOG_ALERT 1
#define LOG_CRIT 2
#define LOG_ERR 3
#define LOG_WARNING 4
#define LOG_NOTICE 5
#define LOG_INFO 6
#define LOG_DEBUG 7
#endif // WIN32

namespace FBi
{

/// Class of log utility.
class LIBCOMMON_DECL Logger
{
public:
	static bool OpenLog(const char* ident, const char* log_dir, const char* log_filename_prefix);
	static bool VLog(int priority, const char* fmt, va_list ap);
	static bool Log(int priority, const char* fmt, ...);
	static bool Log(const char* fmt, ...);

private:
	static void close_log();

private:
	/// Flag to indicate the exit function is registered or not.
	static bool is_register_exit_func_;

#ifdef WIN32	
private:
	static const char* priority_to_str(int priority); 
	static void unsafe_close_log();
	static bool unsafe_log_rotate();

private:
	/// File pointer of log file.
	static FILE* log_file_;
	/// Mutex for manipulating log.
	static boost::mutex mutex_;
	/// String to remember the current log directory.
	static std::string current_log_dir_;
	/// String to remember the current log file name.
	static std::string current_log_filename_;
	/// String to remember the log file name with index.
	static std::string log_filename_format_;
	/// String to remember the log file name with index and the compressed type.
	static std::string compressed_log_filename_format_;
#endif
};

} // namespace FBi

#endif


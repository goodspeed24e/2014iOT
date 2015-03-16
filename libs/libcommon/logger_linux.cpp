#include <libcommon/logger.hpp>
#include <syslog.h>
#include <cstdlib>

namespace FBi
{

bool Logger::is_register_exit_func_ = false;

/**
 * @brief Function to open the log file and assign the directory of log. (For Linux)
 *
 * @param[in] ident The identification for syslog of Linux.
 * @param[in] log_dir The path that the log files will be placed for Windows.
 * @param[in] log_filename_prefix The prefix of filename of log for Windows.
 * @return Always return true.
 */
bool Logger::OpenLog(const char* ident, const char* /*log_dir*/, const char* /*log_filename_prefix*/)
{
	::openlog(ident, LOG_CONS | LOG_PID, LOG_USER);

	if(!is_register_exit_func_)
		if(::atexit(Logger::close_log) == 0)
			is_register_exit_func_ = true;

	return true;
}

/// Function to close the log. (For Linux)
void Logger::close_log()
{
	::closelog();
}

/**
 * @brief Function to write the log file and rotate the log automatically. (For Linux)
 *
 * @param[in] priority The priority of log.
 * @param[in] fmt The format likes that of printf function.
 * @param[in] ap The arguments like those of vprintf function.
 * @return Always return true.
 */
bool Logger::VLog(int priority, const char* fmt, va_list ap)
{
	::vsyslog(priority, fmt, ap);
	return true;
}

/**
 * @brief Function to write the log file and rotate the log automatically. (For Linux)
 *
 * @param[in] priority The priority of log.
 * @param[in] fmt The format likes that of printf function.
 * @param[in] ... The arguments like those of printf function.
 * @return Always return true.
 */
bool Logger::Log(int priority, const char* fmt, ...)
{
	va_list ap;
	va_start(ap, fmt);
	::vsyslog(priority, fmt, ap);
	va_end(ap);
	return true;
}

/**
 * @brief Function to write the log file and rotate the log automatically. The default priority is LOG_WARNING. (For Linux)
 *
 * @param[in] fmt The format likes that of printf function.
 * @param[in] ... The arguments like those of printf function.
 * @return Always return true.
 */
bool Logger::Log(const char* fmt, ...)
{
	va_list ap;
	va_start(ap, fmt);
	::vsyslog(LOG_WARNING, fmt, ap);
	va_end(ap);
	return true;
}

} // namespace FBi

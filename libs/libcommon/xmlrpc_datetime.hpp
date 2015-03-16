#ifndef XMLRPC_DATETIME_HPP
#define XMLRPC_DATETIME_HPP

#include <libcommon/libcommon_config.h>

#include <ctime>
#include <string>

namespace FBi
{

/// Class for date and time of XMLRPC. We only support 2011-12-04T19:27:12 or 20111204T19:27:12 format now.
class LIBCOMMON_DECL XMLRPCDateTime
{
public:
	XMLRPCDateTime();
	XMLRPCDateTime(const struct tm&);

	static bool StringToDateTime(const std::string&, struct tm&);
	static bool DateTimeToString(const struct tm&, std::string&);

	bool SetDateTime(const std::string&);
	bool SetDateTime(const struct tm&);

	const std::string GetDateTimeString() const;
	const struct tm GetDateTime() const;

private:
	void init();
	static bool parse_integer(const std::string& str, size_t& index, size_t num_of_digit, int& value);
	static bool parse_one_char(const std::string& str, size_t& index, char ch);
	static bool parse_year(const std::string& str, size_t& index, int& year);
	static bool parse_month(const std::string& str, size_t& index, int& month);
	static bool parse_day_of_month(const std::string& str, size_t& index, int& mday);
	static bool parse_hour(const std::string& str, size_t& index, int& hour);
	static bool parse_min(const std::string& str, size_t& index, int& min);
	static bool parse_sec(const std::string& str, size_t& index, int& sec);
	static bool parse_date_time_string(const std::string& str, int& year, int& month, int& mday, int& hour, int& min, int& sec);

private:
	/// Struct to store the information of date and time
	struct tm date_time_;
};

} // namespace FBi

#endif


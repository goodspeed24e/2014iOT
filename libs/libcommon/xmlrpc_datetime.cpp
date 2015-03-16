#include <libcommon/xmlrpc_datetime.hpp>
#include <cctype>

namespace FBi
{
/// Constructor of XMLRPCDateTime.
XMLRPCDateTime::XMLRPCDateTime()
{
	init();
}

/**
 * @brief Constructor of XMLRPCDateTime.
 *
 * @param[in] date_time The input struct tm.
 */
XMLRPCDateTime::XMLRPCDateTime(const struct tm& date_time)
	: date_time_(date_time)
{
}

/**
 * @brief Function to transfer string to struct tm.
 *
 * @param[in] str The input string.
 * @param[out] timeinfo The output struct tm.
 * @return true, if it is successful, and false, if it is failed.
 */
bool XMLRPCDateTime::StringToDateTime(const std::string& str, struct tm& timeinfo)
{
	int year, month, mday, hour, min, second;
	if(!parse_date_time_string(str, year, month, mday, hour, min, second))
		return false;

	// We modify some values to follow the struct tm format.
	timeinfo.tm_sec = second;
	timeinfo.tm_min = min;
	timeinfo.tm_hour = hour;
	timeinfo.tm_mday = mday;
	timeinfo.tm_mon = month - 1;
	timeinfo.tm_year = year - 1900;
	timeinfo.tm_wday = 0;
	timeinfo.tm_yday = 0;
	timeinfo.tm_isdst = 0;

	return true;
}

/**
 * @brief Function to transfer struct tm to string.
 *
 * @param[in] timeinfo The input struct tm.
 * @param[out] str The output string.
 * @return true, if it is successful, and false, if it is failed.
 */
bool XMLRPCDateTime::DateTimeToString(const struct tm& timeinfo, std::string& str)
{
	char buffer[128] = {'\0'};

	// 20111204T19:27:12
	if(strftime(buffer, 128, "%Y%m%dT%H:%M:%S", &timeinfo) == 0)
		return false;

	str = buffer;

	return true;
}

/**
 * @brief Function to set date and time from string.
 *
 * @param[in] str The input string.
 * @return true, if it is successful, and false, if it is failed.
 */
bool XMLRPCDateTime::SetDateTime(const std::string& str)
{
	return XMLRPCDateTime::StringToDateTime(str, date_time_);
}

/**
 * @brief Function to set date and time from struct tm.
 *
 * @param[in] date_time The input struct tm.
 * @return true, if it is successful, and false, if it is failed.
 */
bool XMLRPCDateTime::SetDateTime(const struct tm& date_time)
{
	 date_time_ = date_time;
	 return true;
}

/**
 * @brief Function to get the string of date and time.
 *
 * @return The const input string.
 */
const std::string XMLRPCDateTime::GetDateTimeString() const
{
	std::string str;
	XMLRPCDateTime::DateTimeToString(date_time_, str);

	return str;
}

/**
 * @brief Function to get the struct tm of date and time.
 *
 * @return The const input struct tm.
 */
const struct tm XMLRPCDateTime::GetDateTime() const
{
	return date_time_;
}

/// Function to initialize the internal struct tm.
void XMLRPCDateTime::init()
{
	date_time_.tm_sec = 0;
	date_time_.tm_min = 0;
	date_time_.tm_hour = 0;
	date_time_.tm_mday = 1;
	date_time_.tm_mon = 0;
	date_time_.tm_year = 0;
	date_time_.tm_wday = 1;
	date_time_.tm_yday = 0;
	date_time_.tm_isdst = 0;
}

/**
 * @brief Function to parse integer.
 *
 * @param[in] str The input string.
 * @param[in,out] index The start index of this paring step. It will be advanced if parsing one character is successful.
 * @param[in] num_of_digit The number of digit should be parsed.
 * @param[out] value The output integer.
 * @return true, if it is successful, and false, if it is failed.
 */
bool XMLRPCDateTime::parse_integer(const std::string& str, size_t& index, size_t num_of_digit, int& value)
{
	int temp_value = 0;
	size_t i = 0;

	for(; (i < num_of_digit) && (index < str.length()); ++i, ++index)
	{
		if(!isdigit(str[index]))
			return false;

		temp_value = temp_value*10 + static_cast<int>(str[index] - '0');
	}

	if((i == num_of_digit))
	{
		value = temp_value;
		return true;
	}
	else
		return false;
}

/**
 * @brief Function to parse one character.
 *
 * @param[in] str The input string.
 * @param[in,out] index The start index of this paring step. It will be advanced if parsing one character is successful.
 * @param[in] ch The character should be matched.
 * @return true, if it is successful, and false, if it is failed.
 */
bool XMLRPCDateTime::parse_one_char(const std::string& str, size_t& index, char ch)
{
	if((index < str.length()) && (str[index] == ch))
	{
		++index;
		return true;
	}
	else
	{
		return false;
	}
}

/**
 * @brief Function to parse 4-digits year.
 *
 * @param[in] str The input string.
 * @param[in,out] index The start index of this paring step. It will be advanced if parsing one character is successful.
 * @param[out] year The output integer of year.
 * @return true, if it is successful, and false, if it is failed.
 */
bool XMLRPCDateTime::parse_year(const std::string& str, size_t& index, int& year)
{
	return parse_integer(str, index, 4, year);
}

/**
 * @brief Function to parse 2-digits month (Range: 1-12).
 *
 * @param[in] str The input string.
 * @param[in,out] index The start index of this paring step. It will be advanced if parsing one character is successful.
 * @param[out] month The output integer of month.
 * @return true, if it is successful, and false, if it is failed.
 */
bool XMLRPCDateTime::parse_month(const std::string& str, size_t& index, int& month)
{
	int temp_value = 0;

	if(parse_integer(str, index, 2, temp_value) && (temp_value >= 1) && (temp_value <= 12))
	{
		month = temp_value;
		return true;
	}
	else
	{
		return false;
	}
}

/**
 * @brief Function to parse 2-digits day of the month (Range: 1-31). We do not check the leap year etc.
 *
 * @param[in] str The input string.
 * @param[in,out] index The start index of this paring step. It will be advanced if parsing one character is successful.
 * @param[out] mday The output integer of day of the month.
 * @return true, if it is successful, and false, if it is failed.
 */
bool XMLRPCDateTime::parse_day_of_month(const std::string& str, size_t& index, int& mday)
{
	int temp_value = 0;

	// We do not check 28, 29, 30, 31 days of different month and year here.
	if(parse_integer(str, index, 2, temp_value) && (temp_value >= 1) && (temp_value <= 31))
	{
		mday = temp_value;
		return true;
	}
	else
	{
		return false;
	}
}

/**
 * @brief Function to parse 2-digits hour (Range: 0-23).
 *
 * @param[in] str The input string.
 * @param[in,out] index The start index of this paring step. It will be advanced if parsing one character is successful.
 * @param[out] hour The output integer of hour.
 * @return true, if it is successful, and false, if it is failed.
 */
bool XMLRPCDateTime::parse_hour(const std::string& str, size_t& index, int& hour)
{
	int temp_value = 0;

	if(parse_integer(str, index, 2, temp_value) && (temp_value >= 0) && (temp_value <= 23))
	{
		hour = temp_value;
		return true;
	}
	else
	{
		return false;
	}
}

/**
 * @brief Function to parse 2-digits minute (Range: 0-59).
 *
 * @param[in] str The input string.
 * @param[in,out] index The start index of this paring step. It will be advanced if parsing one character is successful.
 * @param[out] min The output integer of minute.
 * @return true, if it is successful, and false, if it is failed.
 */
bool XMLRPCDateTime::parse_min(const std::string& str, size_t& index, int& min)
{
	int temp_value = 0;

	if(parse_integer(str, index, 2, temp_value) && (temp_value >= 0) && (temp_value <= 59))
	{
		min = temp_value;
		return true;
	}
	else
	{
		return false;
	}
}

/**
 * @brief Function to parse 2-digits second (Range: 0-59).
 *
 * @param[in] str The input string.
 * @param[in,out] index The start index of this paring step. It will be advanced if parsing one character is successful.
 * @param[out] sec The output integer of second.
 * @return true, if it is successful, and false, if it is failed.
 */
bool XMLRPCDateTime::parse_sec(const std::string& str, size_t& index, int& sec)
{
	int temp_value = 0;

	if(parse_integer(str, index, 2, temp_value) && (temp_value >= 0) && (temp_value <= 59))
	{
		sec = temp_value;
		return true;
	}
	else
	{
		return false;
	}
}

/**
 * @brief Function to parse ISO 8601 format string. We only support 2011-12-04T19:27:12 or 20111204T19:27:12 format now.
 *
 * @param[in] str The input string.
 * @param[out] year The output year.
 * @param[out] month The output month.
 * @param[out] mday The output day of the month.
 * @param[out] hour The output hour.
 * @param[out] min The output minute.
 * @param[out] sec The output second.
 * @return true, if it is successful, and false, if it is failed.
 */
bool XMLRPCDateTime::parse_date_time_string(const std::string& str, int& year, int& month, int& mday, int& hour, int& min, int& sec)
{
	// 2011-12-04T19:27:12 or 20111204T19:27:12
	size_t index = 0;
	int temp_value;

	// Year
	temp_value = 0;
	if(!parse_year(str, index, temp_value))
		return false;

	year = temp_value;

	// Dash
	// If it is dash, we advance the index and skip it. Otherwise, we parse next number.
	parse_one_char(str, index, '-');

	// Month
	temp_value = 0;
	if(!parse_month(str, index, temp_value))
		return false;

	month = temp_value;

	// Dash
	// If it is dash, we advance the index and skip it. Otherwise, we parse next number.
	parse_one_char(str, index, '-');
	
	// Day of the month
	temp_value = 0;
	if(!parse_day_of_month(str, index, temp_value))
		return false;

	mday = temp_value;

	// T
	if(!parse_one_char(str, index, 'T'))
		return false;

	// Hour
	temp_value = 0;
	if(!parse_hour(str, index, temp_value))
		return false;

	hour = temp_value;

	// Colon
	if(!parse_one_char(str, index, ':'))
		return false;

	// Minute
	temp_value = 0;
	if(!parse_min(str, index, temp_value))
		return false;

	min = temp_value;

	// Colon
	if(!parse_one_char(str, index, ':'))
		return false;

	// Second
	temp_value = 0;
	if(!parse_sec(str, index, temp_value))
		return false;

	sec = temp_value;

	return true;
}

} // namespace FBi

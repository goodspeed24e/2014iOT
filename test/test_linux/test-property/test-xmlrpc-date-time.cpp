#include <libcommon/xmlrpc_datetime.hpp>
#include <iostream>

int main()
{
	struct tm date_time1;

	// Using static function ===========
	// Pass invalid string
	std::cout << "Test invalid string =====================" << std::endl;
	if(instek::XMLRPCDateTime::StringToDateTime("20a1-12-0bT19:27:12", date_time1))
		std::cout << "valid date time string" << std::endl;
	else
		std::cout << "invalid date time string" << std::endl;


	// Pass valid string
	std::cout << "Test valid string =====================" << std::endl;
	if(instek::XMLRPCDateTime::StringToDateTime("2011-12-04T19:27:12", date_time1))
	{
		char buffer[128] = {'\0'};
		strftime(buffer, 128, "%Y-%m-%dT%H:%M:%S", &date_time1);
		std::cout << buffer << std::endl;
	}
	else
	{
		std::cout << "invalid date time string" << std::endl;
	}

	if(instek::XMLRPCDateTime::StringToDateTime("20111204T19:27:12", date_time1))
	{
		char buffer[128] = {'\0'};
		strftime(buffer, 128, "%Y-%m-%dT%H:%M:%S", &date_time1);
		std::cout << buffer << std::endl;
	}
	else
	{
		std::cout << "invalid date time string" << std::endl;
	}

	{
		std::cout << "Test date time to string =====================" << std::endl;
		std::time_t result = std::time(NULL);
		struct tm* timeinfo = NULL;
		timeinfo = std::localtime(&result);

		std::string str;

		if(instek::XMLRPCDateTime::DateTimeToString(*timeinfo, str))
			std::cout << "struct time to IO8601 = " << str << std::endl;
		else
			std::cout << "invalid struct tm" << std::endl;

	}

	{
		std::cout << "Test GetDateTime interface =====================" << std::endl;
		std::time_t result = std::time(NULL);
		struct tm* timeinfo = NULL;
		timeinfo = std::localtime(&result);

		instek::XMLRPCDateTime obj(*timeinfo);
		struct tm ret_time_info;

		std::cout << "Get date time string = " << obj.GetDateTimeString() << std::endl;

		ret_time_info = obj.GetDateTime();
		std::cout << "Get date time struct = ";
		char buffer[128] = {'\0'};
		strftime(buffer, 128, "%Y-%m-%dT%H:%M:%S", &ret_time_info);
		std::cout << buffer << std::endl;
	}

	{
		std::cout << "Test SetDateTime interface =====================" << std::endl;

		instek::XMLRPCDateTime obj;
		std::string original_str;

		original_str = "2019-05-26T23:11:56";
		std::cout << "Input String = " << original_str << std::endl;
		if(obj.SetDateTime(original_str))
			std::cout << "Get date time string which we set from string = " << obj.GetDateTimeString() << std::endl;
		else
			std::cout << "Can not set date time string" << std::endl;

		original_str = "20190526T23:11:56";
		std::cout << "Input String = " << original_str << std::endl;
		if(obj.SetDateTime(original_str))
			std::cout << "Get date time string which we set from string = " << obj.GetDateTimeString() << std::endl;
		else
			std::cout << "Can not set date time string" << std::endl;


		std::time_t result = std::time(NULL);
		struct tm* timeinfo = NULL;
		timeinfo = std::localtime(&result);

		if(obj.SetDateTime(*timeinfo))
			std::cout << "Get date time string which we set from struct tm = " << obj.GetDateTimeString() << std::endl;
		else
			std::cout << "Can not set date time string" << std::endl;

	}

	return 0;
}

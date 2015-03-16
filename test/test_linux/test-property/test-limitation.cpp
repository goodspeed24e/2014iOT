#include <iostream>

#include <libcommon/properties_value_general_limitation.hpp>

void Check(instek::IPropertiesValueLimitation& limitation, instek::Properties::ValueType& value)
{
	if(limitation.Check(value))
		std::cout << "OK" << std::endl;
	else
		std::cout << "Failed" << std::endl;
}


int main()
{
	instek::PropertiesValueRangeLimitation<int> int_range_limitation(23, 45);
	instek::PropertiesValueRangeLimitation<double> double_range_limitation(-0.24, 34.0);
	instek::PropertiesValueTypeLimitation<int> int_type_limitation;
	instek::PropertiesValueTypeLimitation<double> double_type_limitation;

	instek::Properties s1;
	s1["cam"]["value_int"] = 34;
	s1["cam"]["value_double"] = 8.7;
	s1["cam"]["invalid_value_int"] = 1;
	s1["cam"]["invalid_value_double"] = -6.0;

	Check(int_range_limitation, s1["cam"]["value_int"]);
	Check(double_range_limitation, s1["cam"]["value_double"]);
	Check(int_range_limitation, s1["cam"]["invalid_value_int"]);
	Check(double_range_limitation, s1["cam"]["invalid_value_double"]);

	instek::Properties::ValueType value;
	s1.Get("cam:value_int", value);
	std::cout << static_cast<int>(value) << std::endl;
	s1.Get("cam:value_double", value);
	std::cout << static_cast<double>(value) << std::endl;
	s1.Get("cam:invalid_value_int", value);
	std::cout << static_cast<int>(value) << std::endl;
	s1.Get("cam:invalid_value_double", value);
	std::cout << static_cast<double>(value) << std::endl;
	s1.Get("cam", value);
	std::cout << static_cast<double>(value["invalid_value_double"]) << std::endl;

	if(!s1.Get("cam:tt", value))
		std::cout << "Get Failed" << std::endl;

	std::cout << "Check type ==========================" << std::endl;
	Check(int_type_limitation, s1["cam"]["value_int"]);
	Check(double_type_limitation, s1["cam"]["value_int"]);
	Check(int_type_limitation, s1["cam"]["value_double"]);
	Check(double_type_limitation, s1["cam"]["value_double"]);

	if(!s1.Set("cam:test_int", 7))
		std::cout << "Set Failed" << std::endl;
	s1.Set("cam:value_int", 7);
	s1.Get("cam:value_int", value);
	std::cout << static_cast<int>(value) << std::endl;

	std::cout << "Test Set properties ==================" << std::endl;
	instek::Properties s2;
	s2["value_int2"] = 25;
	s2["value_double2"] = 17.7;
	if(!s1.Set("cam", s2))
		std::cout << "Set Failed" << std::endl;

	if(s1.Get("cam:value_int", value))
		std::cout << static_cast<int>(value) << std::endl;
	else
		std::cout << "Get Failed" << std::endl;

	if(s1.Get("cam:value_double", value))
		std::cout << static_cast<double>(value) << std::endl;
	else
		std::cout << "Get Failed" << std::endl;

	if(s1.Get("cam:value_int2", value))
		std::cout << static_cast<int>(value) << std::endl;
	else
		std::cout << "Get Failed" << std::endl;

	if(s1.Get("cam:value_double2", value))
		std::cout << static_cast<double>(value) << std::endl;
	else
		std::cout << "Get Failed" << std::endl;



	return 0;
}

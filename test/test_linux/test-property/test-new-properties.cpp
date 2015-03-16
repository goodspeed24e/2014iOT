#include <iostream>

#include <libcommon/properties_value_general_limitation.hpp>
#include <boost/shared_ptr.hpp>

void NotifySlot(const instek::Properties& /*prop*/)
{
	std::cout << "NotifySlot" << std::endl;
}

int main()
{
	boost::shared_ptr<instek::IPropertiesValueLimitation> int_range_limitation(new instek::PropertiesValueRangeLimitation<int>(23,45));
	boost::shared_ptr<instek::IPropertiesValueLimitation> double_range_limitation(new instek::PropertiesValueRangeLimitation<double>(-0.24, 34.0));
	boost::shared_ptr<instek::IPropertiesValueLimitation> int_type_limitation(new instek::PropertiesValueTypeLimitation<int>);
	boost::shared_ptr<instek::IPropertiesValueLimitation> double_type_limitation(new instek::PropertiesValueTypeLimitation<double>);

	instek::Properties s1;
	instek::Properties::PropertyNode extension;
	instek::Properties::ValueType value;

	s1.RegisterValueChangedSlot(NotifySlot);

	extension.Reset();
	extension.value = 34;
	extension.limitation_list.push_back(int_range_limitation);
	extension.limitation_list.push_back(int_type_limitation);
	extension.is_writable = true;
	s1.Put("cam:value_int", extension);


	extension.Reset();
	extension.value = 8.7;
	extension.limitation_list.push_back(double_range_limitation);
	extension.limitation_list.push_back(double_type_limitation);
	extension.is_writable = true;
	s1.Put("cam:value_double", extension);

	extension.Reset();
	extension.value = 14.3;
	extension.limitation_list.push_back(double_range_limitation);
	extension.limitation_list.push_back(double_type_limitation);
	extension.is_writable = false;
	s1.Put("cam:value_double_read_only", extension);

	std::cout << "Test set value then get it ==========================" << std::endl;
	if(s1.Get("cam:value_int", value))
		std::cout << "Value = " << static_cast<int>(value) << std::endl;
	if(s1.Get("cam:value_double", value))
		std::cout << "Value = " << static_cast<double>(value) << std::endl;


	std::cout << "Test set valid value ==========================" << std::endl;
	s1.Set("cam:value_int", 45);
	s1.Set("cam:value_double", 23.8);

	std::cout << "Test set invalid value ==========================" << std::endl;
	if(!s1.Set("cam:value_int", 1))
		std::cout << "Set Value Failed" << std::endl;
	if(!s1.Set("cam:value_int", 0.1))
		std::cout << "Set Value Failed" << std::endl;
	if(!s1.Set("cam:value_double", -6.7))
		std::cout << "Set Value Failed" << std::endl;
	if(!s1.Set("cam:value_double", 24))
		std::cout << "Set Value Failed" << std::endl;
	if(!s1.Set("cam:value_double_read_only", 23.7))
		std::cout << "Set Value Failed" << std::endl;


	std::cout << "Test get value ==========================" << std::endl;
	if(s1.Get("cam:value_int", value))
		std::cout << "Value = " << static_cast<int>(value) << std::endl;
	if(s1.Get("cam:value_double", value))
		std::cout << "Value = " << static_cast<double>(value) << std::endl;
	if(s1.Get("cam:value_double_read_only", value))
		std::cout << "Value = " << static_cast<double>(value) << std::endl;

	std::cout << "Test get value extension ==========================" << std::endl;
	if(s1.Get("cam:value_int", extension))
		std::cout << "Value = " << static_cast<int>(extension.value) << std::endl;
	if(s1.Get("cam:value_double", extension))
		std::cout << "Value = " << static_cast<double>(extension.value) << std::endl;
	if(s1.Get("cam:value_double_read_only", extension))
		std::cout << "Value = " << static_cast<double>(extension.value) << std::endl;

	std::cout << "Test get value or exxtension of invalid node ==========================" << std::endl;
	if(!s1.Get("cam:invalid_node", value))
		std::cout << "Get Value Failed" << std::endl;
	if(!s1.Get("cam:invalid_node", extension))
		std::cout << "Get Value Extension Failed" << std::endl;

	std::cout << "Test add value ==========================" << std::endl;
	if(!s1.Add("cam:value_int", 45))
		std::cout << "Can not add value into cam:value_int" << std::endl;
	if(s1.Add("cam:new_node", "test new node"))
	{
		std::cout << "Add new node cam:new_node. The value = ";
		if(s1.Get("cam:new_node", value))
			std::cout << static_cast<std::string>(value);
		std::cout << std::endl;
	}
	else
	{
		std::cout << "Add new node cam:new_node failed." << std::endl;
	}

	if(s1.Del("cam:new_node"))
		std::cout << "Delete node success." << std::endl;
	else
		std::cout << "Delete node fail." << std::endl;

	if(s1.Get("cam:new_node", value))
		std::cout << static_cast<std::string>(value);
	else
		std::cout << "Can not get the value of node cam:new_node after deleting the node." << std::endl;


	std::cout << "Test import value ======================================================= " << std::endl;

	instek::Properties s2;

	s2.RegisterValueChangedSlot(NotifySlot);

	extension.Reset();
	extension.value = 34;
	extension.limitation_list.push_back(int_range_limitation);
	extension.limitation_list.push_back(int_type_limitation);
	extension.is_writable = true;
	s2.Put("cam:value_int", extension);

	extension.Reset();
	extension.value = 8.7;
	extension.limitation_list.push_back(double_range_limitation);
	extension.limitation_list.push_back(double_type_limitation);
	extension.is_writable = true;
	s2.Put("cam:value_double", extension);

	extension.Reset();
	extension.value = 14.3;
	extension.limitation_list.push_back(double_range_limitation);
	extension.limitation_list.push_back(double_type_limitation);
	extension.is_writable = false;
	s2.Put("cam:value_double_read_only", extension);

	std::cout << "Before import valid value" << std::endl;
	if(s2.Get("cam:value_int", value))
		std::cout << "cam:value_int = " << static_cast<int>(value) << std::endl;
	if(s2.Get("cam:value_double", value))
		std::cout << "cam:value_double = " << static_cast<double>(value) << std::endl;
	if(s2.Get("cam:value_double_read_only", value))
		std::cout << "cam:value_double_read_only = " << static_cast<double>(value) << std::endl;

	std::cout << "After import valid value (not allow error)" << std::endl;
	instek::Properties s3;
	s3["cam"]["value_int"] = 27;
	s3["cam"]["value_double"] = 32.7;

	s2.ImportValues(s3, false);

	if(s2.Get("cam:value_int", value))
		std::cout << "cam:value_int = " << static_cast<int>(value) << std::endl;
	if(s2.Get("cam:value_double", value))
		std::cout << "cam:value_double = " << static_cast<double>(value) << std::endl;
	if(s2.Get("cam:value_double_read_only", value))
		std::cout << "cam:value_double_read_only = " << static_cast<double>(value) << std::endl;


	std::cout << "After import invalid value (not allow error)" << std::endl;
	s3["cam"]["value_int"] = 33;
	s3["cam"]["value_double"] = 100.0;
	s2.ImportValues(s3, false);
	if(s2.Get("cam:value_int", value))
		std::cout << "cam:value_int = " << static_cast<int>(value) << std::endl;
	if(s2.Get("cam:value_double", value))
		std::cout << "cam:value_double = " << static_cast<double>(value) << std::endl;
	if(s2.Get("cam:value_double_read_only", value))
		std::cout << "cam:value_double_read_only = " << static_cast<double>(value) << std::endl;

	std::cout << "After import invalid value (allow error)" << std::endl;
	s2.ImportValues(s3, true);
	if(s2.Get("cam:value_int", value))
		std::cout << "cam:value_int = " << static_cast<int>(value) << std::endl;
	if(s2.Get("cam:value_double", value))
		std::cout << "cam:value_double = " << static_cast<double>(value) << std::endl;
	if(s2.Get("cam:value_double_read_only", value))
		std::cout << "cam:value_double_read_only = " << static_cast<double>(value) << std::endl;

	std::cout << "After import invalid non-exist node (not allow error)" << std::endl;
	s3["cam"]["value_double"] = 22.0;
	s3["cam"]["test_node"]["invalid_node1"] = "1";
	s3["cam"]["test_node"]["invalid_node2"] = "2";
	s3["cam"]["test_node"]["test_node"]["invalid_node3"] = "3";
	s2.ImportValues(s3, false);
	if(s2.Get("cam:value_int", value))
		std::cout << "cam:value_int = " << static_cast<int>(value) << std::endl;
	if(s2.Get("cam:value_double", value))
		std::cout << "cam:value_double = " << static_cast<double>(value) << std::endl;
	if(s2.Get("cam:value_double_read_only", value))
		std::cout << "cam:value_double_read_only = " << static_cast<double>(value) << std::endl;
	if(!s2.Get("cam:test_node", value))
		std::cout << "Can not get the cam:test_node" << std::endl;

	std::cout << "After import invalid non-exist node (allow error)" << std::endl;
	s2.ImportValues(s3, true);
	if(s2.Get("cam:value_int", value))
		std::cout << "cam:value_int = " << static_cast<int>(value) << std::endl;
	if(s2.Get("cam:value_double", value))
		std::cout << "cam:value_double = " << static_cast<double>(value) << std::endl;
	if(s2.Get("cam:value_double_read_only", value))
		std::cout << "cam:value_double_read_only = " << static_cast<double>(value) << std::endl;
	if(!s2.Get("cam:test_node", value))
		std::cout << "Can not get the cam:test_node" << std::endl;

	// Important: The concept of shared_ptr
	printf("==== The important concept of Property. ====\n");
	instek::Properties s4;
	instek::Properties s5;
	instek::Properties::ValueType value2;
	s4["cam"]["value_int"] = 27;

	s4.Get("cam", value2);
	s5 = value2;
	s5["value_int"] = 13;
	s5["value_str"] = "add string";

	printf("s4[cam][value_int] = %d\n", (int)s4["cam"]["value_int"]); // 13
	printf("s4[cam][value_str] = %s\n", ((std::string)(s4["cam"]["value_str"])).c_str()); // add string


	instek::Properties s6;
	instek::XMLRPCArray array;
	for(int i = 0; i < 2; ++i)
	{
		s6["key1"] = i;
		array.push_back(s6);
	}

	// It will print
	// 1
	// 1
	for(size_t i = 0; i < array.size(); ++i)
	{
		printf("%d\n", (int)array[i]["key1"]);
	}

	array.clear();
	for(int i = 0; i < 2; ++i)
	{
		instek::Properties s7;
		s7["key1"] = i;
		array.push_back(s7);
	}

	// It will print
	// 0
	// 1
	for(size_t i = 0; i < array.size(); ++i)
	{
		printf("%d\n", (int)array[i]["key1"]);
	}

	return 0;
}

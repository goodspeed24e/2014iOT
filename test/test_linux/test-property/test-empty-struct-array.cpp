#include <iostream>
#include <libtransport/method.hpp>


int main()
{
	// Array and Properties to Property Tree
	instek::XMLRPCArray empty_array;
	instek::Properties empty_properties;

	instek::MethodResponse resp1;
	instek::MethodResponse resp2;

	resp1.AddArray(empty_array);
	resp2.AddProperties(empty_properties);

	std::cout << resp1.GetXML() << std::endl;
	std::cout << resp2.GetXML() << std::endl;

	// Property Tree to Array and Properties
	instek::XMLRPCArray empty_array2;
	instek::Properties empty_properties2;

	instek::Properties::PropertyTreeToArray(boost::property_tree::ptree(), empty_array2);
	instek::Properties::PropertyTreeToProperties(boost::property_tree::ptree(), empty_properties2);

	std::cout << empty_array2.size() << std::endl;
	std::cout << empty_properties2.GetContainerPtr()->size() << std::endl;

	return 0;
}

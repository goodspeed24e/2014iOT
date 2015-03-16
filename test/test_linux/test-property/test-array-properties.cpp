#include <libcommon/properties.hpp>
#include <iostream>

using namespace std;

int main()
{
	instek::Properties prop;

	if(!prop.Load("./test-array-properties-in.xml"))
		std::cout << "Load failed!" << std::endl;
	else
		std::cout << "Load success!" << std::endl;

	std::cout << "lowerBound = " << static_cast<int>(prop["lowerBound"]) << endl;
	std::cout << "upperBound = " << static_cast<int>(prop["upperBound"]) << endl;

	std::cout << "struct in struct ====== > inner_struct_1" << endl;
	instek::Properties prop2 = prop["inner_struct_1"];
	std::cout << "min = " << static_cast<double>(prop2["min"]) << endl;
	std::cout << "max = " << static_cast<double>(prop2["max"]) << endl;

	std::cout << "array in struct ====== > testArray" << endl;
	instek::XMLRPCArray array = prop["testArray"];

	std::cout << "size of testArray = " << array.size() << endl;
	std::cout << "testArray[0] = " << static_cast<int>(array[0]) << endl;
	std::cout << "testArray[1] = " << static_cast<std::string>(array[1]) << endl;
	std::cout << "testArray[2] = " << static_cast<bool>(array[2]) << endl;
	std::cout << "testArray[3] = " << static_cast<int>(array[3]) << endl;

	std::cout << "struct in array ====== > testArray" << endl;
	instek::Properties prop3 = array[4];
	std::cout << "testArray[4][\"struct_in_array\"] = " << static_cast<std::string>(prop3["struct_in_array"]) << endl;

	instek::XMLRPCArray array2 = prop3["testArray2"];
	std::cout << "testArray2 ====== >" << endl;
	std::cout << "size of testArray2 = " << array2.size() << endl;
	std::cout << "testArray2[0] = " << static_cast<int>(array2[0]) << endl;
	std::cout << "testArray2[1] = " << static_cast<std::string>(array2[1]) << endl;

	std::cout << "array in array ====== >" << endl;
	instek::XMLRPCArray array3 = array[5];
	std::cout << "testArray3[0] = " << static_cast<std::string>(array3[0]) << endl;
	std::cout << "testArray3[1] = " << static_cast<bool>(array3[1]) << endl;

	if(!prop.Save("./test-array-properties-out.xml"))
		std::cout << "Save failed!" << std::endl;
	else
		std::cout << "Save success!" << std::endl;

	return 0;
}

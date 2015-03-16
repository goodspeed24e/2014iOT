#include <libcommon/properties.hpp>
#include <boost/filesystem.hpp>

#define OUTPUT_DIR "copy_output_dir"

int main()
{
	boost::filesystem::create_directories(OUTPUT_DIR);

	// =========== Copy properties ===========
	instek::Properties org_prop1;
	instek::Properties copied_prop1;
	instek::XMLRPCArray sub_array1;
	instek::Properties sub_sub_prop1;
	instek::XMLRPCArray sub_sub_array1;
	instek::Properties s3_prop1;
	instek::XMLRPCArray s3_array1;

	s3_array1.push_back(77);
	s3_array1.push_back(true);
	s3_array1.push_back(3.7);

	s3_prop1["s3_bool"] = false;
	s3_prop1["s3_int"] = 613;

	sub_sub_array1.push_back(98);
	sub_sub_array1.push_back("sub_sub_str");
	sub_sub_array1.push_back(s3_array1);
	sub_sub_array1.push_back(true);
	sub_sub_array1.push_back(s3_prop1);

	sub_sub_prop1["sub_str"] = "test_sub_str";
	sub_sub_prop1["sub_int"] = 622;
	sub_sub_prop1["sub_prop"]["name"] = "May";

	sub_array1.push_back(45);
	sub_array1.push_back("array_str");
	sub_array1.push_back(sub_sub_prop1);
	sub_array1.push_back(4.62);
	sub_array1.push_back(sub_sub_array1);

	org_prop1["int"] = 3;
	org_prop1["str"] = "test_str";
	org_prop1["doule"] = 3.64;
	org_prop1["prop"]["name"] = "Joe";
	org_prop1["prop"]["type"] = 3;
	org_prop1["array"] = sub_array1;

	// Copy and modify the copied properties.
	org_prop1.GetProperties("*", copied_prop1);
	copied_prop1["int"] = 4;
	instek::XMLRPCArray& temp_array1 = copied_prop1["array"];
	temp_array1[0] = 46;
	instek::Properties& temp_prop1 = temp_array1[2];
	temp_prop1["sub_int"] = 633;
	instek::XMLRPCArray& temp_array2 = temp_array1[4];
	temp_array2[0] = 99;

	// Save to XML file.
	org_prop1.Save(OUTPUT_DIR "/org_prop1.xml");
	copied_prop1.Save(OUTPUT_DIR "/copied_prop1.xml");

	// =========== Copy array ===========
	instek::XMLRPCArray copied_array1;
	instek::Properties::CopyXMLRPCArray(sub_array1, copied_array1);

	copied_array1[3] = 4.63;
	instek::Properties& temp_prop2 = copied_array1[2];
	instek::XMLRPCArray& temp_array3 = copied_array1[4];

	temp_prop2["sub_prop"]["name"] = "Mary";
	temp_array3[3] = false;

	instek::Properties temp_out1;
	instek::Properties temp_out2;

	temp_out1["out_array"] = sub_array1;
	temp_out2["out_array"] = copied_array1;

	temp_out1.Save(OUTPUT_DIR "/sub_array1.xml");
	temp_out2.Save(OUTPUT_DIR "/copied_sub_array1.xml");

	return 0;
}

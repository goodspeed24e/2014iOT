#include <libcommon/properties.hpp>
#include <boost/filesystem.hpp>

#define OUTPUT_DIR "sub_properties_output_dir"
#define MERGE_OUTPUT_DIR "sub_properties_merge_output_dir"

int main()
{
	boost::filesystem::create_directories(OUTPUT_DIR);
	boost::filesystem::create_directories(MERGE_OUTPUT_DIR);

	instek::Properties org_prop;
	instek::Properties sub_prop;
	instek::Properties sub_prop2;
	instek::XMLRPCArray sub_array;
	instek::Properties s2_prop;
	instek::XMLRPCArray s2_array;

	s2_array.push_back(621);
	s2_array.push_back("Happy");
	s2_array.push_back(25.71);
	s2_array.push_back(false);

	s2_prop["int"] = 512;
	s2_prop["bool"] = true;
	s2_prop["str"] = "Steve";

	sub_array.push_back(13);
	sub_array.push_back(s2_array);
	sub_array.push_back(s2_prop);

	org_prop["cam"]["1"]["name"] = "Cam-01";
	org_prop["cam"]["2"]["name"] = "Cam-02";
	org_prop["cam"]["3"]["name"] = "Cam-03";
	org_prop["cam"]["4"]["name"] = "Cam-04";
	org_prop["cam"]["5"]["name"] = "Cam-05";
	org_prop["cam"]["6"]["name"] = "Cam-06";
	org_prop["cam"]["7"]["name"] = "Cam-07";

	org_prop["cam"]["1"]["status"] = "REC-01";
	org_prop["cam"]["2"]["status"] = "REC-02";
	org_prop["cam"]["3"]["status"] = "REC-03";
	org_prop["cam"]["4"]["status"] = "REC-04";
	org_prop["cam"]["5"]["status"] = "REC-05";
	org_prop["cam"]["6"]["status"] = "REC-06";
	org_prop["cam"]["7"]["status"] = "REC-07";

	org_prop["mycam"]["1"]["name"] = "MyCam-01";
	org_prop["mycam"]["2"]["name"] = "MyCam-02";
	org_prop["mycam"]["3"]["name"] = "MyCam-03";
	org_prop["mycam"]["4"]["name"] = "MyCam-04";
	org_prop["mycam"]["5"]["name"] = "MyCam-05";
	org_prop["mycam"]["6"]["name"] = "MyCam-06";
	org_prop["mycam"]["7"]["name"] = "MyCam-07";
	org_prop["mycam"]["8"]["name"] = "MyCam-08";

	org_prop["mycam"]["1"]["type"] = "MyCamType-01";
	org_prop["mycam"]["2"]["type"] = "MyCamType-02";
	org_prop["mycam"]["3"]["type"] = "MyCamType-03";
	org_prop["mycam"]["4"]["type"] = "MyCamType-04";
	org_prop["mycam"]["5"]["type"] = "MyCamType-05";
	org_prop["mycam"]["6"]["type"] = "MyCamType-06";
	org_prop["mycam"]["7"]["type"] = "MyCamType-07";
	org_prop["mycam"]["8"]["type"] = "MyCamType-08";

	org_prop["mycam"]["1"]["com"]["port"] = "MyCamType-01_port";
	org_prop["mycam"]["2"]["com"]["port"] = "MyCamType-02_port";
	org_prop["mycam"]["3"]["com"]["port"] = "MyCamType-03_port";
	org_prop["mycam"]["4"]["com"]["port"] = "MyCamType-04_port";
	org_prop["mycam"]["5"]["com"]["port"] = "MyCamType-05_port";
	org_prop["mycam"]["6"]["com"]["port"] = "MyCamType-06_port";
	org_prop["mycam"]["7"]["com"]["port"] = "MyCamType-07_port";
	org_prop["mycam"]["8"]["com"]["port"] = "MyCamType-08_port";

	org_prop["mycam"]["1"]["com"]["num"] = "MyCamType-01_port_num";
	org_prop["mycam"]["2"]["com"]["num"] = "MyCamType-02_port_num";
	org_prop["mycam"]["3"]["com"]["num"] = "MyCamType-03_port_num";
	org_prop["mycam"]["4"]["com"]["num"] = "MyCamType-04_port_num";
	org_prop["mycam"]["5"]["com"]["num"] = "MyCamType-05_port_num";
	org_prop["mycam"]["6"]["com"]["num"] = "MyCamType-06_port_num";
	org_prop["mycam"]["7"]["com"]["num"] = "MyCamType-07_port_num";
	org_prop["mycam"]["8"]["com"]["num"] = "MyCamType-08_port_num";

	org_prop["mycam"]["1"]["dev"]["num"] = "MyCamType-01_dev_num";
	org_prop["mycam"]["2"]["dev"]["num"] = "MyCamType-02_dev_num";
	org_prop["mycam"]["3"]["dev"]["num"] = "MyCamType-03_dev_num";
	org_prop["mycam"]["4"]["dev"]["num"] = "MyCamType-04_dev_num";
	org_prop["mycam"]["5"]["dev"]["num"] = "MyCamType-05_dev_num";
	org_prop["mycam"]["6"]["dev"]["num"] = "MyCamType-06_dev_num";
	org_prop["mycam"]["7"]["dev"]["num"] = "MyCamType-07_dev_num";
	org_prop["mycam"]["8"]["dev"]["num"] = "MyCamType-08_dev_num";

	org_prop["array"]["body"] = sub_array;
	org_prop["array"]["name"] = "test_array";

	// all
	org_prop.GetProperties("*", sub_prop);
	sub_prop.Save(OUTPUT_DIR "/all.xml");

	sub_prop = instek::Properties();
	org_prop.GetProperties("*:2,4,8:name,com", sub_prop);
	sub_prop.Save(OUTPUT_DIR "/all_2_4_8_name_com.xml");

	sub_prop = instek::Properties();
	org_prop.GetProperties("*:*:name,status,com", sub_prop);
	sub_prop.Save(OUTPUT_DIR "/all_name_status_com.xml");

	// Cam
	sub_prop = instek::Properties();
	org_prop.GetProperties("cam:1:name", sub_prop);
	sub_prop.Save(OUTPUT_DIR "/cam_1_name.xml");

	sub_prop = instek::Properties();
	org_prop.GetProperties("cam:3,5,8:status", sub_prop);
	sub_prop.Save(OUTPUT_DIR "/cam_3_5_8_status.xml");

	sub_prop = instek::Properties();
	org_prop.GetProperties("cam:*:name", sub_prop);
	sub_prop.Save(OUTPUT_DIR "/all_cam_name.xml");

	sub_prop = instek::Properties();
	org_prop.GetProperties("cam:1,4,5:*", sub_prop);
	sub_prop.Save(OUTPUT_DIR "/cam_1_4_5_name_status.xml");

	sub_prop = instek::Properties();
	org_prop.GetProperties("invalid1", sub_prop);
	sub_prop.Save(OUTPUT_DIR "/invalid1.xml");

	sub_prop = instek::Properties();
	org_prop.GetProperties("cam:invalid2", sub_prop);
	sub_prop.Save(OUTPUT_DIR "/invalid2.xml");

	sub_prop = instek::Properties();
	org_prop.GetProperties("cam:invalid3:name,invalid", sub_prop);
	sub_prop.Save(OUTPUT_DIR "/invalid3.xml");

	sub_prop = instek::Properties();
	org_prop.GetProperties("cam:invalid2,1,2", sub_prop);
	sub_prop.Save(OUTPUT_DIR "/cam_invalid_1_2.xml");

	// My Cam
	sub_prop = instek::Properties();
	org_prop.GetProperties("mycam", sub_prop);
	sub_prop.Save(OUTPUT_DIR "/all_mycam.xml");

	sub_prop = instek::Properties();
	org_prop.GetProperties("mycam:3,7", sub_prop);
	sub_prop.Save(OUTPUT_DIR "/mycam_3_7.xml");

	sub_prop = instek::Properties();
	org_prop.GetProperties("mycam:*:name,type,com,dev:port", sub_prop); // The dev will not be got because the next key is port.
	sub_prop.Save(OUTPUT_DIR "/mycam_name_type_com_dev_comport.xml");

	sub_prop = instek::Properties();
	org_prop.GetProperties("mycam:*:name,com,dev", sub_prop); // The dev will not be got because there is no next key.
	sub_prop.Save(OUTPUT_DIR "/mycam_name_com_dev.xml");

	sub_prop = instek::Properties();
	org_prop.GetProperties("mycam:*:name,type,com,dev:num", sub_prop);
	sub_prop.Save(OUTPUT_DIR "/mycam_name_type_com_dev_comnum_devnum.xml");

	sub_prop = instek::Properties();
	org_prop.GetProperties("mycam:*:com", sub_prop);
	sub_prop.Save(OUTPUT_DIR "/all_mycam_com.xml");

	sub_prop = instek::Properties();
	org_prop.GetProperties("mycam:3:dev:num", sub_prop);
	sub_prop.Save(OUTPUT_DIR "/mycam_3_dev_num.xml");

	sub_prop = instek::Properties();
	org_prop.GetProperties("mycam:*:invalid:dev", sub_prop);
	sub_prop.Save(OUTPUT_DIR "/mycam_invalid1.xml");

	// Merge multiple result
	sub_prop = instek::Properties();
	sub_prop2 = instek::Properties();
	org_prop.GetProperties("cam:*:name", sub_prop);
	org_prop.GetProperties("cam:*:status", sub_prop2);
	sub_prop.Merge(sub_prop2);
	sub_prop.Save(MERGE_OUTPUT_DIR "/all_cam_name_status.xml");

	sub_prop = instek::Properties();
	sub_prop2 = instek::Properties();
	org_prop.GetProperties("cam", sub_prop);
	org_prop.GetProperties("mycam", sub_prop2);
	sub_prop.Merge(sub_prop2);
	sub_prop.Save(MERGE_OUTPUT_DIR "/all.xml");

	sub_prop = instek::Properties();
	sub_prop2 = instek::Properties();
	org_prop.GetProperties("mycam:1,3,6,7:com:port", sub_prop);
	org_prop.GetProperties("mycam:1,2,8:com:num", sub_prop2);
	sub_prop.Merge(sub_prop2);
	sub_prop.Save(MERGE_OUTPUT_DIR "/mycam_1_comport_comnum_2_8_comnum_3_6_7_comport.xml");

	sub_prop = instek::Properties();
	sub_prop2 = instek::Properties();
	org_prop.GetProperties("mycam:1,3:com:port", sub_prop);
	sub_prop.Merge(sub_prop2); // If from properties is empty, it will do nothing.
	sub_prop.Save(MERGE_OUTPUT_DIR "/mycam_1_3_comport.xml");

	sub_prop = instek::Properties();
	sub_prop2 = instek::Properties();
	org_prop.GetProperties("mycam:1,2:com:num", sub_prop2);
	sub_prop.Merge(sub_prop2);
	sub_prop.Save(MERGE_OUTPUT_DIR "/mycam_1_2_comnum.xml");

	sub_prop = instek::Properties();
	sub_prop2 = instek::Properties();
	org_prop.GetProperties("mycam:1,2,4,5:name", sub_prop2);
	org_prop.GetProperties("cam:1,2,5:name", sub_prop);
	sub_prop.Merge(sub_prop2);
	org_prop.GetProperties("cam:1,3,5,7:name", sub_prop2);
	sub_prop.Merge(sub_prop2);
	org_prop.GetProperties("mycam:3,7:name", sub_prop2);
	sub_prop.Merge(sub_prop2);
	sub_prop.Save(MERGE_OUTPUT_DIR "/cam_1_2_3_5_7_name-mycam_1_2_3_4_5_7_name.xml");

	sub_prop = instek::Properties();
	sub_prop2 = instek::Properties();
	org_prop.GetProperties("mycam:1,2:name", sub_prop);
	org_prop.GetProperties("array", sub_prop2);
	sub_prop.Merge(sub_prop2);
	sub_prop.Save(MERGE_OUTPUT_DIR "/mycam_1_2_name_array.xml");

	return 0;
}

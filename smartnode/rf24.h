#ifndef _SENSOR_RF24_H
#define _SENSOR_RF24_H

enum SENSOR_STATUS {
	SENSOR_STATUS_OFF,
	SENSOR_STATUS_ON,
	SENSOR_STATUS_RESET
};

enum ELEMENT_TYPE {
	ELEMENT_TYPE_PIPE = 0x00,
	ELEMENT_TYPE_EVENT1,
	ELEMENT_TYPE_RET1,
	ELEMENT_TYPE_EVENT2,
	ELEMENT_TYPE_RET2,
	ELEMENT_TYPE_STATUS,
	ELEMENT_TYPE_ACTION,
	ELEMENT_TYPE_TIMESTAMP,
	ELEMENT_TYPE_MAX
};

enum SENSOR_TYPE {
	SENSOR_TYPE_UNKNOWN     = 0, // 2 << 0
	SENSOR_TYPE_FLAME       = 1, // 2 << 1
	SENSOR_TYPE_GAS         = 2, // 2 << 2
	SENSOR_TYPE_CO          = 3, // 2 << 3
	SENSOR_TYPE_WATER       = 4, // 2 << 4
	SENSOR_TYPE_NOISE       = 5, // 2 << 5
	SENSOR_TYPE_TEMPERATURE = 6, // 2 << 6
	SENSOR_TYPE_RFID        = 7, // 2 << 7
	SENSOR_TYPE_SHOCK       = 8, // 2 << 8
	SENSOR_TYPE_INTRUSION   = 9, // 2 << 9
	SENSOR_TYPE_MAGETIC     = 10,// 2 << 10
	SENSOR_TYPE_PHOTOCELL   = 11,// 2 << 11
	SENSOR_TYPE_REVERSE1 = 12,   // 2 << 12
	SENSOR_TYPE_REVERSE2 = 13,   // 2 << 13
	SENSOR_TYPE_REVERSE3 = 14,   // 2 << 14
	SENSOR_TYPE_MAX      = 15    // 2 << 15
};

enum ACTION_TYPE {
	ACTION_TYPE_DONTCARE	= 0x00,
	ACTION_TYPE_BEE			= 0x01,
	ACTION_TYPE_ALARM		= 0x02,
	ACTION_TYPE_MAIL		= 0x04,
	ACTION_TYPE_PHONE		= 0x08,
	ACTION_TYPE_PHOTO		= 0x10,
	ACTION_TYPE_RECORD		= 0x20,
	ACTION_TYPE_MAP			= 0x40
};

struct Payload {
	char nodeID[12];
	int event1_type;
	int return1_value;
	int event2_type;
	int return2_value;
	SENSOR_STATUS sensor_status;
	ACTION_TYPE	sensor_action;
	unsigned int time_stamp;
};

struct ActionReponse {
	ACTION_TYPE warning_type_;
	int warning_;
	ACTION_TYPE alarm_type_;
	int alarm_;
	ACTION_TYPE urgent_type_;
	int urgent_;
};

struct Sensor {
	int channel_;				// rf24 channel id
	int pipe_id_;				// pipe#
	int sensor_value_;			// sensor value
	unsigned int time_stamp_;	// time stamp
	SENSOR_TYPE sensor_type_;	// sensor type
	SENSOR_STATUS status_;		// sensor status
	ActionReponse action_reponse_;// event reponse
};

static const std::string SENSOR_NAME[]	 = {"UNKNOWN", "FLAME", "GAS", "CO", "WATER", "NOISE", "TEMPERATURE", "RFID","SHOCK", "INTRUSION", "MAGETIC", "PHOTOCELL"};
static const std::string STATUS_MACHINE[]= {"OFF", "ON", "RESET"};
static const std::string SENSOR_ACTION[] = {"DONTCARE", "BEE", "ALARM", "MAIL", "PHONE", "PHOTO", "RECORD", "MAP"};

#endif // _SENSOR_RF24_H
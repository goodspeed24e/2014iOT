#include "smartswitch.h"

int main(int argc, char** argv)
{
	if(argc != 3)
	{
		std::cout << "Usage: " << argv[0] << "<Input Port Number?!>" << std::endl;
		return -1;
	}

	std::cout << "Server Port: "	<< argv[1] << std::endl;
	std::cout << "RF Channel: "	<< argv[2] << std::endl;
	SmartSwitch ss(atoi(argv[1]), atoi(argv[2]));
	
	return 0;
}

//#include <iostream>
#include <fstream>
#include <string>

using namespace std;

int main()
{
	fstream outFile;
	string tempStr;

	outFile.open ("request_1.txt", fstream::out | fstream::trunc);

	if(outFile.is_open())
	{
		tempStr = "<?xml version=\"1.0\"?>\n";
		tempStr += "<methodCall>\n";
		tempStr += "\t<methodName>examples.getStateName</methodName>\n";
		tempStr += "\t<params>\n";
		tempStr += "\t\t<param>\n";
		tempStr += "\t\t\t<value><i4>41</i4></value>\n";
		tempStr += "\t\t</param>\n";
		tempStr += "\t</params>\n";
		tempStr += "</methodCall>\n";

		outFile << "POST /RPC2 HTTP/1.1\r\n"
			<< "User-Agent: Frontier/5.1.2 (WinNT)\r\n"
			<< "Host: betty.userland.com\r\n"
			<< "Content-Type: text/xml\r\n"
			<< "Content-length: "<<tempStr.length()<<"\r\n"
			<< "\r\n"
			<< tempStr;

		outFile.close();
		tempStr.clear();
	}

	outFile.open ("response_1.txt", fstream::out | fstream::trunc);
	if(outFile.is_open())
	{
		tempStr = "<?xml version=\"1.0\"?>\n";
		tempStr += "<methodResponse>\n";
		tempStr += "\t<params>\n";
		tempStr += "\t\t<param>\n";
		tempStr += "\t\t\t<value><string>South Dakota</string></value>\n";
		tempStr += "\t\t</param>\n";
		tempStr += "\t</params>\n";
		tempStr += "</methodResponse>\n";

		outFile << "HTTP/1.1 200 OK\r\n"
			<< "Connection: close\r\n"
			<< "Content-Length: "<<tempStr.length()<<"\r\n"
			<< "Content-Type: text/xml\r\n"
			<< "Date: Fri, 17 Jul 1998 19:55:08 GMT\r\n"
			<< "Server: UserLand Frontier/5.1.2-WinNT\r\n"
			<< "\r\n"
			<< tempStr;

		outFile.close();
		tempStr.clear();
	}

	return 0;
}

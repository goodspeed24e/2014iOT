#pragma once

#ifndef	__DISCOVERY__
#define	__DISCOVERY__

#include <iostream>
#include <string>
#include <map>
#include <vector>
#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <boost/function.hpp>
#include <boost/scoped_ptr.hpp>
#include <discovery/discovery_config.h>

#define	MAX_BUFFER			1024

//	ONLY ID_CC can send out beacon message
using namespace boost::asio::ip;
namespace FBi
{

enum DISCOVERY_EXPORTS_DECL Device_Type {
	ID_DVR = 0,
	ID_NCCTV,
	ID_OTHERS
};

enum DISCOVERY_EXPORTS_DECL Message_Type {
	ID_HELLO = 0,
	ID_DEVICE_INTRODUCE,
	ID_DEVICE_REMOVE
};

class DISCOVERY_EXPORTS_DECL Discovery
{
public:
	std::string  strID_Seperator;
	Discovery( enum Device_Type eType );

	~Discovery();
	void set_Receiving_DVR_CallbackFUN( const boost::function< void(const std::string&, const std::string&, bool ) >& FUN );
	void set_Receiving_NCCTV_CallbackFUN( const boost::function< void(const std::string&, bool ) >& FUN );

	void addLocalMonitorSetting( int iScreenID, unsigned int iPort );
	void Start();
	void Stop();
	void Beacon();

protected:
	void addDVRNode( char* ptrMSG, const std::string& ip_addr);
	void addNCCTVNode( char* ptrMSG );
	void removeDVRNode( char* ptrMSG, const std::string& ip_addr);
	void removeNCCTVNode( char* ptrMSG );

	bool parserNCCTV( char* ptrMSG, std::string& strUniqueID, int& iID, unsigned int& iPort );
private:
	
	std::map< int , unsigned int >	map_ScreenSetting_;
	char Receive_Buff[ MAX_BUFFER ];
	enum Device_Type DeviceType_;
	std::string strServiceType_, strIP_, strUnique_ID_;
	int iPort_;

	udp::endpoint Multicast_Endpoint_, Remote_Endpoint_;
	udp::socket Socket_;

	//	Call back functions
	boost::function< void(const std::string&, const std::string&, bool ) > FUN_RECEIVE_DVR_;
	boost::function< void(const std::string&, bool ) > FUN_RECEIVE_NCCTV_;

	void Sending_Message( enum Message_Type eType );
	void Receiving_Message();
	void Receiving_Message_Complete( const boost::system::error_code& error, size_t bytes_received );
};
}

#endif

#include <cstring>
#include <libcommon/service_engine.hpp>
#include <discovery/discovery.h>
#define	MAX_DEVICE_TYPE		3
#define	MAX_MESSAGE_TYPE	3

namespace FBi
{

static ServiceEngine discover_engine_;
static sb_allocator allocator_;

std::string strDevice_Type[ MAX_DEVICE_TYPE ] =
{	
	"DVR", "NCCTV", "OTHERS"
};

std::string strMessage_Type[ MAX_MESSAGE_TYPE ] =
{	
	"Hello FBi", "ADDED", "REMOVED"
};

Discovery::Discovery(enum Device_Type eType)
	:	DeviceType_( eType ),
		strIP_("239.255.0.1"),
		iPort_(30001),
		Multicast_Endpoint_( address::from_string(strIP_), (unsigned short )iPort_),
		Socket_(discover_engine_.GetIOService())
{
	strID_Seperator = "::";
	strServiceType_ = strDevice_Type[DeviceType_];

	//	Initial Socket
	udp::endpoint listen_endpoint( address::from_string( "0.0.0.0" ), ( unsigned short )iPort_ );
	Socket_.open( listen_endpoint.protocol() );
	Socket_.set_option( udp::socket::reuse_address( true ) );	//	Address reused.
	Socket_.bind( listen_endpoint );

	//	Joining Multicast group
	address multicast_address = address::from_string( strIP_.c_str() );
	Socket_.set_option( multicast::join_group( multicast_address ) );
//	Socket_.set_option( multicast::enable_loopback( false ) );	//	Do not receive packages sent by itself.

	Socket_.set_option( multicast::hops( 4 ) );

	//	Found unique ID for this machine
	strUnique_ID_ = host_name();
	std::cout << "[ Debug Message ] Discovery unique ID: " << strUnique_ID_ << " for " << strServiceType_ << std::endl;
}

Discovery::~Discovery()
{
	Stop();
}

void Discovery::Start()
{
	Receiving_Message();
	discover_engine_.StartService();
}

void Discovery::Stop()
{
	Sending_Message( ID_DEVICE_REMOVE );

	//  Leaving multicast group
	address multicast_address = address::from_string( strIP_.c_str() );
	Socket_.set_option( multicast::leave_group( multicast_address ) );

	discover_engine_.StopService();
}

void Discovery::Beacon()
{
	if ( ID_OTHERS == DeviceType_ )
		Sending_Message(ID_HELLO);
	else if ( ID_NCCTV )
		Sending_Message(ID_DEVICE_INTRODUCE);
}

/*
 */
void Discovery::Sending_Message(enum Message_Type eType)
{
	std::string strMessage;
	switch (eType)
	{
		case ID_DEVICE_INTRODUCE:
		{

			if (ID_NCCTV != DeviceType_)
			{
				strMessage = strServiceType_ + strID_Seperator + strMessage_Type[ eType ] + strID_Seperator + strUnique_ID_;
				Socket_.send_to(boost::asio::buffer( strMessage ), Multicast_Endpoint_);
				std::cout << "[ Debug Message ] Discovery Sent out message: " << strMessage << std::endl;
			}
			else 
			{
				std::map< int, unsigned int >::iterator itor;
				for ( itor = map_ScreenSetting_.begin(); itor != map_ScreenSetting_.end(); ++itor )
				{
					char ptrTEMP[ 16 ] = { 0 };
					sprintf( ptrTEMP, "%d%s%d", itor->first, strID_Seperator.c_str(), itor->second );
					strMessage = strServiceType_ + strID_Seperator + strMessage_Type[ eType ] + strID_Seperator + strUnique_ID_ + strID_Seperator + ptrTEMP;

					Socket_.send_to(boost::asio::buffer( strMessage ), Multicast_Endpoint_);
					std::cout << "[ Debug Message ] Discovery Sent out message: " << strMessage << std::endl;
				}
			}
			break;
		}
		case ID_DEVICE_REMOVE:
		{
			strMessage = strServiceType_ + strID_Seperator + strMessage_Type[ eType ] + strID_Seperator + strUnique_ID_;
			if ( ID_DVR == DeviceType_ )
				Socket_.send_to(boost::asio::buffer( strMessage ), Multicast_Endpoint_);
			else
			{
				for ( std::map< int, unsigned int >::iterator itor = map_ScreenSetting_.begin(); itor != map_ScreenSetting_.end(); ++itor )
				{
					char TMP[ 10 ] = { 0 };
					sprintf( TMP, "%d", itor->first );
					std::string strScreen = strMessage + strID_Seperator + TMP + strID_Seperator;
					sprintf( TMP, "%d", itor->second);
					strScreen += TMP;

					Socket_.send_to(boost::asio::buffer( strScreen ), Multicast_Endpoint_);
				}	
			}
			
			std::cout << "[ Debug Message ] Discovery Sent out message: " << strMessage << std::endl;
			break;
		}
		default:
		{
			strMessage = strMessage_Type[ eType ];	//	default is sending out HELLO message.
			Socket_.send_to(boost::asio::buffer( strMessage ), Multicast_Endpoint_);
			std::cout << "[ Debug Message ] Discovery Sent out message: " << strMessage << std::endl;
		}
	}
}

void Discovery::Receiving_Message()
{
	Socket_.async_receive_from(boost::asio::buffer(Receive_Buff, MAX_BUFFER), Remote_Endpoint_,
		make_custom_alloc_handler(allocator_,
		boost::bind( &Discovery::Receiving_Message_Complete, this,
		boost::asio::placeholders::error, boost::asio::placeholders::bytes_transferred)));
}

void Discovery::Receiving_Message_Complete( const boost::system::error_code& error, size_t bytes_received )
{
	if (!error)
	{

		Receive_Buff[bytes_received] = 0;
		
		//::cout << "message size: " << bytes_received << std::endl;
		//::cout << "Received message: " << Receive_Buff << std::endl;

		std::string ip_addr = Remote_Endpoint_.address().to_string();
		
		char *ptrSeperator = strstr( Receive_Buff, strID_Seperator.c_str() ), *ptrSeperatorNext = NULL;
		if ( NULL == ptrSeperator )
		{
			if ( 0 == strcmp( Receive_Buff, strMessage_Type[ ID_HELLO ].c_str() ) && ID_OTHERS != DeviceType_)
				Sending_Message( ID_DEVICE_INTRODUCE );
		}
		else if ( ID_OTHERS == DeviceType_ )
		{
			std::string device_type(Receive_Buff, ptrSeperator - Receive_Buff);
			
			ptrSeperator += strID_Seperator.length();
			if ( NULL != ( ptrSeperatorNext = strstr( ptrSeperator, strID_Seperator.c_str() ) ) )
			{
				std::string strMessageType = std::string( ptrSeperator, ptrSeperatorNext - ptrSeperator );
				if (strMessage_Type[ID_DEVICE_INTRODUCE] == strMessageType) 
				{
					if (strDevice_Type[ID_DVR] == device_type)
					{
						addDVRNode( ptrSeperatorNext + strID_Seperator.size(), ip_addr );
					}
					else if (strDevice_Type[ID_NCCTV] == device_type)
						addNCCTVNode( ptrSeperatorNext + strID_Seperator.size() );
				}
				else if (strMessage_Type[ID_DEVICE_REMOVE] == strMessageType)
				{
					if (strDevice_Type[ID_DVR] == device_type)
					{
						removeDVRNode( ptrSeperatorNext + strID_Seperator.size(), ip_addr );
					}
					else if (strDevice_Type[ID_NCCTV] == device_type)
						removeNCCTVNode( ptrSeperatorNext + strID_Seperator.size() );
				}
			}
		}
	}
	else
	{
		std::cout << "[ ERROR Message ] Discovery: Error message for receiveing: " << error.message() << std::endl;
	}

	Receiving_Message();
}

void Discovery::addLocalMonitorSetting( int iScreenID, unsigned int iPort )
{
	std::map< int, unsigned int >::iterator itor;

	if ( map_ScreenSetting_.end() == ( itor = map_ScreenSetting_.find( iScreenID ) ) )
		map_ScreenSetting_.insert( std::pair< int, unsigned int >( iScreenID, iPort ) );
	else
		itor->second = iPort;

	for ( itor = map_ScreenSetting_.begin(); itor != map_ScreenSetting_.end(); ++itor )
		std::cout << "Port for Screen " << itor->first << " is " << itor->second << std::endl;
}

void Discovery::addDVRNode( char* ptrMSG, const std::string& ip_addr )
{
	if ( !FUN_RECEIVE_DVR_.empty() )
		FUN_RECEIVE_DVR_( ptrMSG, ip_addr, true );
}

void Discovery::addNCCTVNode( char* ptrMSG )
{
	std::string strUnique;
	int iID;
	unsigned int iPort;

	if ( parserNCCTV( ptrMSG, strUnique, iID, iPort ) )
	{
		std::cout << "unique name: " << strUnique << " ID = " << iID << " port = " << iPort << std::endl;
	
		std::pair< std::string, int > key( strUnique, iID );
		std::map< std::pair< std::string, int >, unsigned int >::iterator itor;


		strUnique.append( "::" );
		char ptrTmp[ 10 ] = { 0 };
		sprintf( ptrTmp, "%d", iID );
		strUnique.append( ptrTmp );
		strUnique.append( "::" );
		sprintf( ptrTmp, "%d", iPort );
		strUnique.append( ptrTmp );

		//	!!!	Warnning !!!
		//	This function will return modified existed nCCTV inform as well, client need to recheck again.
		if ( !FUN_RECEIVE_NCCTV_.empty() )
			FUN_RECEIVE_NCCTV_( strUnique, true );
	}
}

void Discovery::removeDVRNode( char* ptrMSG, const std::string& ip_addr )
{
	if ( !FUN_RECEIVE_DVR_.empty() )
	{
		FUN_RECEIVE_DVR_( ptrMSG, ip_addr, false );
	}
}

void Discovery::removeNCCTVNode( char* ptrMSG )
{	
	std::string strUnique;
	int iID;
	unsigned int iPort;

	if ( parserNCCTV( ptrMSG, strUnique, iID, iPort ) )
	{
		std::cout << "unique name: " << strUnique << " ID = " << iID << " port = " << iPort << std::endl;

		std::stringstream ss;
		ss << strUnique << "::" << iID << "::" << iPort;

		if ( !FUN_RECEIVE_NCCTV_.empty() )
			FUN_RECEIVE_NCCTV_( ss.str(), false );
	}
}

bool Discovery::parserNCCTV( char* ptrMSG, std::string& strUniqueID, int& iID, unsigned int& iPort )
{
	char *ptrStart = ptrMSG, *ptrNextSeperator = NULL;

	if ( NULL == ( ptrNextSeperator = strstr( ptrMSG, strID_Seperator.c_str() ) ) )
		return false;

	strUniqueID = std::string( ptrStart, ptrNextSeperator - ptrStart );
	ptrStart = ptrNextSeperator + strID_Seperator.size();
	if ( NULL == ( ptrNextSeperator = strstr( ptrStart, strID_Seperator.c_str() ) ) )
		return false;
	
	memset( ptrNextSeperator, '\0', 1 );
	iID = atoi( ptrStart );

	ptrStart = ptrNextSeperator + strID_Seperator.size();
	iPort = atoi( ptrStart );
	return true;
}

void Discovery::set_Receiving_DVR_CallbackFUN(const boost::function<void(const std::string&, const std::string&, bool)>& FUN)
{
	FUN_RECEIVE_DVR_ = FUN;
}
		
void Discovery::set_Receiving_NCCTV_CallbackFUN(const boost::function< void(const std::string&, bool ) >& FUN)
{
	FUN_RECEIVE_NCCTV_ = FUN;
}

/*
void Discovery::printRemoteMonitor()
{
	std::map< std::pair< std::string, int >, unsigned int >::iterator itor = map_RemoteMonitor_.begin();
	for ( ; itor != map_RemoteMonitor_.end(); ++itor )
		std::cout << "Unique ID = " << itor->first.first << " with monitor ID = " << itor->first.second << " using XMLRPC Port " << itor->second << std::endl;
}
*/

}

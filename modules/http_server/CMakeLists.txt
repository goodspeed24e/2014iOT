SET(HTTP_SERVER_SRC_LIST http_connection.cpp http_msg.cpp http_msg_parser.cpp http_server.cpp not_implement_request_handler.cpp http_request_dispatcher.cpp uri.cpp notify_request_handler.cpp)

ADD_LIBRARY(http_server SHARED ${HTTP_SERVER_SRC_LIST})
#TARGET_LINK_LIBRARIES(http_server common transport)
TARGET_LINK_LIBRARIES(http_server boost_iostreams)

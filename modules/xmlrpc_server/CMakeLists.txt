SET(XMLRPC_SERVER_SRC_LIST xmlrpc_request_handler.cpp xmlrpc_server.cpp)

ADD_LIBRARY(xmlrpc_server SHARED ${XMLRPC_SERVER_SRC_LIST})
#TARGET_LINK_LIBRARIES(xmlrpc_server http_server)

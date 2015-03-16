CMAKE_MINIMUM_REQUIRED(VERSION 2.6)
PROJECT(smartswitch)

INCLUDE_DIRECTORIES( ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_BINARY_DIR} ${CMAKE_CURRENT_SOURCE_DIR}/../libs ${CMAKE_CURRENT_SOURCE_DIR}/../modules)
LINK_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR}/../buildtemp)

#SET(CMAKE_CXX_FLAGS "-O3 -Wall -Wextra -pedantic")
SET(CMAKE_CXX_FLAGS "-std=c++0x -Wall -Wextra -pedantic")

ADD_LIBRARY(smart_switch SHARED main.cpp sensor_trans.cpp smartswitch.cpp sensor_prototype.cpp xml_rpc_server.cpp)

SET(LIBRARY_OUTPUT_PATH ${CMAKE_CURRENT_SOURCE_DIR}/../buildtemp)

TARGET_LINK_LIBRARIES(smart_switch common boost_thread-mt ssl transport rf24 http_server xmlrpc_server discovery boost_system-mt notification_center boost_filesystem-mt)

### ncctv-launcher
ADD_EXECUTABLE(smartswitch main.cpp sensor_trans.cpp smartswitch.cpp sensor_prototype.cpp xml_rpc_server.cpp)
LINK_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR}/../buildtemp)
TARGET_LINK_LIBRARIES(smartswitch boost_thread-mt ssl transport smart_switch rf24 boost_system-mt)

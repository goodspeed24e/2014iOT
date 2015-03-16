CMAKE_MINIMUM_REQUIRED(VERSION 2.6)
PROJECT(smartnode)

INCLUDE_DIRECTORIES( ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_BINARY_DIR} ${CMAKE_CURRENT_SOURCE_DIR}/../libs ${CMAKE_CURRENT_SOURCE_DIR}/../modules)
LINK_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR}/../buildtemp)

#SET(CMAKE_CXX_FLAGS "-O3 -Wall -Wextra -pedantic")
SET(CMAKE_CXX_FLAGS "-std=c++0x -Wall -Wextra -pedantic")

ADD_LIBRARY(smart_node SHARED main.cpp smartnode.cpp httpSvr_out_layer.cpp edge_sensor.cpp rpc_remote_controller.cpp GPIOClass.cpp)

SET(LIBRARY_OUTPUT_PATH ${CMAKE_CURRENT_SOURCE_DIR}/../buildtemp)

TARGET_LINK_LIBRARIES(smart_node common boost_thread-mt ssl transport http_server xmlrpc_server discovery boost_system-mt notification_center boost_filesystem-mt)

### ncctv-launcher
ADD_EXECUTABLE(smartnode main.cpp smartnode.cpp rpc_remote_controller.cpp httpSvr_out_layer.cpp edge_sensor.cpp)
LINK_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR}/../buildtemp)
TARGET_LINK_LIBRARIES(smartnode boost_thread-mt ssl transport smart_node boost_system-mt)

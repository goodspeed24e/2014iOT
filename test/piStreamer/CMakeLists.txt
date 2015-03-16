CMAKE_MINIMUM_REQUIRED(VERSION 2.6)
PROJECT(ipcam)

INCLUDE_DIRECTORIES( ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_BINARY_DIR} ${SDKSTAGE}/opt/vc/include ${SDKSTAGE}/opt/vc/include/interface/vcos/pthreads ${SDKSTAGE}/opt/vc/include/interface/vmcs_host/linux ${CMAKE_CURRENT_SOURCE_DIR}/inc ${CMAKE_CURRENT_SOURCE_DIR}/src)

LINK_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR}/buildtemp ${PLATFORM_ROOT}/libs/ilclient ${PLATFORM_ROOT}/libs/vgfont /opt/vc/lib/)

#SET(CMAKE_CXX_FLAGS "-O3 -Wall -Wextra -pedantic")
SET(CMAKE_CXX_FLAGS "-std=c++0x -Wall -Wextra -pedantic")

ADD_LIBRARY(ip_cam SHARED main.c logger.c nal_to_rtp.c sx_mgmt_camera_hw.c sx_mgmt_rtp.c sx_mgmt_rtsp.c sx_mgmt_sys.c sx_mgmt_video.c sx_queue.c)

SET(LIBRARY_OUTPUT_PATH ${CMAKE_CURRENT_SOURCE_DIR}/buildtemp)

TARGET_LINK_LIBRARIES(ip_cam pthread rt bcm_host vcos openmaxil mmal bcm_host mmal_util mmal_core)

### ncctv-launcher
ADD_EXECUTABLE(ipcam main.c logger.c nal_to_rtp.c sx_mgmt_camera_hw.c sx_mgmt_rtp.c sx_mgmt_rtsp.c sx_mgmt_sys.c sx_mgmt_video.c sx_queue.c)
LINK_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR}/buildtemp)
TARGET_LINK_LIBRARIES(ipcam pthread rt bcm_host vcos openmaxil mmal bcm_host mmal_util mmal_core)

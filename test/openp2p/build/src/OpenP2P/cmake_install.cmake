# Install script for directory: /home/pi/206/workspace/test/openp2p/src/OpenP2P

# Set the install prefix
IF(NOT DEFINED CMAKE_INSTALL_PREFIX)
  SET(CMAKE_INSTALL_PREFIX "/usr/local")
ENDIF(NOT DEFINED CMAKE_INSTALL_PREFIX)
STRING(REGEX REPLACE "/$" "" CMAKE_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}")

# Set the install configuration name.
IF(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)
  IF(BUILD_TYPE)
    STRING(REGEX REPLACE "^[^A-Za-z0-9_]+" ""
           CMAKE_INSTALL_CONFIG_NAME "${BUILD_TYPE}")
  ELSE(BUILD_TYPE)
    SET(CMAKE_INSTALL_CONFIG_NAME "Debug")
  ENDIF(BUILD_TYPE)
  MESSAGE(STATUS "Install configuration: \"${CMAKE_INSTALL_CONFIG_NAME}\"")
ENDIF(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)

# Set the component getting installed.
IF(NOT CMAKE_INSTALL_COMPONENT)
  IF(COMPONENT)
    MESSAGE(STATUS "Install component: \"${COMPONENT}\"")
    SET(CMAKE_INSTALL_COMPONENT "${COMPONENT}")
  ELSE(COMPONENT)
    SET(CMAKE_INSTALL_COMPONENT)
  ENDIF(COMPONENT)
ENDIF(NOT CMAKE_INSTALL_COMPONENT)

# Install shared libraries without execute permission?
IF(NOT DEFINED CMAKE_INSTALL_SO_NO_EXE)
  SET(CMAKE_INSTALL_SO_NO_EXE "1")
ENDIF(NOT DEFINED CMAKE_INSTALL_SO_NO_EXE)

IF(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for each subdirectory.
  INCLUDE("/home/pi/206/workspace/test/openp2p/build/src/OpenP2P/Buffer/cmake_install.cmake")
  INCLUDE("/home/pi/206/workspace/test/openp2p/build/src/OpenP2P/Crypt/cmake_install.cmake")
  INCLUDE("/home/pi/206/workspace/test/openp2p/build/src/OpenP2P/Event/cmake_install.cmake")
  INCLUDE("/home/pi/206/workspace/test/openp2p/build/src/OpenP2P/FolderSync/cmake_install.cmake")
  INCLUDE("/home/pi/206/workspace/test/openp2p/build/src/OpenP2P/IP/cmake_install.cmake")
  INCLUDE("/home/pi/206/workspace/test/openp2p/build/src/OpenP2P/OFTorrent/cmake_install.cmake")
  INCLUDE("/home/pi/206/workspace/test/openp2p/build/src/OpenP2P/RootNetwork/cmake_install.cmake")
  INCLUDE("/home/pi/206/workspace/test/openp2p/build/src/OpenP2P/Stream/cmake_install.cmake")
  INCLUDE("/home/pi/206/workspace/test/openp2p/build/src/OpenP2P/TCP/cmake_install.cmake")
  INCLUDE("/home/pi/206/workspace/test/openp2p/build/src/OpenP2P/UDP/cmake_install.cmake")

ENDIF(NOT CMAKE_INSTALL_LOCAL_ONLY)


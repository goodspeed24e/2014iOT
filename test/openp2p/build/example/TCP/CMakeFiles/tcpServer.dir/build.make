# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 2.8

#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:

# Remove some rules from gmake that .SUFFIXES does not remove.
SUFFIXES =

.SUFFIXES: .hpux_make_needs_suffix_list

# Suppress display of executed commands.
$(VERBOSE).SILENT:

# A target that is always out of date.
cmake_force:
.PHONY : cmake_force

#=============================================================================
# Set environment variables for the build.

# The shell in which to execute make rules.
SHELL = /bin/sh

# The CMake executable.
CMAKE_COMMAND = /usr/bin/cmake

# The command to remove a file.
RM = /usr/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/pi/206/workspace/test/openp2p

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/pi/206/workspace/test/openp2p/build

# Include any dependencies generated for this target.
include example/TCP/CMakeFiles/tcpServer.dir/depend.make

# Include the progress variables for this target.
include example/TCP/CMakeFiles/tcpServer.dir/progress.make

# Include the compile flags for this target's objects.
include example/TCP/CMakeFiles/tcpServer.dir/flags.make

example/TCP/CMakeFiles/tcpServer.dir/server.cpp.o: example/TCP/CMakeFiles/tcpServer.dir/flags.make
example/TCP/CMakeFiles/tcpServer.dir/server.cpp.o: ../example/TCP/server.cpp
	$(CMAKE_COMMAND) -E cmake_progress_report /home/pi/206/workspace/test/openp2p/build/CMakeFiles $(CMAKE_PROGRESS_1)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building CXX object example/TCP/CMakeFiles/tcpServer.dir/server.cpp.o"
	cd /home/pi/206/workspace/test/openp2p/build/example/TCP && /usr/bin/c++   $(CXX_DEFINES) $(CXX_FLAGS) -o CMakeFiles/tcpServer.dir/server.cpp.o -c /home/pi/206/workspace/test/openp2p/example/TCP/server.cpp

example/TCP/CMakeFiles/tcpServer.dir/server.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/tcpServer.dir/server.cpp.i"
	cd /home/pi/206/workspace/test/openp2p/build/example/TCP && /usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -E /home/pi/206/workspace/test/openp2p/example/TCP/server.cpp > CMakeFiles/tcpServer.dir/server.cpp.i

example/TCP/CMakeFiles/tcpServer.dir/server.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/tcpServer.dir/server.cpp.s"
	cd /home/pi/206/workspace/test/openp2p/build/example/TCP && /usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -S /home/pi/206/workspace/test/openp2p/example/TCP/server.cpp -o CMakeFiles/tcpServer.dir/server.cpp.s

example/TCP/CMakeFiles/tcpServer.dir/server.cpp.o.requires:
.PHONY : example/TCP/CMakeFiles/tcpServer.dir/server.cpp.o.requires

example/TCP/CMakeFiles/tcpServer.dir/server.cpp.o.provides: example/TCP/CMakeFiles/tcpServer.dir/server.cpp.o.requires
	$(MAKE) -f example/TCP/CMakeFiles/tcpServer.dir/build.make example/TCP/CMakeFiles/tcpServer.dir/server.cpp.o.provides.build
.PHONY : example/TCP/CMakeFiles/tcpServer.dir/server.cpp.o.provides

example/TCP/CMakeFiles/tcpServer.dir/server.cpp.o.provides.build: example/TCP/CMakeFiles/tcpServer.dir/server.cpp.o

# Object files for target tcpServer
tcpServer_OBJECTS = \
"CMakeFiles/tcpServer.dir/server.cpp.o"

# External object files for target tcpServer
tcpServer_EXTERNAL_OBJECTS =

example/TCP/tcpServer: example/TCP/CMakeFiles/tcpServer.dir/server.cpp.o
example/TCP/tcpServer: example/TCP/CMakeFiles/tcpServer.dir/build.make
example/TCP/tcpServer: src/OpenP2P/TCP/libopenp2p-tcp.a
example/TCP/tcpServer: src/OpenP2P/IP/libopenp2p-ip.a
example/TCP/tcpServer: src/OpenP2P/Stream/libopenp2p-stream.a
example/TCP/tcpServer: src/OpenP2P/Event/libopenp2p-event.a
example/TCP/tcpServer: src/OpenP2P/libopenp2p.a
example/TCP/tcpServer: example/TCP/CMakeFiles/tcpServer.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --red --bold "Linking CXX executable tcpServer"
	cd /home/pi/206/workspace/test/openp2p/build/example/TCP && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/tcpServer.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
example/TCP/CMakeFiles/tcpServer.dir/build: example/TCP/tcpServer
.PHONY : example/TCP/CMakeFiles/tcpServer.dir/build

example/TCP/CMakeFiles/tcpServer.dir/requires: example/TCP/CMakeFiles/tcpServer.dir/server.cpp.o.requires
.PHONY : example/TCP/CMakeFiles/tcpServer.dir/requires

example/TCP/CMakeFiles/tcpServer.dir/clean:
	cd /home/pi/206/workspace/test/openp2p/build/example/TCP && $(CMAKE_COMMAND) -P CMakeFiles/tcpServer.dir/cmake_clean.cmake
.PHONY : example/TCP/CMakeFiles/tcpServer.dir/clean

example/TCP/CMakeFiles/tcpServer.dir/depend:
	cd /home/pi/206/workspace/test/openp2p/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/pi/206/workspace/test/openp2p /home/pi/206/workspace/test/openp2p/example/TCP /home/pi/206/workspace/test/openp2p/build /home/pi/206/workspace/test/openp2p/build/example/TCP /home/pi/206/workspace/test/openp2p/build/example/TCP/CMakeFiles/tcpServer.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : example/TCP/CMakeFiles/tcpServer.dir/depend

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
include src/OpenP2P/IP/CMakeFiles/openp2p-ip.dir/depend.make

# Include the progress variables for this target.
include src/OpenP2P/IP/CMakeFiles/openp2p-ip.dir/progress.make

# Include the compile flags for this target's objects.
include src/OpenP2P/IP/CMakeFiles/openp2p-ip.dir/flags.make

src/OpenP2P/IP/CMakeFiles/openp2p-ip.dir/Address.cpp.o: src/OpenP2P/IP/CMakeFiles/openp2p-ip.dir/flags.make
src/OpenP2P/IP/CMakeFiles/openp2p-ip.dir/Address.cpp.o: ../src/OpenP2P/IP/Address.cpp
	$(CMAKE_COMMAND) -E cmake_progress_report /home/pi/206/workspace/test/openp2p/build/CMakeFiles $(CMAKE_PROGRESS_1)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building CXX object src/OpenP2P/IP/CMakeFiles/openp2p-ip.dir/Address.cpp.o"
	cd /home/pi/206/workspace/test/openp2p/build/src/OpenP2P/IP && /usr/bin/c++   $(CXX_DEFINES) $(CXX_FLAGS) -o CMakeFiles/openp2p-ip.dir/Address.cpp.o -c /home/pi/206/workspace/test/openp2p/src/OpenP2P/IP/Address.cpp

src/OpenP2P/IP/CMakeFiles/openp2p-ip.dir/Address.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/openp2p-ip.dir/Address.cpp.i"
	cd /home/pi/206/workspace/test/openp2p/build/src/OpenP2P/IP && /usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -E /home/pi/206/workspace/test/openp2p/src/OpenP2P/IP/Address.cpp > CMakeFiles/openp2p-ip.dir/Address.cpp.i

src/OpenP2P/IP/CMakeFiles/openp2p-ip.dir/Address.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/openp2p-ip.dir/Address.cpp.s"
	cd /home/pi/206/workspace/test/openp2p/build/src/OpenP2P/IP && /usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -S /home/pi/206/workspace/test/openp2p/src/OpenP2P/IP/Address.cpp -o CMakeFiles/openp2p-ip.dir/Address.cpp.s

src/OpenP2P/IP/CMakeFiles/openp2p-ip.dir/Address.cpp.o.requires:
.PHONY : src/OpenP2P/IP/CMakeFiles/openp2p-ip.dir/Address.cpp.o.requires

src/OpenP2P/IP/CMakeFiles/openp2p-ip.dir/Address.cpp.o.provides: src/OpenP2P/IP/CMakeFiles/openp2p-ip.dir/Address.cpp.o.requires
	$(MAKE) -f src/OpenP2P/IP/CMakeFiles/openp2p-ip.dir/build.make src/OpenP2P/IP/CMakeFiles/openp2p-ip.dir/Address.cpp.o.provides.build
.PHONY : src/OpenP2P/IP/CMakeFiles/openp2p-ip.dir/Address.cpp.o.provides

src/OpenP2P/IP/CMakeFiles/openp2p-ip.dir/Address.cpp.o.provides.build: src/OpenP2P/IP/CMakeFiles/openp2p-ip.dir/Address.cpp.o

src/OpenP2P/IP/CMakeFiles/openp2p-ip.dir/V4Address.cpp.o: src/OpenP2P/IP/CMakeFiles/openp2p-ip.dir/flags.make
src/OpenP2P/IP/CMakeFiles/openp2p-ip.dir/V4Address.cpp.o: ../src/OpenP2P/IP/V4Address.cpp
	$(CMAKE_COMMAND) -E cmake_progress_report /home/pi/206/workspace/test/openp2p/build/CMakeFiles $(CMAKE_PROGRESS_2)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building CXX object src/OpenP2P/IP/CMakeFiles/openp2p-ip.dir/V4Address.cpp.o"
	cd /home/pi/206/workspace/test/openp2p/build/src/OpenP2P/IP && /usr/bin/c++   $(CXX_DEFINES) $(CXX_FLAGS) -o CMakeFiles/openp2p-ip.dir/V4Address.cpp.o -c /home/pi/206/workspace/test/openp2p/src/OpenP2P/IP/V4Address.cpp

src/OpenP2P/IP/CMakeFiles/openp2p-ip.dir/V4Address.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/openp2p-ip.dir/V4Address.cpp.i"
	cd /home/pi/206/workspace/test/openp2p/build/src/OpenP2P/IP && /usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -E /home/pi/206/workspace/test/openp2p/src/OpenP2P/IP/V4Address.cpp > CMakeFiles/openp2p-ip.dir/V4Address.cpp.i

src/OpenP2P/IP/CMakeFiles/openp2p-ip.dir/V4Address.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/openp2p-ip.dir/V4Address.cpp.s"
	cd /home/pi/206/workspace/test/openp2p/build/src/OpenP2P/IP && /usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -S /home/pi/206/workspace/test/openp2p/src/OpenP2P/IP/V4Address.cpp -o CMakeFiles/openp2p-ip.dir/V4Address.cpp.s

src/OpenP2P/IP/CMakeFiles/openp2p-ip.dir/V4Address.cpp.o.requires:
.PHONY : src/OpenP2P/IP/CMakeFiles/openp2p-ip.dir/V4Address.cpp.o.requires

src/OpenP2P/IP/CMakeFiles/openp2p-ip.dir/V4Address.cpp.o.provides: src/OpenP2P/IP/CMakeFiles/openp2p-ip.dir/V4Address.cpp.o.requires
	$(MAKE) -f src/OpenP2P/IP/CMakeFiles/openp2p-ip.dir/build.make src/OpenP2P/IP/CMakeFiles/openp2p-ip.dir/V4Address.cpp.o.provides.build
.PHONY : src/OpenP2P/IP/CMakeFiles/openp2p-ip.dir/V4Address.cpp.o.provides

src/OpenP2P/IP/CMakeFiles/openp2p-ip.dir/V4Address.cpp.o.provides.build: src/OpenP2P/IP/CMakeFiles/openp2p-ip.dir/V4Address.cpp.o

src/OpenP2P/IP/CMakeFiles/openp2p-ip.dir/V6Address.cpp.o: src/OpenP2P/IP/CMakeFiles/openp2p-ip.dir/flags.make
src/OpenP2P/IP/CMakeFiles/openp2p-ip.dir/V6Address.cpp.o: ../src/OpenP2P/IP/V6Address.cpp
	$(CMAKE_COMMAND) -E cmake_progress_report /home/pi/206/workspace/test/openp2p/build/CMakeFiles $(CMAKE_PROGRESS_3)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building CXX object src/OpenP2P/IP/CMakeFiles/openp2p-ip.dir/V6Address.cpp.o"
	cd /home/pi/206/workspace/test/openp2p/build/src/OpenP2P/IP && /usr/bin/c++   $(CXX_DEFINES) $(CXX_FLAGS) -o CMakeFiles/openp2p-ip.dir/V6Address.cpp.o -c /home/pi/206/workspace/test/openp2p/src/OpenP2P/IP/V6Address.cpp

src/OpenP2P/IP/CMakeFiles/openp2p-ip.dir/V6Address.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/openp2p-ip.dir/V6Address.cpp.i"
	cd /home/pi/206/workspace/test/openp2p/build/src/OpenP2P/IP && /usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -E /home/pi/206/workspace/test/openp2p/src/OpenP2P/IP/V6Address.cpp > CMakeFiles/openp2p-ip.dir/V6Address.cpp.i

src/OpenP2P/IP/CMakeFiles/openp2p-ip.dir/V6Address.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/openp2p-ip.dir/V6Address.cpp.s"
	cd /home/pi/206/workspace/test/openp2p/build/src/OpenP2P/IP && /usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -S /home/pi/206/workspace/test/openp2p/src/OpenP2P/IP/V6Address.cpp -o CMakeFiles/openp2p-ip.dir/V6Address.cpp.s

src/OpenP2P/IP/CMakeFiles/openp2p-ip.dir/V6Address.cpp.o.requires:
.PHONY : src/OpenP2P/IP/CMakeFiles/openp2p-ip.dir/V6Address.cpp.o.requires

src/OpenP2P/IP/CMakeFiles/openp2p-ip.dir/V6Address.cpp.o.provides: src/OpenP2P/IP/CMakeFiles/openp2p-ip.dir/V6Address.cpp.o.requires
	$(MAKE) -f src/OpenP2P/IP/CMakeFiles/openp2p-ip.dir/build.make src/OpenP2P/IP/CMakeFiles/openp2p-ip.dir/V6Address.cpp.o.provides.build
.PHONY : src/OpenP2P/IP/CMakeFiles/openp2p-ip.dir/V6Address.cpp.o.provides

src/OpenP2P/IP/CMakeFiles/openp2p-ip.dir/V6Address.cpp.o.provides.build: src/OpenP2P/IP/CMakeFiles/openp2p-ip.dir/V6Address.cpp.o

# Object files for target openp2p-ip
openp2p__ip_OBJECTS = \
"CMakeFiles/openp2p-ip.dir/Address.cpp.o" \
"CMakeFiles/openp2p-ip.dir/V4Address.cpp.o" \
"CMakeFiles/openp2p-ip.dir/V6Address.cpp.o"

# External object files for target openp2p-ip
openp2p__ip_EXTERNAL_OBJECTS =

src/OpenP2P/IP/libopenp2p-ip.a: src/OpenP2P/IP/CMakeFiles/openp2p-ip.dir/Address.cpp.o
src/OpenP2P/IP/libopenp2p-ip.a: src/OpenP2P/IP/CMakeFiles/openp2p-ip.dir/V4Address.cpp.o
src/OpenP2P/IP/libopenp2p-ip.a: src/OpenP2P/IP/CMakeFiles/openp2p-ip.dir/V6Address.cpp.o
src/OpenP2P/IP/libopenp2p-ip.a: src/OpenP2P/IP/CMakeFiles/openp2p-ip.dir/build.make
src/OpenP2P/IP/libopenp2p-ip.a: src/OpenP2P/IP/CMakeFiles/openp2p-ip.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --red --bold "Linking CXX static library libopenp2p-ip.a"
	cd /home/pi/206/workspace/test/openp2p/build/src/OpenP2P/IP && $(CMAKE_COMMAND) -P CMakeFiles/openp2p-ip.dir/cmake_clean_target.cmake
	cd /home/pi/206/workspace/test/openp2p/build/src/OpenP2P/IP && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/openp2p-ip.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
src/OpenP2P/IP/CMakeFiles/openp2p-ip.dir/build: src/OpenP2P/IP/libopenp2p-ip.a
.PHONY : src/OpenP2P/IP/CMakeFiles/openp2p-ip.dir/build

src/OpenP2P/IP/CMakeFiles/openp2p-ip.dir/requires: src/OpenP2P/IP/CMakeFiles/openp2p-ip.dir/Address.cpp.o.requires
src/OpenP2P/IP/CMakeFiles/openp2p-ip.dir/requires: src/OpenP2P/IP/CMakeFiles/openp2p-ip.dir/V4Address.cpp.o.requires
src/OpenP2P/IP/CMakeFiles/openp2p-ip.dir/requires: src/OpenP2P/IP/CMakeFiles/openp2p-ip.dir/V6Address.cpp.o.requires
.PHONY : src/OpenP2P/IP/CMakeFiles/openp2p-ip.dir/requires

src/OpenP2P/IP/CMakeFiles/openp2p-ip.dir/clean:
	cd /home/pi/206/workspace/test/openp2p/build/src/OpenP2P/IP && $(CMAKE_COMMAND) -P CMakeFiles/openp2p-ip.dir/cmake_clean.cmake
.PHONY : src/OpenP2P/IP/CMakeFiles/openp2p-ip.dir/clean

src/OpenP2P/IP/CMakeFiles/openp2p-ip.dir/depend:
	cd /home/pi/206/workspace/test/openp2p/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/pi/206/workspace/test/openp2p /home/pi/206/workspace/test/openp2p/src/OpenP2P/IP /home/pi/206/workspace/test/openp2p/build /home/pi/206/workspace/test/openp2p/build/src/OpenP2P/IP /home/pi/206/workspace/test/openp2p/build/src/OpenP2P/IP/CMakeFiles/openp2p-ip.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : src/OpenP2P/IP/CMakeFiles/openp2p-ip.dir/depend

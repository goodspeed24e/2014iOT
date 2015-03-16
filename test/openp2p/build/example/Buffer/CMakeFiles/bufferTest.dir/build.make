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
include example/Buffer/CMakeFiles/bufferTest.dir/depend.make

# Include the progress variables for this target.
include example/Buffer/CMakeFiles/bufferTest.dir/progress.make

# Include the compile flags for this target's objects.
include example/Buffer/CMakeFiles/bufferTest.dir/flags.make

example/Buffer/CMakeFiles/bufferTest.dir/main.cpp.o: example/Buffer/CMakeFiles/bufferTest.dir/flags.make
example/Buffer/CMakeFiles/bufferTest.dir/main.cpp.o: ../example/Buffer/main.cpp
	$(CMAKE_COMMAND) -E cmake_progress_report /home/pi/206/workspace/test/openp2p/build/CMakeFiles $(CMAKE_PROGRESS_1)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building CXX object example/Buffer/CMakeFiles/bufferTest.dir/main.cpp.o"
	cd /home/pi/206/workspace/test/openp2p/build/example/Buffer && /usr/bin/c++   $(CXX_DEFINES) $(CXX_FLAGS) -o CMakeFiles/bufferTest.dir/main.cpp.o -c /home/pi/206/workspace/test/openp2p/example/Buffer/main.cpp

example/Buffer/CMakeFiles/bufferTest.dir/main.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/bufferTest.dir/main.cpp.i"
	cd /home/pi/206/workspace/test/openp2p/build/example/Buffer && /usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -E /home/pi/206/workspace/test/openp2p/example/Buffer/main.cpp > CMakeFiles/bufferTest.dir/main.cpp.i

example/Buffer/CMakeFiles/bufferTest.dir/main.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/bufferTest.dir/main.cpp.s"
	cd /home/pi/206/workspace/test/openp2p/build/example/Buffer && /usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -S /home/pi/206/workspace/test/openp2p/example/Buffer/main.cpp -o CMakeFiles/bufferTest.dir/main.cpp.s

example/Buffer/CMakeFiles/bufferTest.dir/main.cpp.o.requires:
.PHONY : example/Buffer/CMakeFiles/bufferTest.dir/main.cpp.o.requires

example/Buffer/CMakeFiles/bufferTest.dir/main.cpp.o.provides: example/Buffer/CMakeFiles/bufferTest.dir/main.cpp.o.requires
	$(MAKE) -f example/Buffer/CMakeFiles/bufferTest.dir/build.make example/Buffer/CMakeFiles/bufferTest.dir/main.cpp.o.provides.build
.PHONY : example/Buffer/CMakeFiles/bufferTest.dir/main.cpp.o.provides

example/Buffer/CMakeFiles/bufferTest.dir/main.cpp.o.provides.build: example/Buffer/CMakeFiles/bufferTest.dir/main.cpp.o

# Object files for target bufferTest
bufferTest_OBJECTS = \
"CMakeFiles/bufferTest.dir/main.cpp.o"

# External object files for target bufferTest
bufferTest_EXTERNAL_OBJECTS =

example/Buffer/bufferTest: example/Buffer/CMakeFiles/bufferTest.dir/main.cpp.o
example/Buffer/bufferTest: example/Buffer/CMakeFiles/bufferTest.dir/build.make
example/Buffer/bufferTest: src/OpenP2P/Buffer/libopenp2p-buffer.a
example/Buffer/bufferTest: src/OpenP2P/Stream/libopenp2p-stream.a
example/Buffer/bufferTest: src/OpenP2P/Event/libopenp2p-event.a
example/Buffer/bufferTest: src/OpenP2P/libopenp2p.a
example/Buffer/bufferTest: example/Buffer/CMakeFiles/bufferTest.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --red --bold "Linking CXX executable bufferTest"
	cd /home/pi/206/workspace/test/openp2p/build/example/Buffer && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/bufferTest.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
example/Buffer/CMakeFiles/bufferTest.dir/build: example/Buffer/bufferTest
.PHONY : example/Buffer/CMakeFiles/bufferTest.dir/build

example/Buffer/CMakeFiles/bufferTest.dir/requires: example/Buffer/CMakeFiles/bufferTest.dir/main.cpp.o.requires
.PHONY : example/Buffer/CMakeFiles/bufferTest.dir/requires

example/Buffer/CMakeFiles/bufferTest.dir/clean:
	cd /home/pi/206/workspace/test/openp2p/build/example/Buffer && $(CMAKE_COMMAND) -P CMakeFiles/bufferTest.dir/cmake_clean.cmake
.PHONY : example/Buffer/CMakeFiles/bufferTest.dir/clean

example/Buffer/CMakeFiles/bufferTest.dir/depend:
	cd /home/pi/206/workspace/test/openp2p/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/pi/206/workspace/test/openp2p /home/pi/206/workspace/test/openp2p/example/Buffer /home/pi/206/workspace/test/openp2p/build /home/pi/206/workspace/test/openp2p/build/example/Buffer /home/pi/206/workspace/test/openp2p/build/example/Buffer/CMakeFiles/bufferTest.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : example/Buffer/CMakeFiles/bufferTest.dir/depend

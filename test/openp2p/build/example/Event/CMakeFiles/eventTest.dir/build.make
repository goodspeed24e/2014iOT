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
include example/Event/CMakeFiles/eventTest.dir/depend.make

# Include the progress variables for this target.
include example/Event/CMakeFiles/eventTest.dir/progress.make

# Include the compile flags for this target's objects.
include example/Event/CMakeFiles/eventTest.dir/flags.make

example/Event/CMakeFiles/eventTest.dir/main.cpp.o: example/Event/CMakeFiles/eventTest.dir/flags.make
example/Event/CMakeFiles/eventTest.dir/main.cpp.o: ../example/Event/main.cpp
	$(CMAKE_COMMAND) -E cmake_progress_report /home/pi/206/workspace/test/openp2p/build/CMakeFiles $(CMAKE_PROGRESS_1)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building CXX object example/Event/CMakeFiles/eventTest.dir/main.cpp.o"
	cd /home/pi/206/workspace/test/openp2p/build/example/Event && /usr/bin/c++   $(CXX_DEFINES) $(CXX_FLAGS) -o CMakeFiles/eventTest.dir/main.cpp.o -c /home/pi/206/workspace/test/openp2p/example/Event/main.cpp

example/Event/CMakeFiles/eventTest.dir/main.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/eventTest.dir/main.cpp.i"
	cd /home/pi/206/workspace/test/openp2p/build/example/Event && /usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -E /home/pi/206/workspace/test/openp2p/example/Event/main.cpp > CMakeFiles/eventTest.dir/main.cpp.i

example/Event/CMakeFiles/eventTest.dir/main.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/eventTest.dir/main.cpp.s"
	cd /home/pi/206/workspace/test/openp2p/build/example/Event && /usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -S /home/pi/206/workspace/test/openp2p/example/Event/main.cpp -o CMakeFiles/eventTest.dir/main.cpp.s

example/Event/CMakeFiles/eventTest.dir/main.cpp.o.requires:
.PHONY : example/Event/CMakeFiles/eventTest.dir/main.cpp.o.requires

example/Event/CMakeFiles/eventTest.dir/main.cpp.o.provides: example/Event/CMakeFiles/eventTest.dir/main.cpp.o.requires
	$(MAKE) -f example/Event/CMakeFiles/eventTest.dir/build.make example/Event/CMakeFiles/eventTest.dir/main.cpp.o.provides.build
.PHONY : example/Event/CMakeFiles/eventTest.dir/main.cpp.o.provides

example/Event/CMakeFiles/eventTest.dir/main.cpp.o.provides.build: example/Event/CMakeFiles/eventTest.dir/main.cpp.o

# Object files for target eventTest
eventTest_OBJECTS = \
"CMakeFiles/eventTest.dir/main.cpp.o"

# External object files for target eventTest
eventTest_EXTERNAL_OBJECTS =

example/Event/eventTest: example/Event/CMakeFiles/eventTest.dir/main.cpp.o
example/Event/eventTest: example/Event/CMakeFiles/eventTest.dir/build.make
example/Event/eventTest: src/OpenP2P/Event/libopenp2p-event.a
example/Event/eventTest: src/OpenP2P/libopenp2p.a
example/Event/eventTest: example/Event/CMakeFiles/eventTest.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --red --bold "Linking CXX executable eventTest"
	cd /home/pi/206/workspace/test/openp2p/build/example/Event && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/eventTest.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
example/Event/CMakeFiles/eventTest.dir/build: example/Event/eventTest
.PHONY : example/Event/CMakeFiles/eventTest.dir/build

example/Event/CMakeFiles/eventTest.dir/requires: example/Event/CMakeFiles/eventTest.dir/main.cpp.o.requires
.PHONY : example/Event/CMakeFiles/eventTest.dir/requires

example/Event/CMakeFiles/eventTest.dir/clean:
	cd /home/pi/206/workspace/test/openp2p/build/example/Event && $(CMAKE_COMMAND) -P CMakeFiles/eventTest.dir/cmake_clean.cmake
.PHONY : example/Event/CMakeFiles/eventTest.dir/clean

example/Event/CMakeFiles/eventTest.dir/depend:
	cd /home/pi/206/workspace/test/openp2p/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/pi/206/workspace/test/openp2p /home/pi/206/workspace/test/openp2p/example/Event /home/pi/206/workspace/test/openp2p/build /home/pi/206/workspace/test/openp2p/build/example/Event /home/pi/206/workspace/test/openp2p/build/example/Event/CMakeFiles/eventTest.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : example/Event/CMakeFiles/eventTest.dir/depend


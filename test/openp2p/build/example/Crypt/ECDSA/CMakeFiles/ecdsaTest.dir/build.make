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
include example/Crypt/ECDSA/CMakeFiles/ecdsaTest.dir/depend.make

# Include the progress variables for this target.
include example/Crypt/ECDSA/CMakeFiles/ecdsaTest.dir/progress.make

# Include the compile flags for this target's objects.
include example/Crypt/ECDSA/CMakeFiles/ecdsaTest.dir/flags.make

example/Crypt/ECDSA/CMakeFiles/ecdsaTest.dir/main.cpp.o: example/Crypt/ECDSA/CMakeFiles/ecdsaTest.dir/flags.make
example/Crypt/ECDSA/CMakeFiles/ecdsaTest.dir/main.cpp.o: ../example/Crypt/ECDSA/main.cpp
	$(CMAKE_COMMAND) -E cmake_progress_report /home/pi/206/workspace/test/openp2p/build/CMakeFiles $(CMAKE_PROGRESS_1)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building CXX object example/Crypt/ECDSA/CMakeFiles/ecdsaTest.dir/main.cpp.o"
	cd /home/pi/206/workspace/test/openp2p/build/example/Crypt/ECDSA && /usr/bin/c++   $(CXX_DEFINES) $(CXX_FLAGS) -o CMakeFiles/ecdsaTest.dir/main.cpp.o -c /home/pi/206/workspace/test/openp2p/example/Crypt/ECDSA/main.cpp

example/Crypt/ECDSA/CMakeFiles/ecdsaTest.dir/main.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/ecdsaTest.dir/main.cpp.i"
	cd /home/pi/206/workspace/test/openp2p/build/example/Crypt/ECDSA && /usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -E /home/pi/206/workspace/test/openp2p/example/Crypt/ECDSA/main.cpp > CMakeFiles/ecdsaTest.dir/main.cpp.i

example/Crypt/ECDSA/CMakeFiles/ecdsaTest.dir/main.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/ecdsaTest.dir/main.cpp.s"
	cd /home/pi/206/workspace/test/openp2p/build/example/Crypt/ECDSA && /usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -S /home/pi/206/workspace/test/openp2p/example/Crypt/ECDSA/main.cpp -o CMakeFiles/ecdsaTest.dir/main.cpp.s

example/Crypt/ECDSA/CMakeFiles/ecdsaTest.dir/main.cpp.o.requires:
.PHONY : example/Crypt/ECDSA/CMakeFiles/ecdsaTest.dir/main.cpp.o.requires

example/Crypt/ECDSA/CMakeFiles/ecdsaTest.dir/main.cpp.o.provides: example/Crypt/ECDSA/CMakeFiles/ecdsaTest.dir/main.cpp.o.requires
	$(MAKE) -f example/Crypt/ECDSA/CMakeFiles/ecdsaTest.dir/build.make example/Crypt/ECDSA/CMakeFiles/ecdsaTest.dir/main.cpp.o.provides.build
.PHONY : example/Crypt/ECDSA/CMakeFiles/ecdsaTest.dir/main.cpp.o.provides

example/Crypt/ECDSA/CMakeFiles/ecdsaTest.dir/main.cpp.o.provides.build: example/Crypt/ECDSA/CMakeFiles/ecdsaTest.dir/main.cpp.o

# Object files for target ecdsaTest
ecdsaTest_OBJECTS = \
"CMakeFiles/ecdsaTest.dir/main.cpp.o"

# External object files for target ecdsaTest
ecdsaTest_EXTERNAL_OBJECTS =

example/Crypt/ECDSA/ecdsaTest: example/Crypt/ECDSA/CMakeFiles/ecdsaTest.dir/main.cpp.o
example/Crypt/ECDSA/ecdsaTest: example/Crypt/ECDSA/CMakeFiles/ecdsaTest.dir/build.make
example/Crypt/ECDSA/ecdsaTest: src/OpenP2P/Crypt/libopenp2p-crypt.a
example/Crypt/ECDSA/ecdsaTest: src/OpenP2P/Stream/libopenp2p-stream.a
example/Crypt/ECDSA/ecdsaTest: src/OpenP2P/Event/libopenp2p-event.a
example/Crypt/ECDSA/ecdsaTest: src/OpenP2P/libopenp2p.a
example/Crypt/ECDSA/ecdsaTest: example/Crypt/ECDSA/CMakeFiles/ecdsaTest.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --red --bold "Linking CXX executable ecdsaTest"
	cd /home/pi/206/workspace/test/openp2p/build/example/Crypt/ECDSA && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/ecdsaTest.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
example/Crypt/ECDSA/CMakeFiles/ecdsaTest.dir/build: example/Crypt/ECDSA/ecdsaTest
.PHONY : example/Crypt/ECDSA/CMakeFiles/ecdsaTest.dir/build

example/Crypt/ECDSA/CMakeFiles/ecdsaTest.dir/requires: example/Crypt/ECDSA/CMakeFiles/ecdsaTest.dir/main.cpp.o.requires
.PHONY : example/Crypt/ECDSA/CMakeFiles/ecdsaTest.dir/requires

example/Crypt/ECDSA/CMakeFiles/ecdsaTest.dir/clean:
	cd /home/pi/206/workspace/test/openp2p/build/example/Crypt/ECDSA && $(CMAKE_COMMAND) -P CMakeFiles/ecdsaTest.dir/cmake_clean.cmake
.PHONY : example/Crypt/ECDSA/CMakeFiles/ecdsaTest.dir/clean

example/Crypt/ECDSA/CMakeFiles/ecdsaTest.dir/depend:
	cd /home/pi/206/workspace/test/openp2p/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/pi/206/workspace/test/openp2p /home/pi/206/workspace/test/openp2p/example/Crypt/ECDSA /home/pi/206/workspace/test/openp2p/build /home/pi/206/workspace/test/openp2p/build/example/Crypt/ECDSA /home/pi/206/workspace/test/openp2p/build/example/Crypt/ECDSA/CMakeFiles/ecdsaTest.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : example/Crypt/ECDSA/CMakeFiles/ecdsaTest.dir/depend


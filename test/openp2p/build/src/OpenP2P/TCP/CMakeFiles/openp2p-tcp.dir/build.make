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
include src/OpenP2P/TCP/CMakeFiles/openp2p-tcp.dir/depend.make

# Include the progress variables for this target.
include src/OpenP2P/TCP/CMakeFiles/openp2p-tcp.dir/progress.make

# Include the compile flags for this target's objects.
include src/OpenP2P/TCP/CMakeFiles/openp2p-tcp.dir/flags.make

src/OpenP2P/TCP/CMakeFiles/openp2p-tcp.dir/Acceptor.cpp.o: src/OpenP2P/TCP/CMakeFiles/openp2p-tcp.dir/flags.make
src/OpenP2P/TCP/CMakeFiles/openp2p-tcp.dir/Acceptor.cpp.o: ../src/OpenP2P/TCP/Acceptor.cpp
	$(CMAKE_COMMAND) -E cmake_progress_report /home/pi/206/workspace/test/openp2p/build/CMakeFiles $(CMAKE_PROGRESS_1)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building CXX object src/OpenP2P/TCP/CMakeFiles/openp2p-tcp.dir/Acceptor.cpp.o"
	cd /home/pi/206/workspace/test/openp2p/build/src/OpenP2P/TCP && /usr/bin/c++   $(CXX_DEFINES) $(CXX_FLAGS) -o CMakeFiles/openp2p-tcp.dir/Acceptor.cpp.o -c /home/pi/206/workspace/test/openp2p/src/OpenP2P/TCP/Acceptor.cpp

src/OpenP2P/TCP/CMakeFiles/openp2p-tcp.dir/Acceptor.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/openp2p-tcp.dir/Acceptor.cpp.i"
	cd /home/pi/206/workspace/test/openp2p/build/src/OpenP2P/TCP && /usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -E /home/pi/206/workspace/test/openp2p/src/OpenP2P/TCP/Acceptor.cpp > CMakeFiles/openp2p-tcp.dir/Acceptor.cpp.i

src/OpenP2P/TCP/CMakeFiles/openp2p-tcp.dir/Acceptor.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/openp2p-tcp.dir/Acceptor.cpp.s"
	cd /home/pi/206/workspace/test/openp2p/build/src/OpenP2P/TCP && /usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -S /home/pi/206/workspace/test/openp2p/src/OpenP2P/TCP/Acceptor.cpp -o CMakeFiles/openp2p-tcp.dir/Acceptor.cpp.s

src/OpenP2P/TCP/CMakeFiles/openp2p-tcp.dir/Acceptor.cpp.o.requires:
.PHONY : src/OpenP2P/TCP/CMakeFiles/openp2p-tcp.dir/Acceptor.cpp.o.requires

src/OpenP2P/TCP/CMakeFiles/openp2p-tcp.dir/Acceptor.cpp.o.provides: src/OpenP2P/TCP/CMakeFiles/openp2p-tcp.dir/Acceptor.cpp.o.requires
	$(MAKE) -f src/OpenP2P/TCP/CMakeFiles/openp2p-tcp.dir/build.make src/OpenP2P/TCP/CMakeFiles/openp2p-tcp.dir/Acceptor.cpp.o.provides.build
.PHONY : src/OpenP2P/TCP/CMakeFiles/openp2p-tcp.dir/Acceptor.cpp.o.provides

src/OpenP2P/TCP/CMakeFiles/openp2p-tcp.dir/Acceptor.cpp.o.provides.build: src/OpenP2P/TCP/CMakeFiles/openp2p-tcp.dir/Acceptor.cpp.o

src/OpenP2P/TCP/CMakeFiles/openp2p-tcp.dir/Resolver.cpp.o: src/OpenP2P/TCP/CMakeFiles/openp2p-tcp.dir/flags.make
src/OpenP2P/TCP/CMakeFiles/openp2p-tcp.dir/Resolver.cpp.o: ../src/OpenP2P/TCP/Resolver.cpp
	$(CMAKE_COMMAND) -E cmake_progress_report /home/pi/206/workspace/test/openp2p/build/CMakeFiles $(CMAKE_PROGRESS_2)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building CXX object src/OpenP2P/TCP/CMakeFiles/openp2p-tcp.dir/Resolver.cpp.o"
	cd /home/pi/206/workspace/test/openp2p/build/src/OpenP2P/TCP && /usr/bin/c++   $(CXX_DEFINES) $(CXX_FLAGS) -o CMakeFiles/openp2p-tcp.dir/Resolver.cpp.o -c /home/pi/206/workspace/test/openp2p/src/OpenP2P/TCP/Resolver.cpp

src/OpenP2P/TCP/CMakeFiles/openp2p-tcp.dir/Resolver.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/openp2p-tcp.dir/Resolver.cpp.i"
	cd /home/pi/206/workspace/test/openp2p/build/src/OpenP2P/TCP && /usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -E /home/pi/206/workspace/test/openp2p/src/OpenP2P/TCP/Resolver.cpp > CMakeFiles/openp2p-tcp.dir/Resolver.cpp.i

src/OpenP2P/TCP/CMakeFiles/openp2p-tcp.dir/Resolver.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/openp2p-tcp.dir/Resolver.cpp.s"
	cd /home/pi/206/workspace/test/openp2p/build/src/OpenP2P/TCP && /usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -S /home/pi/206/workspace/test/openp2p/src/OpenP2P/TCP/Resolver.cpp -o CMakeFiles/openp2p-tcp.dir/Resolver.cpp.s

src/OpenP2P/TCP/CMakeFiles/openp2p-tcp.dir/Resolver.cpp.o.requires:
.PHONY : src/OpenP2P/TCP/CMakeFiles/openp2p-tcp.dir/Resolver.cpp.o.requires

src/OpenP2P/TCP/CMakeFiles/openp2p-tcp.dir/Resolver.cpp.o.provides: src/OpenP2P/TCP/CMakeFiles/openp2p-tcp.dir/Resolver.cpp.o.requires
	$(MAKE) -f src/OpenP2P/TCP/CMakeFiles/openp2p-tcp.dir/build.make src/OpenP2P/TCP/CMakeFiles/openp2p-tcp.dir/Resolver.cpp.o.provides.build
.PHONY : src/OpenP2P/TCP/CMakeFiles/openp2p-tcp.dir/Resolver.cpp.o.provides

src/OpenP2P/TCP/CMakeFiles/openp2p-tcp.dir/Resolver.cpp.o.provides.build: src/OpenP2P/TCP/CMakeFiles/openp2p-tcp.dir/Resolver.cpp.o

src/OpenP2P/TCP/CMakeFiles/openp2p-tcp.dir/Stream.cpp.o: src/OpenP2P/TCP/CMakeFiles/openp2p-tcp.dir/flags.make
src/OpenP2P/TCP/CMakeFiles/openp2p-tcp.dir/Stream.cpp.o: ../src/OpenP2P/TCP/Stream.cpp
	$(CMAKE_COMMAND) -E cmake_progress_report /home/pi/206/workspace/test/openp2p/build/CMakeFiles $(CMAKE_PROGRESS_3)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building CXX object src/OpenP2P/TCP/CMakeFiles/openp2p-tcp.dir/Stream.cpp.o"
	cd /home/pi/206/workspace/test/openp2p/build/src/OpenP2P/TCP && /usr/bin/c++   $(CXX_DEFINES) $(CXX_FLAGS) -o CMakeFiles/openp2p-tcp.dir/Stream.cpp.o -c /home/pi/206/workspace/test/openp2p/src/OpenP2P/TCP/Stream.cpp

src/OpenP2P/TCP/CMakeFiles/openp2p-tcp.dir/Stream.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/openp2p-tcp.dir/Stream.cpp.i"
	cd /home/pi/206/workspace/test/openp2p/build/src/OpenP2P/TCP && /usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -E /home/pi/206/workspace/test/openp2p/src/OpenP2P/TCP/Stream.cpp > CMakeFiles/openp2p-tcp.dir/Stream.cpp.i

src/OpenP2P/TCP/CMakeFiles/openp2p-tcp.dir/Stream.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/openp2p-tcp.dir/Stream.cpp.s"
	cd /home/pi/206/workspace/test/openp2p/build/src/OpenP2P/TCP && /usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -S /home/pi/206/workspace/test/openp2p/src/OpenP2P/TCP/Stream.cpp -o CMakeFiles/openp2p-tcp.dir/Stream.cpp.s

src/OpenP2P/TCP/CMakeFiles/openp2p-tcp.dir/Stream.cpp.o.requires:
.PHONY : src/OpenP2P/TCP/CMakeFiles/openp2p-tcp.dir/Stream.cpp.o.requires

src/OpenP2P/TCP/CMakeFiles/openp2p-tcp.dir/Stream.cpp.o.provides: src/OpenP2P/TCP/CMakeFiles/openp2p-tcp.dir/Stream.cpp.o.requires
	$(MAKE) -f src/OpenP2P/TCP/CMakeFiles/openp2p-tcp.dir/build.make src/OpenP2P/TCP/CMakeFiles/openp2p-tcp.dir/Stream.cpp.o.provides.build
.PHONY : src/OpenP2P/TCP/CMakeFiles/openp2p-tcp.dir/Stream.cpp.o.provides

src/OpenP2P/TCP/CMakeFiles/openp2p-tcp.dir/Stream.cpp.o.provides.build: src/OpenP2P/TCP/CMakeFiles/openp2p-tcp.dir/Stream.cpp.o

# Object files for target openp2p-tcp
openp2p__tcp_OBJECTS = \
"CMakeFiles/openp2p-tcp.dir/Acceptor.cpp.o" \
"CMakeFiles/openp2p-tcp.dir/Resolver.cpp.o" \
"CMakeFiles/openp2p-tcp.dir/Stream.cpp.o"

# External object files for target openp2p-tcp
openp2p__tcp_EXTERNAL_OBJECTS =

src/OpenP2P/TCP/libopenp2p-tcp.a: src/OpenP2P/TCP/CMakeFiles/openp2p-tcp.dir/Acceptor.cpp.o
src/OpenP2P/TCP/libopenp2p-tcp.a: src/OpenP2P/TCP/CMakeFiles/openp2p-tcp.dir/Resolver.cpp.o
src/OpenP2P/TCP/libopenp2p-tcp.a: src/OpenP2P/TCP/CMakeFiles/openp2p-tcp.dir/Stream.cpp.o
src/OpenP2P/TCP/libopenp2p-tcp.a: src/OpenP2P/TCP/CMakeFiles/openp2p-tcp.dir/build.make
src/OpenP2P/TCP/libopenp2p-tcp.a: src/OpenP2P/TCP/CMakeFiles/openp2p-tcp.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --red --bold "Linking CXX static library libopenp2p-tcp.a"
	cd /home/pi/206/workspace/test/openp2p/build/src/OpenP2P/TCP && $(CMAKE_COMMAND) -P CMakeFiles/openp2p-tcp.dir/cmake_clean_target.cmake
	cd /home/pi/206/workspace/test/openp2p/build/src/OpenP2P/TCP && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/openp2p-tcp.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
src/OpenP2P/TCP/CMakeFiles/openp2p-tcp.dir/build: src/OpenP2P/TCP/libopenp2p-tcp.a
.PHONY : src/OpenP2P/TCP/CMakeFiles/openp2p-tcp.dir/build

src/OpenP2P/TCP/CMakeFiles/openp2p-tcp.dir/requires: src/OpenP2P/TCP/CMakeFiles/openp2p-tcp.dir/Acceptor.cpp.o.requires
src/OpenP2P/TCP/CMakeFiles/openp2p-tcp.dir/requires: src/OpenP2P/TCP/CMakeFiles/openp2p-tcp.dir/Resolver.cpp.o.requires
src/OpenP2P/TCP/CMakeFiles/openp2p-tcp.dir/requires: src/OpenP2P/TCP/CMakeFiles/openp2p-tcp.dir/Stream.cpp.o.requires
.PHONY : src/OpenP2P/TCP/CMakeFiles/openp2p-tcp.dir/requires

src/OpenP2P/TCP/CMakeFiles/openp2p-tcp.dir/clean:
	cd /home/pi/206/workspace/test/openp2p/build/src/OpenP2P/TCP && $(CMAKE_COMMAND) -P CMakeFiles/openp2p-tcp.dir/cmake_clean.cmake
.PHONY : src/OpenP2P/TCP/CMakeFiles/openp2p-tcp.dir/clean

src/OpenP2P/TCP/CMakeFiles/openp2p-tcp.dir/depend:
	cd /home/pi/206/workspace/test/openp2p/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/pi/206/workspace/test/openp2p /home/pi/206/workspace/test/openp2p/src/OpenP2P/TCP /home/pi/206/workspace/test/openp2p/build /home/pi/206/workspace/test/openp2p/build/src/OpenP2P/TCP /home/pi/206/workspace/test/openp2p/build/src/OpenP2P/TCP/CMakeFiles/openp2p-tcp.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : src/OpenP2P/TCP/CMakeFiles/openp2p-tcp.dir/depend


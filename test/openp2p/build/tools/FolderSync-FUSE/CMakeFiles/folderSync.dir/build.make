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
include tools/FolderSync-FUSE/CMakeFiles/folderSync.dir/depend.make

# Include the progress variables for this target.
include tools/FolderSync-FUSE/CMakeFiles/folderSync.dir/progress.make

# Include the compile flags for this target's objects.
include tools/FolderSync-FUSE/CMakeFiles/folderSync.dir/flags.make

tools/FolderSync-FUSE/CMakeFiles/folderSync.dir/FileSystemWrapper.cpp.o: tools/FolderSync-FUSE/CMakeFiles/folderSync.dir/flags.make
tools/FolderSync-FUSE/CMakeFiles/folderSync.dir/FileSystemWrapper.cpp.o: ../tools/FolderSync-FUSE/FileSystemWrapper.cpp
	$(CMAKE_COMMAND) -E cmake_progress_report /home/pi/206/workspace/test/openp2p/build/CMakeFiles $(CMAKE_PROGRESS_1)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building CXX object tools/FolderSync-FUSE/CMakeFiles/folderSync.dir/FileSystemWrapper.cpp.o"
	cd /home/pi/206/workspace/test/openp2p/build/tools/FolderSync-FUSE && /usr/bin/c++   $(CXX_DEFINES) $(CXX_FLAGS) -o CMakeFiles/folderSync.dir/FileSystemWrapper.cpp.o -c /home/pi/206/workspace/test/openp2p/tools/FolderSync-FUSE/FileSystemWrapper.cpp

tools/FolderSync-FUSE/CMakeFiles/folderSync.dir/FileSystemWrapper.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/folderSync.dir/FileSystemWrapper.cpp.i"
	cd /home/pi/206/workspace/test/openp2p/build/tools/FolderSync-FUSE && /usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -E /home/pi/206/workspace/test/openp2p/tools/FolderSync-FUSE/FileSystemWrapper.cpp > CMakeFiles/folderSync.dir/FileSystemWrapper.cpp.i

tools/FolderSync-FUSE/CMakeFiles/folderSync.dir/FileSystemWrapper.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/folderSync.dir/FileSystemWrapper.cpp.s"
	cd /home/pi/206/workspace/test/openp2p/build/tools/FolderSync-FUSE && /usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -S /home/pi/206/workspace/test/openp2p/tools/FolderSync-FUSE/FileSystemWrapper.cpp -o CMakeFiles/folderSync.dir/FileSystemWrapper.cpp.s

tools/FolderSync-FUSE/CMakeFiles/folderSync.dir/FileSystemWrapper.cpp.o.requires:
.PHONY : tools/FolderSync-FUSE/CMakeFiles/folderSync.dir/FileSystemWrapper.cpp.o.requires

tools/FolderSync-FUSE/CMakeFiles/folderSync.dir/FileSystemWrapper.cpp.o.provides: tools/FolderSync-FUSE/CMakeFiles/folderSync.dir/FileSystemWrapper.cpp.o.requires
	$(MAKE) -f tools/FolderSync-FUSE/CMakeFiles/folderSync.dir/build.make tools/FolderSync-FUSE/CMakeFiles/folderSync.dir/FileSystemWrapper.cpp.o.provides.build
.PHONY : tools/FolderSync-FUSE/CMakeFiles/folderSync.dir/FileSystemWrapper.cpp.o.provides

tools/FolderSync-FUSE/CMakeFiles/folderSync.dir/FileSystemWrapper.cpp.o.provides.build: tools/FolderSync-FUSE/CMakeFiles/folderSync.dir/FileSystemWrapper.cpp.o

tools/FolderSync-FUSE/CMakeFiles/folderSync.dir/HandleRef.cpp.o: tools/FolderSync-FUSE/CMakeFiles/folderSync.dir/flags.make
tools/FolderSync-FUSE/CMakeFiles/folderSync.dir/HandleRef.cpp.o: ../tools/FolderSync-FUSE/HandleRef.cpp
	$(CMAKE_COMMAND) -E cmake_progress_report /home/pi/206/workspace/test/openp2p/build/CMakeFiles $(CMAKE_PROGRESS_2)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building CXX object tools/FolderSync-FUSE/CMakeFiles/folderSync.dir/HandleRef.cpp.o"
	cd /home/pi/206/workspace/test/openp2p/build/tools/FolderSync-FUSE && /usr/bin/c++   $(CXX_DEFINES) $(CXX_FLAGS) -o CMakeFiles/folderSync.dir/HandleRef.cpp.o -c /home/pi/206/workspace/test/openp2p/tools/FolderSync-FUSE/HandleRef.cpp

tools/FolderSync-FUSE/CMakeFiles/folderSync.dir/HandleRef.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/folderSync.dir/HandleRef.cpp.i"
	cd /home/pi/206/workspace/test/openp2p/build/tools/FolderSync-FUSE && /usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -E /home/pi/206/workspace/test/openp2p/tools/FolderSync-FUSE/HandleRef.cpp > CMakeFiles/folderSync.dir/HandleRef.cpp.i

tools/FolderSync-FUSE/CMakeFiles/folderSync.dir/HandleRef.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/folderSync.dir/HandleRef.cpp.s"
	cd /home/pi/206/workspace/test/openp2p/build/tools/FolderSync-FUSE && /usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -S /home/pi/206/workspace/test/openp2p/tools/FolderSync-FUSE/HandleRef.cpp -o CMakeFiles/folderSync.dir/HandleRef.cpp.s

tools/FolderSync-FUSE/CMakeFiles/folderSync.dir/HandleRef.cpp.o.requires:
.PHONY : tools/FolderSync-FUSE/CMakeFiles/folderSync.dir/HandleRef.cpp.o.requires

tools/FolderSync-FUSE/CMakeFiles/folderSync.dir/HandleRef.cpp.o.provides: tools/FolderSync-FUSE/CMakeFiles/folderSync.dir/HandleRef.cpp.o.requires
	$(MAKE) -f tools/FolderSync-FUSE/CMakeFiles/folderSync.dir/build.make tools/FolderSync-FUSE/CMakeFiles/folderSync.dir/HandleRef.cpp.o.provides.build
.PHONY : tools/FolderSync-FUSE/CMakeFiles/folderSync.dir/HandleRef.cpp.o.provides

tools/FolderSync-FUSE/CMakeFiles/folderSync.dir/HandleRef.cpp.o.provides.build: tools/FolderSync-FUSE/CMakeFiles/folderSync.dir/HandleRef.cpp.o

tools/FolderSync-FUSE/CMakeFiles/folderSync.dir/NodeSystem.cpp.o: tools/FolderSync-FUSE/CMakeFiles/folderSync.dir/flags.make
tools/FolderSync-FUSE/CMakeFiles/folderSync.dir/NodeSystem.cpp.o: ../tools/FolderSync-FUSE/NodeSystem.cpp
	$(CMAKE_COMMAND) -E cmake_progress_report /home/pi/206/workspace/test/openp2p/build/CMakeFiles $(CMAKE_PROGRESS_3)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building CXX object tools/FolderSync-FUSE/CMakeFiles/folderSync.dir/NodeSystem.cpp.o"
	cd /home/pi/206/workspace/test/openp2p/build/tools/FolderSync-FUSE && /usr/bin/c++   $(CXX_DEFINES) $(CXX_FLAGS) -o CMakeFiles/folderSync.dir/NodeSystem.cpp.o -c /home/pi/206/workspace/test/openp2p/tools/FolderSync-FUSE/NodeSystem.cpp

tools/FolderSync-FUSE/CMakeFiles/folderSync.dir/NodeSystem.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/folderSync.dir/NodeSystem.cpp.i"
	cd /home/pi/206/workspace/test/openp2p/build/tools/FolderSync-FUSE && /usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -E /home/pi/206/workspace/test/openp2p/tools/FolderSync-FUSE/NodeSystem.cpp > CMakeFiles/folderSync.dir/NodeSystem.cpp.i

tools/FolderSync-FUSE/CMakeFiles/folderSync.dir/NodeSystem.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/folderSync.dir/NodeSystem.cpp.s"
	cd /home/pi/206/workspace/test/openp2p/build/tools/FolderSync-FUSE && /usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -S /home/pi/206/workspace/test/openp2p/tools/FolderSync-FUSE/NodeSystem.cpp -o CMakeFiles/folderSync.dir/NodeSystem.cpp.s

tools/FolderSync-FUSE/CMakeFiles/folderSync.dir/NodeSystem.cpp.o.requires:
.PHONY : tools/FolderSync-FUSE/CMakeFiles/folderSync.dir/NodeSystem.cpp.o.requires

tools/FolderSync-FUSE/CMakeFiles/folderSync.dir/NodeSystem.cpp.o.provides: tools/FolderSync-FUSE/CMakeFiles/folderSync.dir/NodeSystem.cpp.o.requires
	$(MAKE) -f tools/FolderSync-FUSE/CMakeFiles/folderSync.dir/build.make tools/FolderSync-FUSE/CMakeFiles/folderSync.dir/NodeSystem.cpp.o.provides.build
.PHONY : tools/FolderSync-FUSE/CMakeFiles/folderSync.dir/NodeSystem.cpp.o.provides

tools/FolderSync-FUSE/CMakeFiles/folderSync.dir/NodeSystem.cpp.o.provides.build: tools/FolderSync-FUSE/CMakeFiles/folderSync.dir/NodeSystem.cpp.o

tools/FolderSync-FUSE/CMakeFiles/folderSync.dir/main.cpp.o: tools/FolderSync-FUSE/CMakeFiles/folderSync.dir/flags.make
tools/FolderSync-FUSE/CMakeFiles/folderSync.dir/main.cpp.o: ../tools/FolderSync-FUSE/main.cpp
	$(CMAKE_COMMAND) -E cmake_progress_report /home/pi/206/workspace/test/openp2p/build/CMakeFiles $(CMAKE_PROGRESS_4)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building CXX object tools/FolderSync-FUSE/CMakeFiles/folderSync.dir/main.cpp.o"
	cd /home/pi/206/workspace/test/openp2p/build/tools/FolderSync-FUSE && /usr/bin/c++   $(CXX_DEFINES) $(CXX_FLAGS) -o CMakeFiles/folderSync.dir/main.cpp.o -c /home/pi/206/workspace/test/openp2p/tools/FolderSync-FUSE/main.cpp

tools/FolderSync-FUSE/CMakeFiles/folderSync.dir/main.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/folderSync.dir/main.cpp.i"
	cd /home/pi/206/workspace/test/openp2p/build/tools/FolderSync-FUSE && /usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -E /home/pi/206/workspace/test/openp2p/tools/FolderSync-FUSE/main.cpp > CMakeFiles/folderSync.dir/main.cpp.i

tools/FolderSync-FUSE/CMakeFiles/folderSync.dir/main.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/folderSync.dir/main.cpp.s"
	cd /home/pi/206/workspace/test/openp2p/build/tools/FolderSync-FUSE && /usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -S /home/pi/206/workspace/test/openp2p/tools/FolderSync-FUSE/main.cpp -o CMakeFiles/folderSync.dir/main.cpp.s

tools/FolderSync-FUSE/CMakeFiles/folderSync.dir/main.cpp.o.requires:
.PHONY : tools/FolderSync-FUSE/CMakeFiles/folderSync.dir/main.cpp.o.requires

tools/FolderSync-FUSE/CMakeFiles/folderSync.dir/main.cpp.o.provides: tools/FolderSync-FUSE/CMakeFiles/folderSync.dir/main.cpp.o.requires
	$(MAKE) -f tools/FolderSync-FUSE/CMakeFiles/folderSync.dir/build.make tools/FolderSync-FUSE/CMakeFiles/folderSync.dir/main.cpp.o.provides.build
.PHONY : tools/FolderSync-FUSE/CMakeFiles/folderSync.dir/main.cpp.o.provides

tools/FolderSync-FUSE/CMakeFiles/folderSync.dir/main.cpp.o.provides.build: tools/FolderSync-FUSE/CMakeFiles/folderSync.dir/main.cpp.o

# Object files for target folderSync
folderSync_OBJECTS = \
"CMakeFiles/folderSync.dir/FileSystemWrapper.cpp.o" \
"CMakeFiles/folderSync.dir/HandleRef.cpp.o" \
"CMakeFiles/folderSync.dir/NodeSystem.cpp.o" \
"CMakeFiles/folderSync.dir/main.cpp.o"

# External object files for target folderSync
folderSync_EXTERNAL_OBJECTS =

tools/FolderSync-FUSE/folderSync: tools/FolderSync-FUSE/CMakeFiles/folderSync.dir/FileSystemWrapper.cpp.o
tools/FolderSync-FUSE/folderSync: tools/FolderSync-FUSE/CMakeFiles/folderSync.dir/HandleRef.cpp.o
tools/FolderSync-FUSE/folderSync: tools/FolderSync-FUSE/CMakeFiles/folderSync.dir/NodeSystem.cpp.o
tools/FolderSync-FUSE/folderSync: tools/FolderSync-FUSE/CMakeFiles/folderSync.dir/main.cpp.o
tools/FolderSync-FUSE/folderSync: tools/FolderSync-FUSE/CMakeFiles/folderSync.dir/build.make
tools/FolderSync-FUSE/folderSync: src/OpenP2P/FolderSync/libopenp2p-foldersync.a
tools/FolderSync-FUSE/folderSync: src/OpenP2P/Crypt/libopenp2p-crypt.a
tools/FolderSync-FUSE/folderSync: src/OpenP2P/Stream/libopenp2p-stream.a
tools/FolderSync-FUSE/folderSync: src/OpenP2P/Event/libopenp2p-event.a
tools/FolderSync-FUSE/folderSync: src/OpenP2P/libopenp2p.a
tools/FolderSync-FUSE/folderSync: tools/FolderSync-FUSE/FUSE/libfusewrapper.a
tools/FolderSync-FUSE/folderSync: tools/FolderSync-FUSE/CMakeFiles/folderSync.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --red --bold "Linking CXX executable folderSync"
	cd /home/pi/206/workspace/test/openp2p/build/tools/FolderSync-FUSE && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/folderSync.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
tools/FolderSync-FUSE/CMakeFiles/folderSync.dir/build: tools/FolderSync-FUSE/folderSync
.PHONY : tools/FolderSync-FUSE/CMakeFiles/folderSync.dir/build

tools/FolderSync-FUSE/CMakeFiles/folderSync.dir/requires: tools/FolderSync-FUSE/CMakeFiles/folderSync.dir/FileSystemWrapper.cpp.o.requires
tools/FolderSync-FUSE/CMakeFiles/folderSync.dir/requires: tools/FolderSync-FUSE/CMakeFiles/folderSync.dir/HandleRef.cpp.o.requires
tools/FolderSync-FUSE/CMakeFiles/folderSync.dir/requires: tools/FolderSync-FUSE/CMakeFiles/folderSync.dir/NodeSystem.cpp.o.requires
tools/FolderSync-FUSE/CMakeFiles/folderSync.dir/requires: tools/FolderSync-FUSE/CMakeFiles/folderSync.dir/main.cpp.o.requires
.PHONY : tools/FolderSync-FUSE/CMakeFiles/folderSync.dir/requires

tools/FolderSync-FUSE/CMakeFiles/folderSync.dir/clean:
	cd /home/pi/206/workspace/test/openp2p/build/tools/FolderSync-FUSE && $(CMAKE_COMMAND) -P CMakeFiles/folderSync.dir/cmake_clean.cmake
.PHONY : tools/FolderSync-FUSE/CMakeFiles/folderSync.dir/clean

tools/FolderSync-FUSE/CMakeFiles/folderSync.dir/depend:
	cd /home/pi/206/workspace/test/openp2p/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/pi/206/workspace/test/openp2p /home/pi/206/workspace/test/openp2p/tools/FolderSync-FUSE /home/pi/206/workspace/test/openp2p/build /home/pi/206/workspace/test/openp2p/build/tools/FolderSync-FUSE /home/pi/206/workspace/test/openp2p/build/tools/FolderSync-FUSE/CMakeFiles/folderSync.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : tools/FolderSync-FUSE/CMakeFiles/folderSync.dir/depend

# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.18

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:


#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:


# Disable VCS-based implicit rules.
% : %,v


# Disable VCS-based implicit rules.
% : RCS/%


# Disable VCS-based implicit rules.
% : RCS/%,v


# Disable VCS-based implicit rules.
% : SCCS/s.%


# Disable VCS-based implicit rules.
% : s.%


.SUFFIXES: .hpux_make_needs_suffix_list


# Command-line flag to silence nested $(MAKE).
$(VERBOSE)MAKESILENT = -s

#Suppress display of executed commands.
$(VERBOSE).SILENT:

# A target that is always out of date.
cmake_force:

.PHONY : cmake_force

#=============================================================================
# Set environment variables for the build.

# The shell in which to execute make rules.
SHELL = /bin/sh

# The CMake executable.
CMAKE_COMMAND = /usr/local/bin/cmake

# The command to remove a file.
RM = /usr/local/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = "/mnt/c/Users/Kitsch/Desktop/CS425/MP/MP0-Distributed Log Querier"

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = "/mnt/c/Users/Kitsch/Desktop/CS425/MP/MP0-Distributed Log Querier/cmake-build-debug"

# Include any dependencies generated for this target.
include CMakeFiles/MP0_Distributed_Log_Querier.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/MP0_Distributed_Log_Querier.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/MP0_Distributed_Log_Querier.dir/flags.make

CMakeFiles/MP0_Distributed_Log_Querier.dir/main.cpp.o: CMakeFiles/MP0_Distributed_Log_Querier.dir/flags.make
CMakeFiles/MP0_Distributed_Log_Querier.dir/main.cpp.o: ../main.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir="/mnt/c/Users/Kitsch/Desktop/CS425/MP/MP0-Distributed Log Querier/cmake-build-debug/CMakeFiles" --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/MP0_Distributed_Log_Querier.dir/main.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/MP0_Distributed_Log_Querier.dir/main.cpp.o -c "/mnt/c/Users/Kitsch/Desktop/CS425/MP/MP0-Distributed Log Querier/main.cpp"

CMakeFiles/MP0_Distributed_Log_Querier.dir/main.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/MP0_Distributed_Log_Querier.dir/main.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E "/mnt/c/Users/Kitsch/Desktop/CS425/MP/MP0-Distributed Log Querier/main.cpp" > CMakeFiles/MP0_Distributed_Log_Querier.dir/main.cpp.i

CMakeFiles/MP0_Distributed_Log_Querier.dir/main.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/MP0_Distributed_Log_Querier.dir/main.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S "/mnt/c/Users/Kitsch/Desktop/CS425/MP/MP0-Distributed Log Querier/main.cpp" -o CMakeFiles/MP0_Distributed_Log_Querier.dir/main.cpp.s

CMakeFiles/MP0_Distributed_Log_Querier.dir/src/grep.cpp.o: CMakeFiles/MP0_Distributed_Log_Querier.dir/flags.make
CMakeFiles/MP0_Distributed_Log_Querier.dir/src/grep.cpp.o: ../src/grep.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir="/mnt/c/Users/Kitsch/Desktop/CS425/MP/MP0-Distributed Log Querier/cmake-build-debug/CMakeFiles" --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object CMakeFiles/MP0_Distributed_Log_Querier.dir/src/grep.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/MP0_Distributed_Log_Querier.dir/src/grep.cpp.o -c "/mnt/c/Users/Kitsch/Desktop/CS425/MP/MP0-Distributed Log Querier/src/grep.cpp"

CMakeFiles/MP0_Distributed_Log_Querier.dir/src/grep.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/MP0_Distributed_Log_Querier.dir/src/grep.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E "/mnt/c/Users/Kitsch/Desktop/CS425/MP/MP0-Distributed Log Querier/src/grep.cpp" > CMakeFiles/MP0_Distributed_Log_Querier.dir/src/grep.cpp.i

CMakeFiles/MP0_Distributed_Log_Querier.dir/src/grep.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/MP0_Distributed_Log_Querier.dir/src/grep.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S "/mnt/c/Users/Kitsch/Desktop/CS425/MP/MP0-Distributed Log Querier/src/grep.cpp" -o CMakeFiles/MP0_Distributed_Log_Querier.dir/src/grep.cpp.s

CMakeFiles/MP0_Distributed_Log_Querier.dir/src/run_client.cpp.o: CMakeFiles/MP0_Distributed_Log_Querier.dir/flags.make
CMakeFiles/MP0_Distributed_Log_Querier.dir/src/run_client.cpp.o: ../src/run_client.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir="/mnt/c/Users/Kitsch/Desktop/CS425/MP/MP0-Distributed Log Querier/cmake-build-debug/CMakeFiles" --progress-num=$(CMAKE_PROGRESS_3) "Building CXX object CMakeFiles/MP0_Distributed_Log_Querier.dir/src/run_client.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/MP0_Distributed_Log_Querier.dir/src/run_client.cpp.o -c "/mnt/c/Users/Kitsch/Desktop/CS425/MP/MP0-Distributed Log Querier/src/run_client.cpp"

CMakeFiles/MP0_Distributed_Log_Querier.dir/src/run_client.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/MP0_Distributed_Log_Querier.dir/src/run_client.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E "/mnt/c/Users/Kitsch/Desktop/CS425/MP/MP0-Distributed Log Querier/src/run_client.cpp" > CMakeFiles/MP0_Distributed_Log_Querier.dir/src/run_client.cpp.i

CMakeFiles/MP0_Distributed_Log_Querier.dir/src/run_client.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/MP0_Distributed_Log_Querier.dir/src/run_client.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S "/mnt/c/Users/Kitsch/Desktop/CS425/MP/MP0-Distributed Log Querier/src/run_client.cpp" -o CMakeFiles/MP0_Distributed_Log_Querier.dir/src/run_client.cpp.s

CMakeFiles/MP0_Distributed_Log_Querier.dir/src/run_server.cpp.o: CMakeFiles/MP0_Distributed_Log_Querier.dir/flags.make
CMakeFiles/MP0_Distributed_Log_Querier.dir/src/run_server.cpp.o: ../src/run_server.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir="/mnt/c/Users/Kitsch/Desktop/CS425/MP/MP0-Distributed Log Querier/cmake-build-debug/CMakeFiles" --progress-num=$(CMAKE_PROGRESS_4) "Building CXX object CMakeFiles/MP0_Distributed_Log_Querier.dir/src/run_server.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/MP0_Distributed_Log_Querier.dir/src/run_server.cpp.o -c "/mnt/c/Users/Kitsch/Desktop/CS425/MP/MP0-Distributed Log Querier/src/run_server.cpp"

CMakeFiles/MP0_Distributed_Log_Querier.dir/src/run_server.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/MP0_Distributed_Log_Querier.dir/src/run_server.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E "/mnt/c/Users/Kitsch/Desktop/CS425/MP/MP0-Distributed Log Querier/src/run_server.cpp" > CMakeFiles/MP0_Distributed_Log_Querier.dir/src/run_server.cpp.i

CMakeFiles/MP0_Distributed_Log_Querier.dir/src/run_server.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/MP0_Distributed_Log_Querier.dir/src/run_server.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S "/mnt/c/Users/Kitsch/Desktop/CS425/MP/MP0-Distributed Log Querier/src/run_server.cpp" -o CMakeFiles/MP0_Distributed_Log_Querier.dir/src/run_server.cpp.s

# Object files for target MP0_Distributed_Log_Querier
MP0_Distributed_Log_Querier_OBJECTS = \
"CMakeFiles/MP0_Distributed_Log_Querier.dir/main.cpp.o" \
"CMakeFiles/MP0_Distributed_Log_Querier.dir/src/grep.cpp.o" \
"CMakeFiles/MP0_Distributed_Log_Querier.dir/src/run_client.cpp.o" \
"CMakeFiles/MP0_Distributed_Log_Querier.dir/src/run_server.cpp.o"

# External object files for target MP0_Distributed_Log_Querier
MP0_Distributed_Log_Querier_EXTERNAL_OBJECTS =

MP0_Distributed_Log_Querier: CMakeFiles/MP0_Distributed_Log_Querier.dir/main.cpp.o
MP0_Distributed_Log_Querier: CMakeFiles/MP0_Distributed_Log_Querier.dir/src/grep.cpp.o
MP0_Distributed_Log_Querier: CMakeFiles/MP0_Distributed_Log_Querier.dir/src/run_client.cpp.o
MP0_Distributed_Log_Querier: CMakeFiles/MP0_Distributed_Log_Querier.dir/src/run_server.cpp.o
MP0_Distributed_Log_Querier: CMakeFiles/MP0_Distributed_Log_Querier.dir/build.make
MP0_Distributed_Log_Querier: CMakeFiles/MP0_Distributed_Log_Querier.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir="/mnt/c/Users/Kitsch/Desktop/CS425/MP/MP0-Distributed Log Querier/cmake-build-debug/CMakeFiles" --progress-num=$(CMAKE_PROGRESS_5) "Linking CXX executable MP0_Distributed_Log_Querier"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/MP0_Distributed_Log_Querier.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/MP0_Distributed_Log_Querier.dir/build: MP0_Distributed_Log_Querier

.PHONY : CMakeFiles/MP0_Distributed_Log_Querier.dir/build

CMakeFiles/MP0_Distributed_Log_Querier.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/MP0_Distributed_Log_Querier.dir/cmake_clean.cmake
.PHONY : CMakeFiles/MP0_Distributed_Log_Querier.dir/clean

CMakeFiles/MP0_Distributed_Log_Querier.dir/depend:
	cd "/mnt/c/Users/Kitsch/Desktop/CS425/MP/MP0-Distributed Log Querier/cmake-build-debug" && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" "/mnt/c/Users/Kitsch/Desktop/CS425/MP/MP0-Distributed Log Querier" "/mnt/c/Users/Kitsch/Desktop/CS425/MP/MP0-Distributed Log Querier" "/mnt/c/Users/Kitsch/Desktop/CS425/MP/MP0-Distributed Log Querier/cmake-build-debug" "/mnt/c/Users/Kitsch/Desktop/CS425/MP/MP0-Distributed Log Querier/cmake-build-debug" "/mnt/c/Users/Kitsch/Desktop/CS425/MP/MP0-Distributed Log Querier/cmake-build-debug/CMakeFiles/MP0_Distributed_Log_Querier.dir/DependInfo.cmake" --color=$(COLOR)
.PHONY : CMakeFiles/MP0_Distributed_Log_Querier.dir/depend


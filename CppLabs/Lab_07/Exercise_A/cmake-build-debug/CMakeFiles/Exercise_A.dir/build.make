# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.15

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:


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
CMAKE_COMMAND = /Applications/CLion.app/Contents/bin/cmake/mac/bin/cmake

# The command to remove a file.
RM = /Applications/CLion.app/Contents/bin/cmake/mac/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = "/Users/mattiarosso/Desktop/LM-I anno-II semestre/System_and_device_programming/Quer/poliTO-SystemAndDeviceProgramming-Labs/CppLabs/Lab_07/Exercise_A"

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = "/Users/mattiarosso/Desktop/LM-I anno-II semestre/System_and_device_programming/Quer/poliTO-SystemAndDeviceProgramming-Labs/CppLabs/Lab_07/Exercise_A/cmake-build-debug"

# Include any dependencies generated for this target.
include CMakeFiles/Exercise_A.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/Exercise_A.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/Exercise_A.dir/flags.make

CMakeFiles/Exercise_A.dir/main.cpp.o: CMakeFiles/Exercise_A.dir/flags.make
CMakeFiles/Exercise_A.dir/main.cpp.o: ../main.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir="/Users/mattiarosso/Desktop/LM-I anno-II semestre/System_and_device_programming/Quer/poliTO-SystemAndDeviceProgramming-Labs/CppLabs/Lab_07/Exercise_A/cmake-build-debug/CMakeFiles" --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/Exercise_A.dir/main.cpp.o"
	/Library/Developer/CommandLineTools/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/Exercise_A.dir/main.cpp.o -c "/Users/mattiarosso/Desktop/LM-I anno-II semestre/System_and_device_programming/Quer/poliTO-SystemAndDeviceProgramming-Labs/CppLabs/Lab_07/Exercise_A/main.cpp"

CMakeFiles/Exercise_A.dir/main.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/Exercise_A.dir/main.cpp.i"
	/Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E "/Users/mattiarosso/Desktop/LM-I anno-II semestre/System_and_device_programming/Quer/poliTO-SystemAndDeviceProgramming-Labs/CppLabs/Lab_07/Exercise_A/main.cpp" > CMakeFiles/Exercise_A.dir/main.cpp.i

CMakeFiles/Exercise_A.dir/main.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/Exercise_A.dir/main.cpp.s"
	/Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S "/Users/mattiarosso/Desktop/LM-I anno-II semestre/System_and_device_programming/Quer/poliTO-SystemAndDeviceProgramming-Labs/CppLabs/Lab_07/Exercise_A/main.cpp" -o CMakeFiles/Exercise_A.dir/main.cpp.s

# Object files for target Exercise_A
Exercise_A_OBJECTS = \
"CMakeFiles/Exercise_A.dir/main.cpp.o"

# External object files for target Exercise_A
Exercise_A_EXTERNAL_OBJECTS =

Exercise_A: CMakeFiles/Exercise_A.dir/main.cpp.o
Exercise_A: CMakeFiles/Exercise_A.dir/build.make
Exercise_A: CMakeFiles/Exercise_A.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir="/Users/mattiarosso/Desktop/LM-I anno-II semestre/System_and_device_programming/Quer/poliTO-SystemAndDeviceProgramming-Labs/CppLabs/Lab_07/Exercise_A/cmake-build-debug/CMakeFiles" --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable Exercise_A"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/Exercise_A.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/Exercise_A.dir/build: Exercise_A

.PHONY : CMakeFiles/Exercise_A.dir/build

CMakeFiles/Exercise_A.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/Exercise_A.dir/cmake_clean.cmake
.PHONY : CMakeFiles/Exercise_A.dir/clean

CMakeFiles/Exercise_A.dir/depend:
	cd "/Users/mattiarosso/Desktop/LM-I anno-II semestre/System_and_device_programming/Quer/poliTO-SystemAndDeviceProgramming-Labs/CppLabs/Lab_07/Exercise_A/cmake-build-debug" && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" "/Users/mattiarosso/Desktop/LM-I anno-II semestre/System_and_device_programming/Quer/poliTO-SystemAndDeviceProgramming-Labs/CppLabs/Lab_07/Exercise_A" "/Users/mattiarosso/Desktop/LM-I anno-II semestre/System_and_device_programming/Quer/poliTO-SystemAndDeviceProgramming-Labs/CppLabs/Lab_07/Exercise_A" "/Users/mattiarosso/Desktop/LM-I anno-II semestre/System_and_device_programming/Quer/poliTO-SystemAndDeviceProgramming-Labs/CppLabs/Lab_07/Exercise_A/cmake-build-debug" "/Users/mattiarosso/Desktop/LM-I anno-II semestre/System_and_device_programming/Quer/poliTO-SystemAndDeviceProgramming-Labs/CppLabs/Lab_07/Exercise_A/cmake-build-debug" "/Users/mattiarosso/Desktop/LM-I anno-II semestre/System_and_device_programming/Quer/poliTO-SystemAndDeviceProgramming-Labs/CppLabs/Lab_07/Exercise_A/cmake-build-debug/CMakeFiles/Exercise_A.dir/DependInfo.cmake" --color=$(COLOR)
.PHONY : CMakeFiles/Exercise_A.dir/depend

# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.22

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
CMAKE_COMMAND = /usr/bin/cmake

# The command to remove a file.
RM = /usr/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /mnt/c/Users/Олеся/CLionProjects/sistpr_4

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /mnt/c/Users/Олеся/CLionProjects/sistpr_4/cmake-build-debug

# Include any dependencies generated for this target.
include CMakeFiles/main1.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include CMakeFiles/main1.dir/compiler_depend.make

# Include the progress variables for this target.
include CMakeFiles/main1.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/main1.dir/flags.make

CMakeFiles/main1.dir/task1/main1.c.o: CMakeFiles/main1.dir/flags.make
CMakeFiles/main1.dir/task1/main1.c.o: ../task1/main1.c
CMakeFiles/main1.dir/task1/main1.c.o: CMakeFiles/main1.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/mnt/c/Users/Олеся/CLionProjects/sistpr_4/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building C object CMakeFiles/main1.dir/task1/main1.c.o"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -MD -MT CMakeFiles/main1.dir/task1/main1.c.o -MF CMakeFiles/main1.dir/task1/main1.c.o.d -o CMakeFiles/main1.dir/task1/main1.c.o -c /mnt/c/Users/Олеся/CLionProjects/sistpr_4/task1/main1.c

CMakeFiles/main1.dir/task1/main1.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/main1.dir/task1/main1.c.i"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /mnt/c/Users/Олеся/CLionProjects/sistpr_4/task1/main1.c > CMakeFiles/main1.dir/task1/main1.c.i

CMakeFiles/main1.dir/task1/main1.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/main1.dir/task1/main1.c.s"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /mnt/c/Users/Олеся/CLionProjects/sistpr_4/task1/main1.c -o CMakeFiles/main1.dir/task1/main1.c.s

# Object files for target main1
main1_OBJECTS = \
"CMakeFiles/main1.dir/task1/main1.c.o"

# External object files for target main1
main1_EXTERNAL_OBJECTS =

main1: CMakeFiles/main1.dir/task1/main1.c.o
main1: CMakeFiles/main1.dir/build.make
main1: CMakeFiles/main1.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/mnt/c/Users/Олеся/CLionProjects/sistpr_4/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking C executable main1"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/main1.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/main1.dir/build: main1
.PHONY : CMakeFiles/main1.dir/build

CMakeFiles/main1.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/main1.dir/cmake_clean.cmake
.PHONY : CMakeFiles/main1.dir/clean

CMakeFiles/main1.dir/depend:
	cd /mnt/c/Users/Олеся/CLionProjects/sistpr_4/cmake-build-debug && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /mnt/c/Users/Олеся/CLionProjects/sistpr_4 /mnt/c/Users/Олеся/CLionProjects/sistpr_4 /mnt/c/Users/Олеся/CLionProjects/sistpr_4/cmake-build-debug /mnt/c/Users/Олеся/CLionProjects/sistpr_4/cmake-build-debug /mnt/c/Users/Олеся/CLionProjects/sistpr_4/cmake-build-debug/CMakeFiles/main1.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/main1.dir/depend


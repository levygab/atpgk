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
CMAKE_SOURCE_DIR = /home/gabriel/Documents/ecole/3A/semestre1/ATPG/atpg_git

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/gabriel/Documents/ecole/3A/semestre1/ATPG/atpg_git/build

# Include any dependencies generated for this target.
include CMakeFiles/TOP_LEVEL.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include CMakeFiles/TOP_LEVEL.dir/compiler_depend.make

# Include the progress variables for this target.
include CMakeFiles/TOP_LEVEL.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/TOP_LEVEL.dir/flags.make

CMakeFiles/TOP_LEVEL.dir/src/atpg_top/atpg_top.cpp.o: CMakeFiles/TOP_LEVEL.dir/flags.make
CMakeFiles/TOP_LEVEL.dir/src/atpg_top/atpg_top.cpp.o: ../src/atpg_top/atpg_top.cpp
CMakeFiles/TOP_LEVEL.dir/src/atpg_top/atpg_top.cpp.o: CMakeFiles/TOP_LEVEL.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/gabriel/Documents/ecole/3A/semestre1/ATPG/atpg_git/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/TOP_LEVEL.dir/src/atpg_top/atpg_top.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/TOP_LEVEL.dir/src/atpg_top/atpg_top.cpp.o -MF CMakeFiles/TOP_LEVEL.dir/src/atpg_top/atpg_top.cpp.o.d -o CMakeFiles/TOP_LEVEL.dir/src/atpg_top/atpg_top.cpp.o -c /home/gabriel/Documents/ecole/3A/semestre1/ATPG/atpg_git/src/atpg_top/atpg_top.cpp

CMakeFiles/TOP_LEVEL.dir/src/atpg_top/atpg_top.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/TOP_LEVEL.dir/src/atpg_top/atpg_top.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/gabriel/Documents/ecole/3A/semestre1/ATPG/atpg_git/src/atpg_top/atpg_top.cpp > CMakeFiles/TOP_LEVEL.dir/src/atpg_top/atpg_top.cpp.i

CMakeFiles/TOP_LEVEL.dir/src/atpg_top/atpg_top.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/TOP_LEVEL.dir/src/atpg_top/atpg_top.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/gabriel/Documents/ecole/3A/semestre1/ATPG/atpg_git/src/atpg_top/atpg_top.cpp -o CMakeFiles/TOP_LEVEL.dir/src/atpg_top/atpg_top.cpp.s

# Object files for target TOP_LEVEL
TOP_LEVEL_OBJECTS = \
"CMakeFiles/TOP_LEVEL.dir/src/atpg_top/atpg_top.cpp.o"

# External object files for target TOP_LEVEL
TOP_LEVEL_EXTERNAL_OBJECTS =

libTOP_LEVEL.so: CMakeFiles/TOP_LEVEL.dir/src/atpg_top/atpg_top.cpp.o
libTOP_LEVEL.so: CMakeFiles/TOP_LEVEL.dir/build.make
libTOP_LEVEL.so: libREADER.so
libTOP_LEVEL.so: libWRITER_TXT.so
libTOP_LEVEL.so: libWRITER_JSON.so
libTOP_LEVEL.so: libFAULT_API.so
libTOP_LEVEL.so: libPARSER_JSON.so
libTOP_LEVEL.so: libCIRCUIT_TREE.so
libTOP_LEVEL.so: libBUILDER_API.so
libTOP_LEVEL.so: CMakeFiles/TOP_LEVEL.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/gabriel/Documents/ecole/3A/semestre1/ATPG/atpg_git/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX shared library libTOP_LEVEL.so"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/TOP_LEVEL.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/TOP_LEVEL.dir/build: libTOP_LEVEL.so
.PHONY : CMakeFiles/TOP_LEVEL.dir/build

CMakeFiles/TOP_LEVEL.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/TOP_LEVEL.dir/cmake_clean.cmake
.PHONY : CMakeFiles/TOP_LEVEL.dir/clean

CMakeFiles/TOP_LEVEL.dir/depend:
	cd /home/gabriel/Documents/ecole/3A/semestre1/ATPG/atpg_git/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/gabriel/Documents/ecole/3A/semestre1/ATPG/atpg_git /home/gabriel/Documents/ecole/3A/semestre1/ATPG/atpg_git /home/gabriel/Documents/ecole/3A/semestre1/ATPG/atpg_git/build /home/gabriel/Documents/ecole/3A/semestre1/ATPG/atpg_git/build /home/gabriel/Documents/ecole/3A/semestre1/ATPG/atpg_git/build/CMakeFiles/TOP_LEVEL.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/TOP_LEVEL.dir/depend

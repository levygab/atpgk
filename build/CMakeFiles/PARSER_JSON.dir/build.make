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
include CMakeFiles/PARSER_JSON.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include CMakeFiles/PARSER_JSON.dir/compiler_depend.make

# Include the progress variables for this target.
include CMakeFiles/PARSER_JSON.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/PARSER_JSON.dir/flags.make

CMakeFiles/PARSER_JSON.dir/src/parser/gate.cpp.o: CMakeFiles/PARSER_JSON.dir/flags.make
CMakeFiles/PARSER_JSON.dir/src/parser/gate.cpp.o: ../src/parser/gate.cpp
CMakeFiles/PARSER_JSON.dir/src/parser/gate.cpp.o: CMakeFiles/PARSER_JSON.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/gabriel/Documents/ecole/3A/semestre1/ATPG/atpg_git/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/PARSER_JSON.dir/src/parser/gate.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/PARSER_JSON.dir/src/parser/gate.cpp.o -MF CMakeFiles/PARSER_JSON.dir/src/parser/gate.cpp.o.d -o CMakeFiles/PARSER_JSON.dir/src/parser/gate.cpp.o -c /home/gabriel/Documents/ecole/3A/semestre1/ATPG/atpg_git/src/parser/gate.cpp

CMakeFiles/PARSER_JSON.dir/src/parser/gate.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/PARSER_JSON.dir/src/parser/gate.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/gabriel/Documents/ecole/3A/semestre1/ATPG/atpg_git/src/parser/gate.cpp > CMakeFiles/PARSER_JSON.dir/src/parser/gate.cpp.i

CMakeFiles/PARSER_JSON.dir/src/parser/gate.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/PARSER_JSON.dir/src/parser/gate.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/gabriel/Documents/ecole/3A/semestre1/ATPG/atpg_git/src/parser/gate.cpp -o CMakeFiles/PARSER_JSON.dir/src/parser/gate.cpp.s

CMakeFiles/PARSER_JSON.dir/src/parser/parsed_circuit.cpp.o: CMakeFiles/PARSER_JSON.dir/flags.make
CMakeFiles/PARSER_JSON.dir/src/parser/parsed_circuit.cpp.o: ../src/parser/parsed_circuit.cpp
CMakeFiles/PARSER_JSON.dir/src/parser/parsed_circuit.cpp.o: CMakeFiles/PARSER_JSON.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/gabriel/Documents/ecole/3A/semestre1/ATPG/atpg_git/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object CMakeFiles/PARSER_JSON.dir/src/parser/parsed_circuit.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/PARSER_JSON.dir/src/parser/parsed_circuit.cpp.o -MF CMakeFiles/PARSER_JSON.dir/src/parser/parsed_circuit.cpp.o.d -o CMakeFiles/PARSER_JSON.dir/src/parser/parsed_circuit.cpp.o -c /home/gabriel/Documents/ecole/3A/semestre1/ATPG/atpg_git/src/parser/parsed_circuit.cpp

CMakeFiles/PARSER_JSON.dir/src/parser/parsed_circuit.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/PARSER_JSON.dir/src/parser/parsed_circuit.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/gabriel/Documents/ecole/3A/semestre1/ATPG/atpg_git/src/parser/parsed_circuit.cpp > CMakeFiles/PARSER_JSON.dir/src/parser/parsed_circuit.cpp.i

CMakeFiles/PARSER_JSON.dir/src/parser/parsed_circuit.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/PARSER_JSON.dir/src/parser/parsed_circuit.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/gabriel/Documents/ecole/3A/semestre1/ATPG/atpg_git/src/parser/parsed_circuit.cpp -o CMakeFiles/PARSER_JSON.dir/src/parser/parsed_circuit.cpp.s

CMakeFiles/PARSER_JSON.dir/src/parser/parser.cpp.o: CMakeFiles/PARSER_JSON.dir/flags.make
CMakeFiles/PARSER_JSON.dir/src/parser/parser.cpp.o: ../src/parser/parser.cpp
CMakeFiles/PARSER_JSON.dir/src/parser/parser.cpp.o: CMakeFiles/PARSER_JSON.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/gabriel/Documents/ecole/3A/semestre1/ATPG/atpg_git/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building CXX object CMakeFiles/PARSER_JSON.dir/src/parser/parser.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/PARSER_JSON.dir/src/parser/parser.cpp.o -MF CMakeFiles/PARSER_JSON.dir/src/parser/parser.cpp.o.d -o CMakeFiles/PARSER_JSON.dir/src/parser/parser.cpp.o -c /home/gabriel/Documents/ecole/3A/semestre1/ATPG/atpg_git/src/parser/parser.cpp

CMakeFiles/PARSER_JSON.dir/src/parser/parser.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/PARSER_JSON.dir/src/parser/parser.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/gabriel/Documents/ecole/3A/semestre1/ATPG/atpg_git/src/parser/parser.cpp > CMakeFiles/PARSER_JSON.dir/src/parser/parser.cpp.i

CMakeFiles/PARSER_JSON.dir/src/parser/parser.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/PARSER_JSON.dir/src/parser/parser.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/gabriel/Documents/ecole/3A/semestre1/ATPG/atpg_git/src/parser/parser.cpp -o CMakeFiles/PARSER_JSON.dir/src/parser/parser.cpp.s

CMakeFiles/PARSER_JSON.dir/src/parser/yosys_json_parser.cpp.o: CMakeFiles/PARSER_JSON.dir/flags.make
CMakeFiles/PARSER_JSON.dir/src/parser/yosys_json_parser.cpp.o: ../src/parser/yosys_json_parser.cpp
CMakeFiles/PARSER_JSON.dir/src/parser/yosys_json_parser.cpp.o: CMakeFiles/PARSER_JSON.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/gabriel/Documents/ecole/3A/semestre1/ATPG/atpg_git/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Building CXX object CMakeFiles/PARSER_JSON.dir/src/parser/yosys_json_parser.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/PARSER_JSON.dir/src/parser/yosys_json_parser.cpp.o -MF CMakeFiles/PARSER_JSON.dir/src/parser/yosys_json_parser.cpp.o.d -o CMakeFiles/PARSER_JSON.dir/src/parser/yosys_json_parser.cpp.o -c /home/gabriel/Documents/ecole/3A/semestre1/ATPG/atpg_git/src/parser/yosys_json_parser.cpp

CMakeFiles/PARSER_JSON.dir/src/parser/yosys_json_parser.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/PARSER_JSON.dir/src/parser/yosys_json_parser.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/gabriel/Documents/ecole/3A/semestre1/ATPG/atpg_git/src/parser/yosys_json_parser.cpp > CMakeFiles/PARSER_JSON.dir/src/parser/yosys_json_parser.cpp.i

CMakeFiles/PARSER_JSON.dir/src/parser/yosys_json_parser.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/PARSER_JSON.dir/src/parser/yosys_json_parser.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/gabriel/Documents/ecole/3A/semestre1/ATPG/atpg_git/src/parser/yosys_json_parser.cpp -o CMakeFiles/PARSER_JSON.dir/src/parser/yosys_json_parser.cpp.s

# Object files for target PARSER_JSON
PARSER_JSON_OBJECTS = \
"CMakeFiles/PARSER_JSON.dir/src/parser/gate.cpp.o" \
"CMakeFiles/PARSER_JSON.dir/src/parser/parsed_circuit.cpp.o" \
"CMakeFiles/PARSER_JSON.dir/src/parser/parser.cpp.o" \
"CMakeFiles/PARSER_JSON.dir/src/parser/yosys_json_parser.cpp.o"

# External object files for target PARSER_JSON
PARSER_JSON_EXTERNAL_OBJECTS =

libPARSER_JSON.so: CMakeFiles/PARSER_JSON.dir/src/parser/gate.cpp.o
libPARSER_JSON.so: CMakeFiles/PARSER_JSON.dir/src/parser/parsed_circuit.cpp.o
libPARSER_JSON.so: CMakeFiles/PARSER_JSON.dir/src/parser/parser.cpp.o
libPARSER_JSON.so: CMakeFiles/PARSER_JSON.dir/src/parser/yosys_json_parser.cpp.o
libPARSER_JSON.so: CMakeFiles/PARSER_JSON.dir/build.make
libPARSER_JSON.so: CMakeFiles/PARSER_JSON.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/gabriel/Documents/ecole/3A/semestre1/ATPG/atpg_git/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_5) "Linking CXX shared library libPARSER_JSON.so"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/PARSER_JSON.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/PARSER_JSON.dir/build: libPARSER_JSON.so
.PHONY : CMakeFiles/PARSER_JSON.dir/build

CMakeFiles/PARSER_JSON.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/PARSER_JSON.dir/cmake_clean.cmake
.PHONY : CMakeFiles/PARSER_JSON.dir/clean

CMakeFiles/PARSER_JSON.dir/depend:
	cd /home/gabriel/Documents/ecole/3A/semestre1/ATPG/atpg_git/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/gabriel/Documents/ecole/3A/semestre1/ATPG/atpg_git /home/gabriel/Documents/ecole/3A/semestre1/ATPG/atpg_git /home/gabriel/Documents/ecole/3A/semestre1/ATPG/atpg_git/build /home/gabriel/Documents/ecole/3A/semestre1/ATPG/atpg_git/build /home/gabriel/Documents/ecole/3A/semestre1/ATPG/atpg_git/build/CMakeFiles/PARSER_JSON.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/PARSER_JSON.dir/depend

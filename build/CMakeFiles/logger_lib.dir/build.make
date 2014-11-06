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
CMAKE_SOURCE_DIR = /home/joe/programming/cpp/projects/xml_named_entity_miner

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/joe/programming/cpp/projects/xml_named_entity_miner/build

# Include any dependencies generated for this target.
include CMakeFiles/logger_lib.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/logger_lib.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/logger_lib.dir/flags.make

CMakeFiles/logger_lib.dir/lib/logger.cpp.o: CMakeFiles/logger_lib.dir/flags.make
CMakeFiles/logger_lib.dir/lib/logger.cpp.o: ../lib/logger.cpp
	$(CMAKE_COMMAND) -E cmake_progress_report /home/joe/programming/cpp/projects/xml_named_entity_miner/build/CMakeFiles $(CMAKE_PROGRESS_1)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building CXX object CMakeFiles/logger_lib.dir/lib/logger.cpp.o"
	/usr/bin/clang++   $(CXX_DEFINES) $(CXX_FLAGS) -o CMakeFiles/logger_lib.dir/lib/logger.cpp.o -c /home/joe/programming/cpp/projects/xml_named_entity_miner/lib/logger.cpp

CMakeFiles/logger_lib.dir/lib/logger.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/logger_lib.dir/lib/logger.cpp.i"
	/usr/bin/clang++  $(CXX_DEFINES) $(CXX_FLAGS) -E /home/joe/programming/cpp/projects/xml_named_entity_miner/lib/logger.cpp > CMakeFiles/logger_lib.dir/lib/logger.cpp.i

CMakeFiles/logger_lib.dir/lib/logger.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/logger_lib.dir/lib/logger.cpp.s"
	/usr/bin/clang++  $(CXX_DEFINES) $(CXX_FLAGS) -S /home/joe/programming/cpp/projects/xml_named_entity_miner/lib/logger.cpp -o CMakeFiles/logger_lib.dir/lib/logger.cpp.s

CMakeFiles/logger_lib.dir/lib/logger.cpp.o.requires:
.PHONY : CMakeFiles/logger_lib.dir/lib/logger.cpp.o.requires

CMakeFiles/logger_lib.dir/lib/logger.cpp.o.provides: CMakeFiles/logger_lib.dir/lib/logger.cpp.o.requires
	$(MAKE) -f CMakeFiles/logger_lib.dir/build.make CMakeFiles/logger_lib.dir/lib/logger.cpp.o.provides.build
.PHONY : CMakeFiles/logger_lib.dir/lib/logger.cpp.o.provides

CMakeFiles/logger_lib.dir/lib/logger.cpp.o.provides.build: CMakeFiles/logger_lib.dir/lib/logger.cpp.o

CMakeFiles/logger_lib.dir/lib/log.cpp.o: CMakeFiles/logger_lib.dir/flags.make
CMakeFiles/logger_lib.dir/lib/log.cpp.o: ../lib/log.cpp
	$(CMAKE_COMMAND) -E cmake_progress_report /home/joe/programming/cpp/projects/xml_named_entity_miner/build/CMakeFiles $(CMAKE_PROGRESS_2)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building CXX object CMakeFiles/logger_lib.dir/lib/log.cpp.o"
	/usr/bin/clang++   $(CXX_DEFINES) $(CXX_FLAGS) -o CMakeFiles/logger_lib.dir/lib/log.cpp.o -c /home/joe/programming/cpp/projects/xml_named_entity_miner/lib/log.cpp

CMakeFiles/logger_lib.dir/lib/log.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/logger_lib.dir/lib/log.cpp.i"
	/usr/bin/clang++  $(CXX_DEFINES) $(CXX_FLAGS) -E /home/joe/programming/cpp/projects/xml_named_entity_miner/lib/log.cpp > CMakeFiles/logger_lib.dir/lib/log.cpp.i

CMakeFiles/logger_lib.dir/lib/log.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/logger_lib.dir/lib/log.cpp.s"
	/usr/bin/clang++  $(CXX_DEFINES) $(CXX_FLAGS) -S /home/joe/programming/cpp/projects/xml_named_entity_miner/lib/log.cpp -o CMakeFiles/logger_lib.dir/lib/log.cpp.s

CMakeFiles/logger_lib.dir/lib/log.cpp.o.requires:
.PHONY : CMakeFiles/logger_lib.dir/lib/log.cpp.o.requires

CMakeFiles/logger_lib.dir/lib/log.cpp.o.provides: CMakeFiles/logger_lib.dir/lib/log.cpp.o.requires
	$(MAKE) -f CMakeFiles/logger_lib.dir/build.make CMakeFiles/logger_lib.dir/lib/log.cpp.o.provides.build
.PHONY : CMakeFiles/logger_lib.dir/lib/log.cpp.o.provides

CMakeFiles/logger_lib.dir/lib/log.cpp.o.provides.build: CMakeFiles/logger_lib.dir/lib/log.cpp.o

# Object files for target logger_lib
logger_lib_OBJECTS = \
"CMakeFiles/logger_lib.dir/lib/logger.cpp.o" \
"CMakeFiles/logger_lib.dir/lib/log.cpp.o"

# External object files for target logger_lib
logger_lib_EXTERNAL_OBJECTS =

liblogger_lib.a: CMakeFiles/logger_lib.dir/lib/logger.cpp.o
liblogger_lib.a: CMakeFiles/logger_lib.dir/lib/log.cpp.o
liblogger_lib.a: CMakeFiles/logger_lib.dir/build.make
liblogger_lib.a: CMakeFiles/logger_lib.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --red --bold "Linking CXX static library liblogger_lib.a"
	$(CMAKE_COMMAND) -P CMakeFiles/logger_lib.dir/cmake_clean_target.cmake
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/logger_lib.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/logger_lib.dir/build: liblogger_lib.a
.PHONY : CMakeFiles/logger_lib.dir/build

CMakeFiles/logger_lib.dir/requires: CMakeFiles/logger_lib.dir/lib/logger.cpp.o.requires
CMakeFiles/logger_lib.dir/requires: CMakeFiles/logger_lib.dir/lib/log.cpp.o.requires
.PHONY : CMakeFiles/logger_lib.dir/requires

CMakeFiles/logger_lib.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/logger_lib.dir/cmake_clean.cmake
.PHONY : CMakeFiles/logger_lib.dir/clean

CMakeFiles/logger_lib.dir/depend:
	cd /home/joe/programming/cpp/projects/xml_named_entity_miner/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/joe/programming/cpp/projects/xml_named_entity_miner /home/joe/programming/cpp/projects/xml_named_entity_miner /home/joe/programming/cpp/projects/xml_named_entity_miner/build /home/joe/programming/cpp/projects/xml_named_entity_miner/build /home/joe/programming/cpp/projects/xml_named_entity_miner/build/CMakeFiles/logger_lib.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/logger_lib.dir/depend


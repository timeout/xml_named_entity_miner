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
include CMakeFiles/xml_lib.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/xml_lib.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/xml_lib.dir/flags.make

CMakeFiles/xml_lib.dir/lib/xml_doc.cpp.o: CMakeFiles/xml_lib.dir/flags.make
CMakeFiles/xml_lib.dir/lib/xml_doc.cpp.o: ../lib/xml_doc.cpp
	$(CMAKE_COMMAND) -E cmake_progress_report /home/joe/programming/cpp/projects/xml_named_entity_miner/build/CMakeFiles $(CMAKE_PROGRESS_1)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building CXX object CMakeFiles/xml_lib.dir/lib/xml_doc.cpp.o"
	/usr/bin/clang++   $(CXX_DEFINES) $(CXX_FLAGS) -o CMakeFiles/xml_lib.dir/lib/xml_doc.cpp.o -c /home/joe/programming/cpp/projects/xml_named_entity_miner/lib/xml_doc.cpp

CMakeFiles/xml_lib.dir/lib/xml_doc.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/xml_lib.dir/lib/xml_doc.cpp.i"
	/usr/bin/clang++  $(CXX_DEFINES) $(CXX_FLAGS) -E /home/joe/programming/cpp/projects/xml_named_entity_miner/lib/xml_doc.cpp > CMakeFiles/xml_lib.dir/lib/xml_doc.cpp.i

CMakeFiles/xml_lib.dir/lib/xml_doc.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/xml_lib.dir/lib/xml_doc.cpp.s"
	/usr/bin/clang++  $(CXX_DEFINES) $(CXX_FLAGS) -S /home/joe/programming/cpp/projects/xml_named_entity_miner/lib/xml_doc.cpp -o CMakeFiles/xml_lib.dir/lib/xml_doc.cpp.s

CMakeFiles/xml_lib.dir/lib/xml_doc.cpp.o.requires:
.PHONY : CMakeFiles/xml_lib.dir/lib/xml_doc.cpp.o.requires

CMakeFiles/xml_lib.dir/lib/xml_doc.cpp.o.provides: CMakeFiles/xml_lib.dir/lib/xml_doc.cpp.o.requires
	$(MAKE) -f CMakeFiles/xml_lib.dir/build.make CMakeFiles/xml_lib.dir/lib/xml_doc.cpp.o.provides.build
.PHONY : CMakeFiles/xml_lib.dir/lib/xml_doc.cpp.o.provides

CMakeFiles/xml_lib.dir/lib/xml_doc.cpp.o.provides.build: CMakeFiles/xml_lib.dir/lib/xml_doc.cpp.o

# Object files for target xml_lib
xml_lib_OBJECTS = \
"CMakeFiles/xml_lib.dir/lib/xml_doc.cpp.o"

# External object files for target xml_lib
xml_lib_EXTERNAL_OBJECTS =

libxml_lib.a: CMakeFiles/xml_lib.dir/lib/xml_doc.cpp.o
libxml_lib.a: CMakeFiles/xml_lib.dir/build.make
libxml_lib.a: CMakeFiles/xml_lib.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --red --bold "Linking CXX static library libxml_lib.a"
	$(CMAKE_COMMAND) -P CMakeFiles/xml_lib.dir/cmake_clean_target.cmake
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/xml_lib.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/xml_lib.dir/build: libxml_lib.a
.PHONY : CMakeFiles/xml_lib.dir/build

CMakeFiles/xml_lib.dir/requires: CMakeFiles/xml_lib.dir/lib/xml_doc.cpp.o.requires
.PHONY : CMakeFiles/xml_lib.dir/requires

CMakeFiles/xml_lib.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/xml_lib.dir/cmake_clean.cmake
.PHONY : CMakeFiles/xml_lib.dir/clean

CMakeFiles/xml_lib.dir/depend:
	cd /home/joe/programming/cpp/projects/xml_named_entity_miner/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/joe/programming/cpp/projects/xml_named_entity_miner /home/joe/programming/cpp/projects/xml_named_entity_miner /home/joe/programming/cpp/projects/xml_named_entity_miner/build /home/joe/programming/cpp/projects/xml_named_entity_miner/build /home/joe/programming/cpp/projects/xml_named_entity_miner/build/CMakeFiles/xml_lib.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/xml_lib.dir/depend


# CMAKE generated file: DO NOT EDIT!
# Generated by "MinGW Makefiles" Generator, CMake Version 3.15

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

SHELL = cmd.exe

# The CMake executable.
CMAKE_COMMAND = "D:\CLion\CLion 2019.3.6\bin\cmake\win\bin\cmake.exe"

# The command to remove a file.
RM = "D:\CLion\CLion 2019.3.6\bin\cmake\win\bin\cmake.exe" -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = D:\aaaaaaaaaaaa\CE\compiler

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = D:\aaaaaaaaaaaa\CE\compiler\cmake-build-debug

# Include any dependencies generated for this target.
include CMakeFiles/compiler.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/compiler.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/compiler.dir/flags.make

CMakeFiles/compiler.dir/main.cpp.obj: CMakeFiles/compiler.dir/flags.make
CMakeFiles/compiler.dir/main.cpp.obj: ../main.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=D:\aaaaaaaaaaaa\CE\compiler\cmake-build-debug\CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/compiler.dir/main.cpp.obj"
	D:\aaaaaaaaaaaa\CE\env\mingw64\bin\g++.exe  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles\compiler.dir\main.cpp.obj -c D:\aaaaaaaaaaaa\CE\compiler\main.cpp

CMakeFiles/compiler.dir/main.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/compiler.dir/main.cpp.i"
	D:\aaaaaaaaaaaa\CE\env\mingw64\bin\g++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E D:\aaaaaaaaaaaa\CE\compiler\main.cpp > CMakeFiles\compiler.dir\main.cpp.i

CMakeFiles/compiler.dir/main.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/compiler.dir/main.cpp.s"
	D:\aaaaaaaaaaaa\CE\env\mingw64\bin\g++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S D:\aaaaaaaaaaaa\CE\compiler\main.cpp -o CMakeFiles\compiler.dir\main.cpp.s

CMakeFiles/compiler.dir/Lexer.cpp.obj: CMakeFiles/compiler.dir/flags.make
CMakeFiles/compiler.dir/Lexer.cpp.obj: ../Lexer.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=D:\aaaaaaaaaaaa\CE\compiler\cmake-build-debug\CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object CMakeFiles/compiler.dir/Lexer.cpp.obj"
	D:\aaaaaaaaaaaa\CE\env\mingw64\bin\g++.exe  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles\compiler.dir\Lexer.cpp.obj -c D:\aaaaaaaaaaaa\CE\compiler\Lexer.cpp

CMakeFiles/compiler.dir/Lexer.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/compiler.dir/Lexer.cpp.i"
	D:\aaaaaaaaaaaa\CE\env\mingw64\bin\g++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E D:\aaaaaaaaaaaa\CE\compiler\Lexer.cpp > CMakeFiles\compiler.dir\Lexer.cpp.i

CMakeFiles/compiler.dir/Lexer.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/compiler.dir/Lexer.cpp.s"
	D:\aaaaaaaaaaaa\CE\env\mingw64\bin\g++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S D:\aaaaaaaaaaaa\CE\compiler\Lexer.cpp -o CMakeFiles\compiler.dir\Lexer.cpp.s

CMakeFiles/compiler.dir/Token.cpp.obj: CMakeFiles/compiler.dir/flags.make
CMakeFiles/compiler.dir/Token.cpp.obj: ../Token.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=D:\aaaaaaaaaaaa\CE\compiler\cmake-build-debug\CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building CXX object CMakeFiles/compiler.dir/Token.cpp.obj"
	D:\aaaaaaaaaaaa\CE\env\mingw64\bin\g++.exe  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles\compiler.dir\Token.cpp.obj -c D:\aaaaaaaaaaaa\CE\compiler\Token.cpp

CMakeFiles/compiler.dir/Token.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/compiler.dir/Token.cpp.i"
	D:\aaaaaaaaaaaa\CE\env\mingw64\bin\g++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E D:\aaaaaaaaaaaa\CE\compiler\Token.cpp > CMakeFiles\compiler.dir\Token.cpp.i

CMakeFiles/compiler.dir/Token.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/compiler.dir/Token.cpp.s"
	D:\aaaaaaaaaaaa\CE\env\mingw64\bin\g++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S D:\aaaaaaaaaaaa\CE\compiler\Token.cpp -o CMakeFiles\compiler.dir\Token.cpp.s

CMakeFiles/compiler.dir/utils.cpp.obj: CMakeFiles/compiler.dir/flags.make
CMakeFiles/compiler.dir/utils.cpp.obj: ../utils.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=D:\aaaaaaaaaaaa\CE\compiler\cmake-build-debug\CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Building CXX object CMakeFiles/compiler.dir/utils.cpp.obj"
	D:\aaaaaaaaaaaa\CE\env\mingw64\bin\g++.exe  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles\compiler.dir\utils.cpp.obj -c D:\aaaaaaaaaaaa\CE\compiler\utils.cpp

CMakeFiles/compiler.dir/utils.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/compiler.dir/utils.cpp.i"
	D:\aaaaaaaaaaaa\CE\env\mingw64\bin\g++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E D:\aaaaaaaaaaaa\CE\compiler\utils.cpp > CMakeFiles\compiler.dir\utils.cpp.i

CMakeFiles/compiler.dir/utils.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/compiler.dir/utils.cpp.s"
	D:\aaaaaaaaaaaa\CE\env\mingw64\bin\g++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S D:\aaaaaaaaaaaa\CE\compiler\utils.cpp -o CMakeFiles\compiler.dir\utils.cpp.s

CMakeFiles/compiler.dir/Parser.cpp.obj: CMakeFiles/compiler.dir/flags.make
CMakeFiles/compiler.dir/Parser.cpp.obj: ../Parser.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=D:\aaaaaaaaaaaa\CE\compiler\cmake-build-debug\CMakeFiles --progress-num=$(CMAKE_PROGRESS_5) "Building CXX object CMakeFiles/compiler.dir/Parser.cpp.obj"
	D:\aaaaaaaaaaaa\CE\env\mingw64\bin\g++.exe  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles\compiler.dir\Parser.cpp.obj -c D:\aaaaaaaaaaaa\CE\compiler\Parser.cpp

CMakeFiles/compiler.dir/Parser.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/compiler.dir/Parser.cpp.i"
	D:\aaaaaaaaaaaa\CE\env\mingw64\bin\g++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E D:\aaaaaaaaaaaa\CE\compiler\Parser.cpp > CMakeFiles\compiler.dir\Parser.cpp.i

CMakeFiles/compiler.dir/Parser.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/compiler.dir/Parser.cpp.s"
	D:\aaaaaaaaaaaa\CE\env\mingw64\bin\g++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S D:\aaaaaaaaaaaa\CE\compiler\Parser.cpp -o CMakeFiles\compiler.dir\Parser.cpp.s

CMakeFiles/compiler.dir/Node.cpp.obj: CMakeFiles/compiler.dir/flags.make
CMakeFiles/compiler.dir/Node.cpp.obj: ../Node.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=D:\aaaaaaaaaaaa\CE\compiler\cmake-build-debug\CMakeFiles --progress-num=$(CMAKE_PROGRESS_6) "Building CXX object CMakeFiles/compiler.dir/Node.cpp.obj"
	D:\aaaaaaaaaaaa\CE\env\mingw64\bin\g++.exe  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles\compiler.dir\Node.cpp.obj -c D:\aaaaaaaaaaaa\CE\compiler\Node.cpp

CMakeFiles/compiler.dir/Node.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/compiler.dir/Node.cpp.i"
	D:\aaaaaaaaaaaa\CE\env\mingw64\bin\g++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E D:\aaaaaaaaaaaa\CE\compiler\Node.cpp > CMakeFiles\compiler.dir\Node.cpp.i

CMakeFiles/compiler.dir/Node.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/compiler.dir/Node.cpp.s"
	D:\aaaaaaaaaaaa\CE\env\mingw64\bin\g++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S D:\aaaaaaaaaaaa\CE\compiler\Node.cpp -o CMakeFiles\compiler.dir\Node.cpp.s

# Object files for target compiler
compiler_OBJECTS = \
"CMakeFiles/compiler.dir/main.cpp.obj" \
"CMakeFiles/compiler.dir/Lexer.cpp.obj" \
"CMakeFiles/compiler.dir/Token.cpp.obj" \
"CMakeFiles/compiler.dir/utils.cpp.obj" \
"CMakeFiles/compiler.dir/Parser.cpp.obj" \
"CMakeFiles/compiler.dir/Node.cpp.obj"

# External object files for target compiler
compiler_EXTERNAL_OBJECTS =

compiler.exe: CMakeFiles/compiler.dir/main.cpp.obj
compiler.exe: CMakeFiles/compiler.dir/Lexer.cpp.obj
compiler.exe: CMakeFiles/compiler.dir/Token.cpp.obj
compiler.exe: CMakeFiles/compiler.dir/utils.cpp.obj
compiler.exe: CMakeFiles/compiler.dir/Parser.cpp.obj
compiler.exe: CMakeFiles/compiler.dir/Node.cpp.obj
compiler.exe: CMakeFiles/compiler.dir/build.make
compiler.exe: CMakeFiles/compiler.dir/linklibs.rsp
compiler.exe: CMakeFiles/compiler.dir/objects1.rsp
compiler.exe: CMakeFiles/compiler.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=D:\aaaaaaaaaaaa\CE\compiler\cmake-build-debug\CMakeFiles --progress-num=$(CMAKE_PROGRESS_7) "Linking CXX executable compiler.exe"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles\compiler.dir\link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/compiler.dir/build: compiler.exe

.PHONY : CMakeFiles/compiler.dir/build

CMakeFiles/compiler.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles\compiler.dir\cmake_clean.cmake
.PHONY : CMakeFiles/compiler.dir/clean

CMakeFiles/compiler.dir/depend:
	$(CMAKE_COMMAND) -E cmake_depends "MinGW Makefiles" D:\aaaaaaaaaaaa\CE\compiler D:\aaaaaaaaaaaa\CE\compiler D:\aaaaaaaaaaaa\CE\compiler\cmake-build-debug D:\aaaaaaaaaaaa\CE\compiler\cmake-build-debug D:\aaaaaaaaaaaa\CE\compiler\cmake-build-debug\CMakeFiles\compiler.dir\DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/compiler.dir/depend


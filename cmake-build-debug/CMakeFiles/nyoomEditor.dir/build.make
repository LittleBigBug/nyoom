# CMAKE generated file: DO NOT EDIT!
# Generated by "NMake Makefiles" Generator, CMake Version 3.16

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:


#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:


.SUFFIXES: .hpux_make_needs_suffix_list


# Suppress display of executed commands.
$(VERBOSE).SILENT:


# A target that is always out of date.
cmake_force:

.PHONY : cmake_force

#=============================================================================
# Set environment variables for the build.

!IF "$(OS)" == "Windows_NT"
NULL=
!ELSE
NULL=nul
!ENDIF
SHELL = cmd.exe

# The CMake executable.
CMAKE_COMMAND = "C:\Program Files\JetBrains\CLion 2020.1.1\bin\cmake\win\bin\cmake.exe"

# The command to remove a file.
RM = "C:\Program Files\JetBrains\CLion 2020.1.1\bin\cmake\win\bin\cmake.exe" -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = "C:\Users\ethan\Documents\Unreal Projects\nyoom"

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = "C:\Users\ethan\Documents\Unreal Projects\nyoom\cmake-build-debug"

# Utility rule file for nyoomEditor.

# Include the progress variables for this target.
include CMakeFiles\nyoomEditor.dir\progress.make

CMakeFiles\nyoomEditor:
	call Y:/UE4/UE_4.24/Engine/Build/BatchFiles/Build.bat nyoomEditor Win64 Development "-project=C:/Users/ethan/Documents/Unreal Projects/nyoom/nyoom.uproject" -game -progress -buildscw

nyoomEditor: CMakeFiles\nyoomEditor
nyoomEditor: CMakeFiles\nyoomEditor.dir\build.make

.PHONY : nyoomEditor

# Rule to build all files generated by this target.
CMakeFiles\nyoomEditor.dir\build: nyoomEditor

.PHONY : CMakeFiles\nyoomEditor.dir\build

CMakeFiles\nyoomEditor.dir\clean:
	$(CMAKE_COMMAND) -P CMakeFiles\nyoomEditor.dir\cmake_clean.cmake
.PHONY : CMakeFiles\nyoomEditor.dir\clean

CMakeFiles\nyoomEditor.dir\depend:
	$(CMAKE_COMMAND) -E cmake_depends "NMake Makefiles" "C:\Users\ethan\Documents\Unreal Projects\nyoom" "C:\Users\ethan\Documents\Unreal Projects\nyoom" "C:\Users\ethan\Documents\Unreal Projects\nyoom\cmake-build-debug" "C:\Users\ethan\Documents\Unreal Projects\nyoom\cmake-build-debug" "C:\Users\ethan\Documents\Unreal Projects\nyoom\cmake-build-debug\CMakeFiles\nyoomEditor.dir\DependInfo.cmake" --color=$(COLOR)
.PHONY : CMakeFiles\nyoomEditor.dir\depend

cmake_minimum_required(VERSION 3.8)
project(CommandToolKit)

set(CMAKE_CXX_STANDARD 17)
set(CTK_PATH ${CMAKE_CURRENT_LIST_DIR})

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})

add_subdirectory(testing)

add_library(CTK SHARED 
	include/ctk/ctkDllApi.h
	src/ctk/impl/ctkManifestImpl.cpp
	src/ctk/ctkInstance.cpp
	src/ctk/impl/interpreter/ctkManifestTokeniser.cpp
	src/ctk/impl/ctkInstanceImpl.cpp
	src/ctk/impl/ctkString.cpp
	src/ctk/ctkResult.cpp
	src/ctk/ctkManifest.cpp
	src/ctk/impl/interpreter/ctkCmdTokeniser.cpp
	src/ctk/ctkGuard.cpp
	src/ctk/impl/ctkGuardWatch.cpp
	src/ctk/ctkIO.cpp)

if (WIN32)
	add_compile_definitions(CTK PRIVATE -D_CRT_SECURE_NO_WARNINGS)
endif()

target_include_directories(CTK PUBLIC include)
target_compile_definitions(CTK PRIVATE -DCTK_EXPORT)
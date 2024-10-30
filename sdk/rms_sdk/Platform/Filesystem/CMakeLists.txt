cmake_minimum_required(VERSION 3.0)

set(TARGET_NAME platformfilesystem)

PROJECT(${TARGET_NAME} VERSION 1.0.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)

set(SOURCES_FILE
    FileQt.cpp
    FileSystemQt.cpp)

set(HEAD_FILE
    IFile.h
    IFileSystem.h
    FileQt.h
    FileSystemQt.h)

add_library(${TARGET_NAME} STATIC ${SOURCE_FILE} ${HEAD_FILE})
set_target_properties(${TARGET_NAME} PROPERTIES LINKER_LANGUAGE CXX)

set(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR})

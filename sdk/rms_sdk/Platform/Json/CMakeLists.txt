cmake_minimum_required(VERSION 3.0)

set(TARGET_NAME platformjson)

PROJECT(${TARGET_NAME} VERSION 1.0.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)

set(SOURCES_FILE
    JsonArrayQt.cpp
    JsonObjectQt.cpp
    JsonParserQt.cpp)

set(HEAD_FILE
    IJsonArray.h
    IJsonObject.h
    JsonArrayQt.h
    JsonObjectQt.h
    JsonParserQt.h
    IJsonParser.h)

add_library(${TARGET_NAME} STATIC ${SOURCE_FILE} ${HEAD_FILE})
set_target_properties(${TARGET_NAME} PROPERTIES LINKER_LANGUAGE CXX)

set(LIBRARY_OUTPUT_PATH ${CMAKE_BINARY_DIR})

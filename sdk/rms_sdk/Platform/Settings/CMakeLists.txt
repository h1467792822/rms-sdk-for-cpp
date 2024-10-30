cmake_minimum_required(VERSION 3.0)

set(TARGET_NAME plaformsettings)
PROJECT(${TARGET_NAME} VERSION 1.0.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)

set(SOURCES_FILE
    LanguageSettings.cpp
    LocalSettingsQt.cpp
    IRMSEnvironmentImpl.cpp)

set(HEAD_FILE
    ILocalSettings.h
    ILanguageSettings.h
    LanguageSettings.h
    LocalSettingsQt.h
    IRMSEnvironmentImpl.h)

add_library(${TARGET_NAME} STATIC ${SOURCE_FILE} ${HEAD_FILE})
set_target_properties(${TARGET_NAME} PROPERTIES LINKER_LANGUAGE CXX)
set(LIBRARY_OUTPUT_PATH ${CMAKE_BINARY_DIR})

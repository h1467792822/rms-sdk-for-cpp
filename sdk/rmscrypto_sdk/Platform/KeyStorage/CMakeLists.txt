cmake_minimum_required(VERSION 3.0)

set(TARGET_NAME platformkeystorage)
PROJECT(${TARGET_NAME} VERSION 1.0.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CRYPTO_PLATFORM_DIR ${CMAKE_BINARY_DIR})

include_directories(/usr/include/libsecret-1/)
link_directories(${CRYPTO_PLATFORM_DIR})

find_package(PkgConfig REQUIRED)
pkg_check_modules(GLIB REQUIRED glib-2.0)
include_directories(${GLIB_INCLUDE_DIRS})
link_directories(${GLIB_LIBRARY_DIRS})
message("GLIB_INCLUDE_DIRS: ${GLIB_INCLUDE_DIRS}, GLIB_LIBRARY_DIRS: ${GLIB_LIBRARY_DIRS}, GLIB_LIBRARY: ${GLIB_LIBRARIES}")

set(SOURCES_FILE
    KeyStoragePosix.h)

set(HEAD_FILE
    KeyStoragePosix.cpp)

add_library(${TARGET_NAME} STATIC ${SOURCE_FILE} ${HEAD_FILE})

target_link_libraries(${TARGET_NAME} ${GLIB_LIBRARIES})

set_target_properties(${TARGET_NAME} PROPERTIES LINKER_LANGUAGE CXX)
set(LIBRARY_OUTPUT_PATH ${CMAKE_BINARY_DIR})

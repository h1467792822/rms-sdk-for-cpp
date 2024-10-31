cmake_minimum_required(VERSION 3.0)

set(TARGET_NAME platformkeystorage)
PROJECT(${TARGET_NAME} VERSION 1.0.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
# message(STATUS "${TARGET_NAME} LIBRARY_OUTPUT_PATH: ${ROOT_BIN_CRYPTO_PLATFORM_DIR}")
set(LIBRARY_OUTPUT_PATH ${ROOT_BIN_CRYPTO_PLATFORM_DIR})

include_directories(/usr/include/libsecret-1/)
link_directories(${ROOT_BIN_CRYPTO_PLATFORM_DIR})

find_package(PkgConfig REQUIRED)
pkg_check_modules(GLIB REQUIRED glib-2.0)
include_directories(${GLIB_INCLUDE_DIRS})
link_directories(${GLIB_LIBRARY_DIRS})
# message(STATUS "GLIB_INCLUDE_DIRS: ${GLIB_INCLUDE_DIRS}, GLIB_LIBRARY_DIRS: ${GLIB_LIBRARY_DIRS}, GLIB_LIBRARY: ${GLIB_LIBRARIES}")

set(SOURCES_FILE
    KeyStoragePosix.cpp)

set(HEAD_FILE
    KeyStoragePosix.h)

add_library(${TARGET_NAME} STATIC ${SOURCES_FILE} ${HEAD_FILE})

target_link_libraries(${TARGET_NAME} ${GLIB_LIBRARIES})

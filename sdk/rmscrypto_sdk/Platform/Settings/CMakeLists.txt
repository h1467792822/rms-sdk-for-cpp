cmake_minimum_required(VERSION 3.0)
set(CMAKE_CXX_STANDARD 17)

set(TARGET_NAME platformsettings)
PROJECT(${TARGET_NAME} VERSION 1.0.0.0 LANGUAGES CXX)

# message(STATUS "${TARGET_NAME} LIBRARY_OUTPUT_PATH: ${ROOT_BIN_CRYPTO_PLATFORM_DIR}")
set(LIBRARY_OUTPUT_PATH ${ROOT_BIN_CRYPTO_PLATFORM_DIR})

set(SOURCES_FILE
    IRMSCryptoEnvironmentImpl.cpp)

set(HEAD_FILE
    IRMSCryptoEnvironmentImpl.h)

add_library(${TARGET_NAME} STATIC ${SOURCES_FILE} ${HEAD_FILE})
target_compile_definitions()
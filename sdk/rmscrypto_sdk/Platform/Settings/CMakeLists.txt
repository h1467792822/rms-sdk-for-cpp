cmake_minimum_required(VERSION 3.0)
project(cryptoplatformsettings)

set(CMAKE_CXX_STANDARD 17)

set(TARGET_NAME cryptoplatformsettings)
PROJECT(${TARGET_NAME} VERSION 1.0.0.0 LANGUAGES CXX)
set(LIBRARY_OUTPUT_DIRECTORY ${ROOT_BIN_CRYPTO_PLATFORM_DIR})
message(STATUS "${TARGET_NAME} LIBRARY_OUTPUT_PATH: ${ROOT_BIN_CRYPTO_PLATFORM_DIR}")

set(SOURCES_FILE
    IRMSCryptoEnvironmentImpl.cpp)

set(HEAD_FILE
    IRMSCryptoEnvironmentImpl.h)

add_library(${TARGET_NAME} STATIC ${SOURCES_FILE} ${HEAD_FILE})



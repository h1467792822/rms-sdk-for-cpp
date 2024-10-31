cmake_minimum_required(VERSION 3.0)

set(TARGET_NAME modcrypto)
PROJECT(${TARGET_NAME} VERSION 1.0.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)

# message(STATUS "${TARGET_NAME} LIBRARY_OUTPUT_PATH: ${ROOT_BIN_CRYPTO_DIR}")
set(LIBRARY_OUTPUT_PATH ${ROOT_BIN_CRYPTO_DIR})

link_directories(${ROOT_BIN_CRYPTO_PLATFORM_DIR})

set(SOURCES_FILE
    Cbc4kCryptoProvider.cpp
    Cbc512NoPaddingCryptoProvider.cpp
    EcbCryptoProvider.cpp)

set(HEAD_FILE
    Cbc4kCryptoProvider.h
    Cbc512NoPaddingCryptoProvider.h
    EcbCryptoProvider.h
    CryptoConstants.h)

add_library(${TARGET_NAME} STATIC ${SOURCES_FILE} ${HEAD_FILE})

target_link_libraries(${TARGET_NAME} platformcrypto)


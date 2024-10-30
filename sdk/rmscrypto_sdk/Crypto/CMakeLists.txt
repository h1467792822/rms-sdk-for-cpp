cmake_minimum_required(VERSION 3.0)

set(TARGET_NAME modcrypto)
PROJECT(${TARGET_NAME} VERSION 1.0.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)

set(SOURCES_FILE
    Cbc4kCryptoProvider.cpp
    Cbc512NoPaddingCryptoProvider.cpp
    EcbCryptoProvider.cpp)

set(HEAD_FILE
    Cbc4kCryptoProvider.h
    Cbc512NoPaddingCryptoProvider.h
    EcbCryptoProvider.h
    CryptoConstants.h)

add_library(${TARGET_NAME} STATIC ${SOURCE_FILE} ${HEAD_FILE})

target_link_libraries(${TARGET_NAME} platformcrypto)

set_target_properties(${TARGET_NAME} PROPERTIES LINKER_LANGUAGE CXX)
set(LIBRARY_OUTPUT_PATH ${CMAKE_BINARY_DIR})

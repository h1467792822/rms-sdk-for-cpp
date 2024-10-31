cmake_minimum_required(VERSION 3.0)

set(TARGET_NAME rmscrypto)
PROJECT(${TARGET_NAME} VERSION 1.0.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)

# message(STATUS "${TARGET_NAME} LIBRARY_OUTPUT_PATH: ${ROOT_BIN_DIR}")
set(LIBRARY_OUTPUT_PATH ${ROOT_BIN_DIR})

add_definitions(-DRMS_CRYPTO_LIBRARY )
include_directories(
    /usr/include/glib-2.0/
    /usr/include/libsecret-1/
    /usr/lib/x86_64-linux-gnu/glib-2.0/include/
)
link_directories(${ROOT_BIN_CRYPTO_DIR} ${ROOT_BIN_CRYPTO_PLATFORM_DIR})

set(SOURCES_FILE
    BlockBasedProtectedStream.cpp
    CachedBlock.cpp
    SimpleProtectedStream.cpp
    CryptoAPI.cpp
    StdStreamAdapter.cpp
    IRMSCryptoEnvironment.cpp)

set(HEAD_FILE
    BlockBasedProtectedStream.h
    CachedBlock.h
    IStream.h
    SimpleProtectedStream.h
    ICryptoStream.h
    CryptoAPI.h
    StdStreamAdapter.h
    ICryptoProvider.h
    ICryptoEngine.h
    ICryptoHash.h
    ICryptoKey.h
    CryptoAPIExport.h
    RMSCryptoExceptions.h
    IRMSCryptoEnvironment.h)

add_library(${TARGET_NAME} SHARED ${SOURCES_FILE} ${HEAD_FILE})

target_link_libraries(${TARGET_NAME}
PUBLIC
    modcrypto
    platformkeystorage
    platformcrypto
    platformsettings
    ssl
    crypto
    secret-1
    glib-2.0)

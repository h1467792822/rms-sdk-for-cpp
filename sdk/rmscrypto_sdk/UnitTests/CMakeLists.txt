cmake_minimum_required(VERSION 3.10)

project(rmscryptoUnitTests)

find_package(Qt5 COMPONENTS Core Test Widgets REQUIRED)

set(EXECUTABLE_OUTPUT_PATH ${ROOT_BIN_TEST_DIR})

add_executable(${PROJECT_NAME})

# add source files
target_sources(${PROJECT_NAME} 
PRIVATE
    main.cpp
    PlatformCryptoTest.cpp
    KeyStorageTests.cpp
    CryptedStreamTests.cpp
    CryptoAPITests.cpp)

#add head files
target_sources(${PROJECT_NAME} 
PRIVATE
    KeyStorageTests.h
    PlatformCryptoTest.h
    CryptedStreamTests.h
    TestHelpers.h
    CryptoAPITests.h)

# include_directories(
#     /usr/include/glib-2.0
#     /usr/include/libsecret-1
#     /usr/lib/x86_64-linux-gnu/glib-2.0/include)

target_include_directories(${PROJECT_NAME} PRIVATE
    ${ROOT_DIR}/sdk/rmscrypto_sdk/CryptoAPI)

target_link_directories(${PROJECT_NAME} PRIVATE 
    ${ROOT_DIR}/bin
    ${ROOT_DIR}/bin/crypto
    ${ROOT_DIR}/bin/crypto/platform)

target_link_libraries(${PROJECT_NAME} PRIVATE
    modcrypto
    platformkeystorage
    platformcrypto
    cryptoplatformsettings
    rmscrypto

    crypto
    secret-1
    glib-2.0
)

target_link_libraries(${PROJECT_NAME} PRIVATE
    Qt::Core
    Qt::Test
    Qt::Widgets
)

set_target_properties(${PROJECT_NAME} PROPERTIES AUTOMOC ON)
set_target_properties(${PROJECT_NAME} PROPERTIES AUTOUIC ON)
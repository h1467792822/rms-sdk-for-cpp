cmake_minimum_required(VERSION 3.10)

project(RestClientsUnitTests)
set(CMAKE_CXX_STANDARD 17)

# 设置相对路径
set(REPO_ROOT ${CMAKE_CURRENT_SOURCE_DIR}/../../../..)
message(STATUS "project ${PROJECT_NAME} REPO_ROOT: ${REPO_ROOT}")

find_package(Qt5 COMPONENTS Core Test Network Xml REQUIRED)

add_definitions(-DSRCDIR="${CMAKE_CURRENT_SOURCE_DIR}/")

set(EXECUTABLE_OUTPUT_PATH ${REPO_ROOT}/bin/tests)

add_executable(${PROJECT_NAME})

# add source files
target_sources(${PROJECT_NAME} 
PRIVATE
    main.cpp
    LicenseParserTest.cpp
    LicenseParserTestConstants.cpp)

#add head files
target_sources(${PROJECT_NAME} 
PRIVATE
    LicenseParserTest.h
    LicenseParserTestConstants.h)

target_include_directories(${PROJECT_NAME} 
PRIVATE
    ${REPO_ROOT}/sdk/rmscrypto_sdk/CryptoAPI
    /usr/include/libxml2)

target_link_directories(${PROJECT_NAME} 
PRIVATE 
    ${REPO_ROOT}/bin
    ${REPO_ROOT}/bin/crypto
    ${REPO_ROOT}/bin/crypto/platform)

target_link_libraries(${PROJECT_NAME}
PRIVATE
    platformhttp
    platformjson
    platformfilesystem
    platformsettings

    modprotectedfile
    modcore
    modrestclients
    modconsent
    modcommon
    modjson

    rmscrypto
    rms

    ssl
    crypto
    xml2
    glib-2.0
)

target_link_libraries(${PROJECT_NAME} 
PRIVATE
    Qt::Core
    Qt::Test
    Qt::Network
    Qt::Xml
)

set_target_properties(${PROJECT_NAME} PROPERTIES AUTOMOC ON)
set_target_properties(${PROJECT_NAME} PROPERTIES AUTOUIC ON)
cmake_minimum_required(VERSION 3.10)

project(rmsauthUnitTests)
# 设置相对路径
set(REPO_ROOT ${CMAKE_CURRENT_SOURCE_DIR}/../../..)

find_package(Qt5 COMPONENTS Core Test Widgets REQUIRED)

add_definitions(-DSRCDIR="${CMAKE_CURRENT_SOURCE_DIR}/")
add_definitions(-DQ_COMPILER_INITIALIZER_LISTS )

set(EXECUTABLE_OUTPUT_PATH ${REPO_ROOT}/bin/tests)

add_executable(${PROJECT_NAME})

# add source files
target_sources(${PROJECT_NAME} 
PRIVATE
    main.cpp
    NonInteractiveTests.cpp
    InteractiveTests.cpp
    UrlQtTests.cpp)

#add head files
target_sources(${PROJECT_NAME} 
PRIVATE
    NonInteractiveTests.h
    InteractiveTests.h
    UrlQtTests.h)

target_include_directories(${PROJECT_NAME} 
PRIVATE
    ${REPO_ROOT}/sdk/rmsauth_sdk/WebAuthDialog
    ${REPO_ROOT}/sdk/rmsauth_sdk/rmsauth/rmsauth)

target_link_directories(${PROJECT_NAME} 
PRIVATE 
    ${REPO_ROOT}/bin
    ${REPO_ROOT}/bin/crypto
    ${REPO_ROOT}/bin/crypto/platform)

target_link_libraries(${PROJECT_NAME}
PRIVATE
    rmsauthWebAuthDialog
    rmscrypto
    rmsauth
    curl

    ssl
    crypto
    secret-1
    glib-2.0
)

target_link_libraries(${PROJECT_NAME} 
PRIVATE
    Qt::Core
    Qt::Test
    Qt::Widgets
)

set_target_properties(${PROJECT_NAME} PROPERTIES AUTOMOC ON)
set_target_properties(${PROJECT_NAME} PROPERTIES AUTOUIC ON)
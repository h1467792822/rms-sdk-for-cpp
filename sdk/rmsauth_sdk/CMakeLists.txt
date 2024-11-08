cmake_minimum_required(VERSION 3.10)
project(rmsauth_sdk)

# 查找 Qt qmake
find_program(QMAKE_EXECUTABLE NAMES qmake HINTS ${QTDIR} PATH_SUFFIXES bin)
if(NOT QMAKE_EXECUTABLE)
    message(FATAL_ERROR "找不到qmake，请确保Qt正确安装并设置QTDIR环境变量")
endif()

# 使用qmake编译WebAuthDialog
add_custom_target(WebAuthDialog
    COMMAND ${QMAKE_EXECUTABLE} WebAuthDialog.pro
    COMMAND make
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/WebAuthDialog
)

option(USE_QT_WEBUI "use QT web get access token" OFF)
remove_definitions(-DUSE_QT_WEBUI)
if (USE_QT_WEBUI)
    add_definitions(-DUSE_QT_WEBUI)
endif()

# 添加 rmsauth 子目录
add_subdirectory(rmsauth)
add_dependencies(rmsauth WebAuthDialog)

if (MAKE_UT_TEST)
    message(STATUS "${PROJECT_NAME} make UT")
    add_subdirectory(UnitTests)
endif()
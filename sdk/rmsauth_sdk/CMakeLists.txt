# cmake_minimum_required(VERSION 3.10)
# project(rmsauth_sdk)

# # 只添加 rmsauth 子目录
# add_subdirectory(rmsauth)


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

# 添加 rmsauth 子目录
add_subdirectory(rmsauth)
add_dependencies(rmsauth WebAuthDialog)
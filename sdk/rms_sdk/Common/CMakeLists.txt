cmake_minimum_required(VERSION 3.14)
project(modcommon)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)
# 设置 C++ 标准
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
# 设置 REPO_ROOT (相对于当前 CMakeLists.txt 的位置)
set(REPO_ROOT ${CMAKE_CURRENT_SOURCE_DIR}/../../..)

# 设置源文件和头文件
set(SOURCES
    tools.cpp
)

set(HEADERS
    CommonTypes.h
    FrameworkSpecificTypes.h
    tools.h
)

# 创建静态库
add_library(${PROJECT_NAME} STATIC
    ${SOURCES}
    ${HEADERS}
)

# 设置输出目录
set_target_properties(${PROJECT_NAME} PROPERTIES
    ARCHIVE_OUTPUT_DIRECTORY "${REPO_ROOT}/bin/rms"
)

# Debug 模式下添加后缀 'd'
set_target_properties(${PROJECT_NAME} PROPERTIES 
    DEBUG_POSTFIX "d"
)

# 启用警告

target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wextra)

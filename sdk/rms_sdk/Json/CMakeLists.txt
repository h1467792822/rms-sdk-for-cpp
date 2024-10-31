cmake_minimum_required(VERSION 3.10)

# 设置项目名称
project(modjson)
set(REPO_ROOT ${CMAKE_CURRENT_SOURCE_DIR}/../../..)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

# 设置 C++ 标准
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Debug 模式下添加 'd' 后缀
set(CMAKE_DEBUG_POSTFIX "d")

# 添加静态库
add_library(${PROJECT_NAME} STATIC
    jsonserializer.cpp
    IJsonSerializer.h
    jsonserializer.h
)

# 设置输出目录
set_target_properties(${PROJECT_NAME} PROPERTIES
    ARCHIVE_OUTPUT_DIRECTORY "${REPO_ROOT}/bin/rms"
)

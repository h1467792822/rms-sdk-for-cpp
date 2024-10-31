# 设置 CMake 最低版本要求
cmake_minimum_required(VERSION 3.14)

# 项目名称
project(modcore)
# 设置 REPO_ROOT (相对于当前 CMakeLists.txt 的位置)
set(REPO_ROOT ${CMAKE_CURRENT_SOURCE_DIR}/../../..)
# 设置 C++ 标准
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

# 设置输出目录
# set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/../../../bin/rms)
# set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/../../../bin/rms)
# 设置输出目录

# set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${REPO_ROOT}/bin/rms)


# Debug 模式下添加 'd' 后缀
set(CMAKE_DEBUG_POSTFIX "d")

# 源文件和头文件
set(SOURCES
    ProtectionPolicy.cpp
)

set(HEADERS
    ProtectionPolicy.h
    FeatureControl.h
)

# 创建静态库
add_library(${PROJECT_NAME} STATIC
    ${SOURCES}
    ${HEADERS}
)

# 设置包含目录
# 添加包含目录
target_include_directories(${PROJECT_NAME} PRIVATE
    ${REPO_ROOT}/sdk/rmscrypto_sdk/CryptoAPI
)

set_target_properties(${PROJECT_NAME} PROPERTIES
    ARCHIVE_OUTPUT_DIRECTORY "${REPO_ROOT}/bin/rms"
)

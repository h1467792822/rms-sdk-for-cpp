# 设置 REPO_ROOT (相对于当前 CMakeLists.txt 的位置)
set(REPO_ROOT ${CMAKE_CURRENT_SOURCE_DIR}/../../..)

set(CMAKE_POSITION_INDEPENDENT_CODE ON)


# 设置项目名和目标
project(modprotectedfile)
add_library(${PROJECT_NAME} STATIC)

# 设置 C++ 标准
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 设置编译选项
if(MSVC)
    target_compile_options(${PROJECT_NAME} PRIVATE /W4)
endif()

# 添加源文件
target_sources(${PROJECT_NAME} PRIVATE
    PfileHeader.cpp
    PfileHeaderReader.cpp
    PfileHeaderWriter.cpp
)

# 添加头文件
target_sources(${PROJECT_NAME} PRIVATE
    IPfileHeaderReader.h
    IPfileHeaderWriter.h
    PfileHeader.h
    PfileHeaderReader.h
    PfileHeaderWriter.h
)

# 添加包含目录
target_include_directories(${PROJECT_NAME} PRIVATE
    ${REPO_ROOT}/sdk/rmscrypto_sdk/CryptoAPI
)

# Debug 配置下添加后缀 'd'
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    set_target_properties(${PROJECT_NAME} PROPERTIES DEBUG_POSTFIX "d")
endif()

# 设置输出目录
set_target_properties(${PROJECT_NAME} PROPERTIES
    ARCHIVE_OUTPUT_DIRECTORY "${REPO_ROOT}/bin/rms"
)


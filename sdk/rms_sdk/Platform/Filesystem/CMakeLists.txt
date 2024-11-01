# 设置 REPO_ROOT
set(REPO_ROOT ${CMAKE_CURRENT_SOURCE_DIR}/../../../..)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)



# 设置项目名和目标
project(platformfilesystem)
# 设置 C++ 标准
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
add_library(${PROJECT_NAME} STATIC)



# 添加包含目录
target_include_directories(${PROJECT_NAME} PRIVATE
    ${REPO_ROOT}/sdk/rmscrypto_sdk/CryptoAPI
)

# 添加源文件
target_sources(${PROJECT_NAME} PRIVATE
    FileQt.cpp
    FileSystemQt.cpp
)

# 添加头文件
target_sources(${PROJECT_NAME} PRIVATE
    IFile.h
    IFileSystem.h
    FileQt.h
    FileSystemQt.h
)

# Debug 配置下添加后缀 'd'
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    set_target_properties(${PROJECT_NAME} PROPERTIES DEBUG_POSTFIX "d")
endif()

# 设置输出目录
set_target_properties(${PROJECT_NAME} PROPERTIES
    ARCHIVE_OUTPUT_DIRECTORY "${REPO_ROOT}/bin/rms"
)
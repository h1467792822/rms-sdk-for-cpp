# 设置 REPO_ROOT
set(REPO_ROOT ${CMAKE_CURRENT_SOURCE_DIR}/../../../..)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

# 设置输出目录
#set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${REPO_ROOT}/bin/rms/platform)

# 设置项目名和目标
project(platformjson)
add_library(${PROJECT_NAME} STATIC)

# 设置 C++ 标准
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 设置编译选项
if(MSVC)
    target_compile_options(${PROJECT_NAME} PRIVATE /W3)
endif()

# 添加源文件
target_sources(${PROJECT_NAME} PRIVATE
    JsonArrayQt.cpp
    JsonObjectQt.cpp
    JsonParserQt.cpp
)

# 添加头文件
target_sources(${PROJECT_NAME} PRIVATE
    IJsonArray.h
    IJsonObject.h
    JsonArrayQt.h
    JsonObjectQt.h
    JsonParserQt.h
    IJsonParser.h
)

# 添加编译定义
# target_compile_definitions(${PROJECT_NAME} PRIVATE QTFRAMEWORK)

# Debug 配置下添加后缀 'd'
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    set_target_properties(${PROJECT_NAME} PROPERTIES DEBUG_POSTFIX "d")
endif()

# 设置输出目录
set_target_properties(${PROJECT_NAME} PROPERTIES
    ARCHIVE_OUTPUT_DIRECTORY "${REPO_ROOT}/bin/rms"
)
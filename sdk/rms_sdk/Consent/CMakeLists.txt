cmake_minimum_required(VERSION 3.10)

# 设置项目名称和版本
project(modconsent VERSION 1.0)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)
# 设置 REPO_ROOT (相对于当前 CMakeLists.txt 的位置)
set(REPO_ROOT ${CMAKE_CURRENT_SOURCE_DIR}/../../..)
# 设置 C++ 标准
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# 设置构建类型
if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Debug)
endif()

# 添加源文件
set(SOURCES
    IConsentManager.cpp
    DocumentTrackingConsentManager.cpp
    ServiceUrlConsentManager.cpp
    ServiceUrlConsent.cpp
    DocumentTrackingConsent.cpp
    ConsentDBHelper.cpp
)

# 添加头文件
set(HEADERS
    IConsentManager.h
    DocumentTrackingConsentManager.h
    ServiceUrlConsentManager.h
    ServiceUrlConsent.h
    DocumentTrackingConsent.h
    Consent.h
    ConsentDBHelper.h
)

# 添加静态库目标
add_library(${PROJECT_NAME} STATIC ${SOURCES} ${HEADERS})

# 设置输出目录
set_target_properties(${PROJECT_NAME} PROPERTIES
    ARCHIVE_OUTPUT_DIRECTORY "${REPO_ROOT}/bin/rms"
)

# 添加编译选项
target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wextra -Werror)

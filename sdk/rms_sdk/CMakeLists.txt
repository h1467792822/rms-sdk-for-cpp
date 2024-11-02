cmake_minimum_required(VERSION 3.10)
project(rms_sdk)

# 设置一些通用变量
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 按依赖顺序添加子目录
add_subdirectory(Platform)    # 最基础的依赖
add_subdirectory(Common)      # 依赖 Platform
add_subdirectory(Json)
add_subdirectory(Core)
add_subdirectory(Consent)
add_subdirectory(PFile)
add_subdirectory(RestClients) # 依赖 PFile
add_subdirectory(ModernAPI)   # 依赖其他所有模块

if (MAKE_UT_TEST)
    message(STATUS "${PROJECT_NAME} make UT")
    add_subdirectory(UnitTests)
endif()
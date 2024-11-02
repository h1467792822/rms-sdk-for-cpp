cmake_minimum_required(VERSION 3.0)

# 是否编译UT，UT会使用QT库
option(MAKE_UT_TEST "compile test projects" OFF)

add_subdirectory(rmscrypto_sdk)
add_subdirectory(rmsauth_sdk)
add_subdirectory(rms_sdk)
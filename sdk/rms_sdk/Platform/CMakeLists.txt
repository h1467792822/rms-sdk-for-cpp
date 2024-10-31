cmake_minimum_required(VERSION 3.10)
project(Platform)

# 定义 QTFRAMEWORK 宏
# add_definitions(-DQTFRAMEWORK)

# 添加所有子目录
add_subdirectory(Http)
# add_subdirectory(Common)
add_subdirectory(Filesystem)
add_subdirectory(Json)
add_subdirectory(Settings)
# add_subdirectory(Xml)

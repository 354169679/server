# CMake 最低版本号要求
cmake_minimum_required (VERSION 3.22)

# 项目信息
project (qqq)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 指定生成目标

add_executable(server server.cpp)

add_executable(client client.cpp)




find_package(PkgConfig REQUIRED)

target_link_libraries(server)

#target_link_libraries(a.out PkgConfig::libnetlink)

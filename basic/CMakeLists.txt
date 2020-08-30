cmake_minimum_required(VERSION 3.14)
# 可以指定工程采用的语言([C] [CXX] [java])，选项分别表示：C++, C, java， 如不指定默认支持所有语言
project(basic C CXX)

set(CMAKE_C_FLAGS  "${CMAKE_C_FLAGS} --std=c99")
set(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} -std=c++11")

# 包含以下源码目录
include_directories(src/)

# 递归目录中符合条件的文件
file(GLOB_RECURSE SRCS "src/*.h" "src/*.c" "src/*.cpp")
file(GLOB_RECURSE MAIN main.h main.c main.cpp)
# 打印日志信息
message("src files: ${SRCS}")

# 可执行文件
add_executable(basic ${MAIN} ${SRCS})
cmake_minimum_required(VERSION 3.14)
project(basic C)

set(CMAKE_C_STANDARD 99)

# 包含以下源码目录
include_directories(src/main/)

# 递归目录中符合条件的文件
file(GLOB_RECURSE SRC_RESOURCES "src/*.h" "src/*.c")
# 打印日志信息
message("src files: ${SRC_RESOURCES}")

# 可执行文件
add_executable(basic main.c ${SRC_RESOURCES})
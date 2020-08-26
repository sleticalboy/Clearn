cmake_minimum_required(VERSION 3.14)
project(basic C)

set(CMAKE_C_STANDARD 99)

# 包含以下源码目录
include_directories(data_type/)
include_directories(str/)

# 递归目录中符合条件的文件
file(GLOB_RECURSE DATA_TYPE_RESOURCES "data_type/*.h" "data_type/*.c")
file(GLOB_RECURSE STR_RESOURCES "str/*.h" "str/*.c")

# 可执行文件
add_executable(basic main.c ${DATA_TYPE_RESOURCES} ${STR_RESOURCES})
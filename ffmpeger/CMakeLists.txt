cmake_minimum_required(VERSION 3.14)
# 可以指定工程采用的语言([C] [CXX] [java])，选项分别表示：C++, C, java， 如不指定默认支持所有语言
project(ffmpegger C CXX)

# 指定编译器
set(CMAKE_C_COMPILER "/usr/bin/clang")
set(CMAKE_CXX_COMPILER "/usr/bin/clang++")

# original is --std=c99, and now we use --std=gnu99 for usleep()
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} --std=gnu99 -pthread")
# -pthread for undefined reference to `pthread_create`
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -pthread -fPIC -stdlib=libc++ -fexceptions")

# 包含以下源码目录
include_directories(
        ${CMAKE_SOURCE_DIR}/include
        ${CMAKE_SOURCE_DIR}/src
        /home/binlee/bin
)

link_directories(
        /usr/lib/x86_64-linux-gnu/
        /usr/lib/
#        /home/binlee/bin/lib/
)

# 递归目录中符合条件的文件
file(GLOB_RECURSE SRCS "src/*.h" "src/*.c" "src/*.cpp")
file(GLOB_RECURSE MAIN src/c/main.h src/c/main.c src/cpp/main.cpp)
# 打印日志信息
message("src files: ${SRCS}")

# 可执行文件 ffmpeger
add_executable(ffmpeger
        src/ffmpeger-main.cpp
        )
# 链接动态库
target_link_libraries(ffmpeger avformat)


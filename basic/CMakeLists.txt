cmake_minimum_required(VERSION 3.14)
project(basic C)

set(CMAKE_C_STANDARD 99)

add_executable(basic main.c str/str_usage.h str/str_usage.c)
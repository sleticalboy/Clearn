//
// Created by binlee on 2020/8/31.
//

#include <iostream>
#include <string>
#include "cpp_string.h"

void cpp_string() {
    std::string hello = "Hello ";
    std::string world = "World";
    std::string combine;

    // 复制 hello 到 combine
    combine = hello;
    std::cout << "combine string is: " << combine << std::endl;

    // 连接 Hello 和 world
    combine = hello + world;
    std::cout << "combine string is: " << combine << std::endl;

    // 计算字符串的长度
    const int size = combine.size();
    std::cout << "combine string size is: " << size << std::endl;
    const int length = combine.length();
    std::cout << "combine string length is: " << length << std::endl;

    std::cout << "Null pointer is: " << NULL << std::endl;
}
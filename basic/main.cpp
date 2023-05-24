//
// Created by binlee on 2020/8/30.
//

#include "main.h"
#include "src/cpp/cpp_string.h"
#include "src/cpp/Person.h"

#include <dirent.h>
#include <iostream>
#include <map>
#include <string>
#include <filesystem>

using namespace std;
namespace fs = std::__fs::filesystem;

int cpp_main();

int main() {
    // c 语言
    const int ret = c_main();
    if (ret) return ret;

    // cpp
    std::cout << "\n>>>>>>>Welcome to C++ World!<<<<<<<<" << std::endl;
    return cpp_main();
}

void map_files_0(const string &path, map<long long, string> &file_map) {
    auto map_ttid = [&](const string &name) {
        size_t left = name.find("0x");
        size_t right = name.find(".xyt");
        if (right != string::npos && right > left) {
            long long n = strtoll(name.substr(left, right).c_str(), nullptr, 16);
            file_map.insert(pair<long long, string>(n, name));
        }
    };
    if (fs::is_directory(path)) {
        for (auto &ent: fs::directory_iterator(path)) {
            if (ent.is_regular_file()) {
                map_ttid(ent.path().string());
            }
        }
    } else if (fs::is_regular_file(path)) {
        printf("%s(): %s\n", __func__, path.c_str());
        map_ttid(path);
    }
}

void map_files(const string &path, map<long long, string> &file_map) {
    DIR *dir = opendir(path.c_str());
    struct dirent *f;
    while ((f = readdir(dir)) != nullptr) {
        string name(f->d_name);
        size_t left = name.find("0x");
        size_t right = name.rfind(".xyt");
        if (right != string::npos && right > left) {
            long long n = std::strtoll(name.substr(left, right).c_str(), nullptr, 16);
            file_map.insert(pair<long long, string>(n, string(path) + "/" + f->d_name));
        }
    }
}

int cpp_main() {
    cpp_string();
    person_main();

    string path("/home/binlee/Downloads/xyt");
    auto file_map = map<long long, string>();
    map_files(path, file_map);
    for (const auto &item: file_map) {
        std::printf("c style --->%lld: %s\n", item.first, item.second.c_str());
    }
    file_map.clear();
    map_files_0(path, file_map);
    for (const auto &item: file_map) {
        std::printf("cpp style --->%lld: %s\n", item.first, item.second.c_str());
    }
    return 0;
}

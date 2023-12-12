//
// Created by binlee on 23-12-12.
//

#include <csignal>
#include "string"
#include "iostream"
#include "sys/stat.h"
#include "filesystem"

#include "io_sample.h"

void readFileTest() {
    auto cwd = getcwd(nullptr, 0);
    std::string prj_root = std::__fs::filesystem::path(cwd).parent_path();
    auto test_file = prj_root + "/testdata/results.json";
    // 打开文件
    auto fp = fopen(test_file.data(), "rb");
    struct stat s{};
    stat(test_file.data(), &s);
    char buf[s.st_size];
    fread(buf, sizeof(buf), sizeof(*buf), fp);
    std::cout << "file size: " << s.st_size << " is: " << buf << std::endl;
}

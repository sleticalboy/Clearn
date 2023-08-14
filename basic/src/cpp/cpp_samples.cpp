//
// Created by binlee on 8/14/23.
//

#include <dirent.h>
#include <iostream>
#include <map>
#include <string>
#include <filesystem>
#include <sys/stat.h>
#include <vector>

#include "cpp_samples.h"
#include "cpp_string.h"
#include "Person.h"

namespace fs = std::__fs::filesystem;


bool startsWith(const std::string &s, const std::string &prefix) {
  return s.find(prefix) == 0;
}

bool endsWith(const std::string &s, const std::string &suffix) {
  auto pos = s.rfind(suffix);
  return pos != std::string::npos && s.substr(pos) == suffix;
}

void map_files_0(const std::string &path, std::map<long long, std::string> &file_map) {
  auto map_ttid = [&](const std::string &name) {
    size_t left = name.find("0x");
    size_t right = name.find(".xyt");
    if (right != std::string::npos && right > left) {
      long long n = strtoll(name.substr(left, right).c_str(), nullptr, 16);
      file_map.insert(std::pair<long long, std::string>(n, name));
    }
  };
  if (fs::is_directory(path)) {
    for (auto &ent: fs::directory_iterator(path)) {
      map_files_0(ent.path().string(), file_map);
    }
  } else if (fs::is_regular_file(path)) {
    printf("%s(): %s\n", __func__, path.c_str());
    map_ttid(path);
  }
}

void map_files(const std::string &path, std::map<long long, std::string> &file_map) {
  struct stat sb{};
  if (stat(path.c_str(), &sb) != 0) {
    return;
  }
  if ((sb.st_mode & S_IFDIR) != 0) {
    DIR *dir = opendir(path.c_str());
    struct dirent *f;
    while ((f = readdir(dir)) != nullptr) {
      // 过滤当前目录以及父目录
      if (strcmp(f->d_name, ".") == 0) continue;
      if (strcmp(f->d_name, "..") == 0) continue;
      map_files(path + "/" + f->d_name, file_map);
    }
  } else if ((sb.st_mode & S_IFREG) != 0) {
    size_t left = path.find("0x");
    size_t right = path.rfind(".xyt");
    if (right != std::string::npos && right > left) {
      long long n = std::strtoll(path.substr(left, right).c_str(), nullptr, 16);
      file_map.insert(std::pair<long long, std::string>(n, path));
    }
  } else {
    printf("%s() invalid file: %s\n", __func__, path.c_str());
  }
}

long long parseTemplateId(const std::string &name) {
  if (startsWith(name, "0x")) {
    size_t right;
    if (endsWith(name, ".xyt")) {
      right = name.rfind(".xyt");
    } else if (endsWith(name, ".zip")) {
      right = name.rfind(".zip");
    } else {
      right = name.length() - 1;
    }
    if (right != std::string::npos) {
      return strtoll(name.substr(0, right).c_str(), nullptr, 16);
    }
  } else {
    return strtoll(name.c_str(), nullptr, 10);
  }
  return -1L;
}

std::string getName(const std::string &path) {
  auto pos = path.rfind('/');
  if (pos != std::string::npos) {
    return path.substr(pos + 1);
  }
  return path;
}

std::string getExt(const std::string &path) {
  auto name = getName(path);
  auto pos = name.rfind('.');
  if (pos != std::string::npos) {
    return name.substr(pos + 1);
  }
  return "";
}

void search_files(const std::string &path, std::vector<std::string> &output, const std::string &ext,
                  bool single = true) {

  if (single && !output.empty()) return;

  struct stat sb{};
  if (stat(path.c_str(), &sb) != 0) {
    return;
  }
  if ((sb.st_mode & S_IFDIR) != 0) {
    DIR *dir = opendir(path.c_str());
    struct dirent *f;
    while ((f = readdir(dir)) != nullptr) {
      // 过滤当前目录以及父目录，否则会导致死循环
      if (strcmp(f->d_name, ".") == 0) continue;
      if (strcmp(f->d_name, "..") == 0) continue;
      search_files(path + "/" + f->d_name, output, ext, single);
    }
  } else if ((sb.st_mode & S_IFREG) != 0) {
    if (endsWith(path, ext)) {
      output.push_back(path);
      if (single) return;
    }
  } else {
    printf("%s() invalid file: %s\n", __func__, path.c_str());
  }
}


int cpp_samples() {

  std::cout << "\n>>>>>>>Welcome to C++ World!<<<<<<<<" << std::endl;

  cpp_string();
  person_main();

  std::string path("/home/binlee/Downloads/xyt");

  auto targets = std::vector<std::string>();
  search_files(path, targets, ".xyt", false);
  for (const auto &item: targets) {
    printf("xyt file: %s\n", item.c_str());
  }
  return 0;

  auto file_map = std::map<long long, std::string>();
  map_files(path, file_map);
  for (const auto &item: file_map) {
    std::printf("c style --->%lld: %s\n", item.first, item.second.c_str());
  }
  file_map.clear();
  map_files_0(path, file_map);
  for (const auto &item: file_map) {
    std::printf("cpp style --->%lld: %s\n", item.first, item.second.c_str());
  }
  std::string tester("0x06000000000007CA.xyt");
  printf("%s starts with '0x': %d\n", tester.c_str(), startsWith(tester, "0x"));
  printf("%s ends with '.xyt': %d\n", tester.c_str(), endsWith(tester, ".xyt"));
  tester = "00xddd.xytt";
  printf("%s starts with '0x': %d\n", tester.c_str(), startsWith(tester, "0x"));
  printf("%s ends with '.xyt': %d\n", tester.c_str(), endsWith(tester, ".xyt"));
  tester = "0xddd.xyt.json";
  printf("%s starts with '0x': %d\n", tester.c_str(), startsWith(tester, "0x"));
  printf("%s ends with '.xyt': %d\n", tester.c_str(), endsWith(tester, ".xyt"));
  tester = "0dxddd.xyt";
  printf("%s starts with '0x': %d\n", tester.c_str(), startsWith(tester, "0x"));
  printf("%s ends with '.xyt': %d\n", tester.c_str(), endsWith(tester, ".xyt"));

  printf("'648518346341352029' -> %llx\n", parseTemplateId("648518346341352029"));
  printf("'0400600000001FF1' -> %llx\n", parseTemplateId("0400600000001FF1"));
  printf("'0x0400600000001FF1' -> %llx\n", parseTemplateId("0x0400600000001FF1"));

  // 获取文件名测试
  printf("/home/me/a.txt -> name: %s\n", getName("a.txt").c_str());
  printf("a.jpg -> name: %s\n", getName("a.jpg").c_str());
  printf("/home/me/a.png -> name: %s\n", getName("a.png").c_str());
  // 获取文件后缀测试
  printf("a.txt -> ext: %s\n", getExt("a.txt").c_str());
  printf("a.jpg -> ext: %s\n", getExt("a.jpg").c_str());
  printf("a.png -> ext: %s\n", getExt("a.png").c_str());
  printf("https://www.example.com/home/a.dat -> ext: %s\n",
         getExt("http://www.example.com/home/a.dat").c_str());
  printf("https://www.example.com/home/a.exe -> ext: %s\n",
         getExt("http://www.example.com/home/a.exe").c_str());
  printf("https://www.example.com/home/a.html -> ext: %s\n",
         getExt("http://www.example.com/home/a.html").c_str());
  printf("https://www.example.com/home/a. -> ext: %s\n",
         getExt("http://www.example.com/home/a.").c_str());
  printf("https://www.example.com/home/a -> ext: %s\n",
         getExt("http://www.example.com/home/a").c_str());

  try {
    // throws std::length_error
    std::string("abc").substr(10);
  } catch (const std::exception &e) {
    // reference to the base of a polymorphic object
    printf("catch exception: '%s'\n", e.what());
  }
  return 0;
}


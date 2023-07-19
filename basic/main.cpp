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
#include <sys/stat.h>

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

bool startsWith(const string &s, const string &prefix) {
  return s.find(prefix) == 0;
}

bool endsWith(const string &s, const string &suffix) {
  auto pos = s.rfind(suffix);
  return pos != string::npos && s.substr(pos) == suffix;
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
      map_files_0(ent.path().string(), file_map);
    }
  } else if (fs::is_regular_file(path)) {
    printf("%s(): %s\n", __func__, path.c_str());
    map_ttid(path);
  }
}

void map_files(const string &path, map<long long, string> &file_map) {
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
    if (right != string::npos && right > left) {
      long long n = std::strtoll(path.substr(left, right).c_str(), nullptr, 16);
      file_map.insert(pair<long long, string>(n, path));
    }
  } else {
    printf("%s() invalid file: %s\n", __func__, path.c_str());
  }
}

long long parseTemplateId(const string &name) {
  if (startsWith(name, "0x")) {
    size_t right;
    if (endsWith(name, ".xyt")) {
      right = name.rfind(".xyt");
    } else if (endsWith(name, ".zip")) {
      right = name.rfind(".zip");
    } else {
      right = name.length() - 1;
    }
    if (right != string::npos) {
      return strtoll(name.substr(0, right).c_str(), nullptr, 16);
    }
  } else {
    return strtoll(name.c_str(), nullptr, 10);
  }
  return -1L;
}

string getName(const string &path) {
  auto pos = path.rfind('/');
  if (pos != string::npos) {
    return path.substr(pos + 1);
  }
  return path;
}

string getExt(const string &path) {
  auto name = getName(path);
  auto pos = name.rfind('.');
  if (pos != string::npos) {
    return name.substr(pos + 1);
  }
  return "";
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
  string tester("0x06000000000007CA.xyt");
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
  printf("https://www.example.com/home/a.dat -> ext: %s\n", getExt("http://www.example.com/home/a.dat").c_str());
  printf("https://www.example.com/home/a.exe -> ext: %s\n", getExt("http://www.example.com/home/a.exe").c_str());
  printf("https://www.example.com/home/a.html -> ext: %s\n", getExt("http://www.example.com/home/a.html").c_str());
  printf("https://www.example.com/home/a. -> ext: %s\n", getExt("http://www.example.com/home/a.").c_str());
  printf("https://www.example.com/home/a -> ext: %s\n", getExt("http://www.example.com/home/a").c_str());
  return 0;
}

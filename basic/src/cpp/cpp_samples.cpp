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
#include <csignal>
#include <sys/inotify.h>

#include "cpp_samples.h"

namespace fs = std::__fs::filesystem;


bool startsWith(const std::string &s, const std::string &prefix) {
  return s.find(prefix) == 0;
}

bool endsWith(const std::string &s, const std::string &suffix) {
  auto pos = s.rfind(suffix);
  return pos != std::string::npos && s.substr(pos) == suffix;
}

void traversalDir(const std::string &path, const std::function<void(const std::string &)> &file_handler,
                  const std::function<bool()> &break_handler = []() { return false; },
                  bool recursive = false, int level = 0) {

  if (break_handler != nullptr && break_handler()) return;

  struct stat sb{};
  if (stat(path.c_str(), &sb) != 0) return;

  if ((sb.st_mode & S_IFREG) != 0) {
    // 直接处理文件
    if (file_handler != nullptr) file_handler(path);
    if (break_handler != nullptr && break_handler()) return;
  } else if ((sb.st_mode & S_IFDIR) != 0) {
    // 不需要递归，直接返回
    if (!recursive && level != 0) return;
    // 目录层级递增 1
    level++;
    // 递归目录
    DIR *dir = opendir(path.c_str());
    struct dirent *f;
    while ((f = readdir(dir)) != nullptr) {
      // 过滤当前目录以及父目录，否则会导致死循环
      if (strcmp(f->d_name, ".") == 0 || strcmp(f->d_name, "..") == 0) continue;
      traversalDir(path + "/" + f->d_name, file_handler, break_handler, recursive, level);
    }
  } else {
    printf("%s() invalid file: %s\n", __func__, path.c_str());
  }
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
  traversalDir(path, [&](const std::string &fp) {
    auto left = fp.find("0x");
    auto right = fp.rfind(".xyt");
    if (right != std::string::npos && right > left) {
      long long n = std::strtoll(fp.substr(left, right).c_str(), nullptr, 16);
      file_map.insert(std::pair<long long, std::string>(n, fp));
    }
  }, []() { return false; }, true);
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
  traversalDir(path, [&](const std::string &fp) {
    if (endsWith(fp, ext)) output.push_back(fp);
  }, [&]() {
    return single && !output.empty();
  });
}

void search_test() {
  std::string path("/home/binlee/Downloads/xyt");
  auto targets = std::vector<std::string>();
  search_files(path, targets, ".xyt", false);
  for (const auto &item: targets) {
    printf("xyt file: %s\n", item.c_str());
  }

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
}

void match_test() {
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
}

void parse_test() {
  printf("'648518346341352029' -> %llx\n", parseTemplateId("648518346341352029"));
  printf("'0400600000001FF1' -> %llx\n", parseTemplateId("0400600000001FF1"));
  printf("'0x0400600000001FF1' -> %llx\n", parseTemplateId("0x0400600000001FF1"));
}

void get_file_name_test() {
  // 获取文件名测试
  printf("/home/me/a.txt -> name: %s\n", getName("a.txt").c_str());
  printf("a.jpg -> name: %s\n", getName("a.jpg").c_str());
  printf("/home/me/a.png -> name: %s\n", getName("a.png").c_str());
}

void get_file_ext_test() {
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
}

void try_catch_test() {
  try {
    // throws std::length_error
    std::string("abc").substr(10);
  } catch (const std::exception &e) {
    // reference to the base of a polymorphic object
    printf("catch exception: '%s'\n", e.what());
  }
}

void trim_path_test() {
  auto p = std::string("/tmp/3de1c8ca-c64c-49c5-80cb-9e6d6d0dc75f/0x01000000000022BA/0x47800000000022BA/music.mp3");
  // auto p = std::string("/music.mp3");
  if (getName(p) == "music.mp3") {
    auto pos = p.rfind('/', p.rfind('/') - 1);
    auto n = p.substr(pos + 1);
    printf("absolute path is: %s\n", p.c_str());
    printf("relative path is: %s\n", n.c_str());
  }
}

void pipe_test() {
  auto fd = popen("ls -alh .", "r");
  if (fd == nullptr) {
    std::cerr << "Create pipe error." << std::endl;
  }
  char buf[1024];
  while (fgets(buf, sizeof(buf), fd) != nullptr) {
    std::cout << buf;
  }
  pclose(fd);
}

void file_observer_test() {
  auto cwd = getcwd(nullptr, 0);
  std::string prj_root = fs::path(cwd).parent_path();
  std::cout << "cwd: " << cwd << ", prj root: " << prj_root << std::endl;
  if (cwd != nullptr) {
    free(cwd);
  }

  int fd = inotify_init1(IN_NONBLOCK);
  if (fd == -1) {
    std::cerr << "Failed to initialize fd observer.\n";
  }
  std::cout << "Initialize observer fd: " << fd << std::endl;
  int watched_fd = inotify_add_watch(fd, (prj_root + "/testdata").data(), IN_MODIFY | IN_CREATE);
  if (watched_fd == -1) {
    std::cerr << "Failed to add path to observer.\n";
    close(fd);
    return;
  }
  char buf[128];
  while (true) {
    ssize_t rs = read(fd, buf, sizeof(buf));
    std::cout << "File was written bytes: " << rs << std::endl;
    if (rs > 0) {
      unsigned long offset = 0;
      while (offset < rs) {
        auto event = (inotify_event *) &buf[offset];
        // if (event->wd == watched_fd && event->mask & IN_CLOSE_WRITE) {
        // }
        std::cout << "File was written even: " << event->name << std::endl;
        offset += sizeof(struct inotify_event) + event->len;
      }
    } else {
      std::cerr << "read err, continue.\n";
      // break;
    }
    usleep(1500000);
  }

  // pthread_t observer_t;
  // pthread_create(&observer_t, nullptr, [](void *data) {
  //   int *fd = (int *) data;
  //   std::cout << "observer proc enter with fd: " << *fd << std::endl;
  //   char buf[128];
  //   while (true) {
  //     ssize_t rs = read(*fd, buf, sizeof(buf));
  //     std::cout << "File was written bytes: " << rs << std::endl;
  //     if (rs > 0) {
  //       std::cout << "File was written data: " << buf << std::endl;
  //     } else {
  //       // std::cerr << "read err, continue.\n";
  //       // break;
  //     }
  //   }
  //   return (void *) nullptr;
  // }, &fd);

  // pthread_t cmd_t;
  // pthread_create(&cmd_t, nullptr, [](void *data) {
  //   std::string cmd = std::string("python3 ") + (char *)(data) + "/scripts/write_file.py";
  //   std::cout << "cmd is: " << cmd << std::endl;
  //   int res = system(cmd.data());
  //   std::cout << "cmd exit with: " << res << std::endl;
  //   return (void *) nullptr;
  // }, (void *) prj_root.data());

  // 等待子线程执行完成
  // pthread_join(observer_t, nullptr);
  // pthread_join(cmd_t, nullptr);

  // 关闭文件句柄
  close(fd);
  std::cout << __func__ << "() exit.\n";
}

int cpp_samples() {
  std::cout << "\n>>>>>>>Welcome to C++ World!<<<<<<<<\n" << std::endl;

  // cpp_string();
  // person_main();
  // search_test();
  // match_test();
  // parse_test();
  // get_file_name_test();
  // get_file_ext_test();
  // try_catch_test();
  // trim_path_test();
  // pipe_test();
  file_observer_test();

  return 0;
}


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
#include <sys/epoll.h>
#include <sys/inotify.h>
#include <sstream>
#include <fstream>
#include <thread>

#include "cpp_samples.h"
#include "cpp_string.h"
#include "Person.h"
#include "cmd_runner.h"


// json
#include "nlohmann/json.hpp"

namespace fs = std::__fs::filesystem;

static std::string PIPE_ERR("pipe err");

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

void mapFiles0(const std::string &path, std::map<long long, std::string> &file_map) {
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
      mapFiles0(ent.path().string(), file_map);
    }
  } else if (fs::is_regular_file(path)) {
    printf("%s(): %s\n", __func__, path.c_str());
    map_ttid(path);
  }
}

void mapFiles(const std::string &path, std::map<long long, std::string> &file_map) {
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

void searchFiles(const std::string &path, std::vector<std::string> &output, const std::string &ext,
                  bool single = true) {
  traversalDir(path, [&](const std::string &fp) {
    if (endsWith(fp, ext)) output.push_back(fp);
  }, [&]() {
    return single && !output.empty();
  });
}

void searchTest() {
  std::string path("/home/binlee/Downloads/xyt");
  auto targets = std::vector<std::string>();
  searchFiles(path, targets, ".xyt", false);
  for (const auto &item: targets) {
    printf("xyt file: %s\n", item.c_str());
  }

  auto file_map = std::map<long long, std::string>();
  mapFiles(path, file_map);
  for (const auto &item: file_map) {
    std::printf("c style --->%lld: %s\n", item.first, item.second.c_str());
  }
  file_map.clear();
  mapFiles0(path, file_map);
  for (const auto &item: file_map) {
    std::printf("cpp style --->%lld: %s\n", item.first, item.second.c_str());
  }
}

void matchTest() {
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

void parseTest() {
  printf("'648518346341352029' -> %llx\n", parseTemplateId("648518346341352029"));
  printf("'0400600000001FF1' -> %llx\n", parseTemplateId("0400600000001FF1"));
  printf("'0x0400600000001FF1' -> %llx\n", parseTemplateId("0x0400600000001FF1"));
}

void getFileNameTest() {
  // 获取文件名测试
  printf("/home/me/a.txt -> name: %s\n", getName("a.txt").c_str());
  printf("a.jpg -> name: %s\n", getName("a.jpg").c_str());
  printf("/home/me/a.png -> name: %s\n", getName("a.png").c_str());
}

void getFileExtTest() {
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

void tryCatchTest() {
  try {
    // throws std::length_error
    std::string("abc").substr(10);
  } catch (const std::exception &e) {
    // reference to the base of a polymorphic object
    printf("catch exception: '%s'\n", e.what());
  }
}

void trimPathTest() {
  auto p = std::string("/tmp/3de1c8ca-c64c-49c5-80cb-9e6d6d0dc75f/0x01000000000022BA/0x47800000000022BA/music.mp3");
  // auto p = std::string("/music.mp3");
  if (getName(p) == "music.mp3") {
    auto pos = p.rfind('/', p.rfind('/') - 1);
    auto n = p.substr(pos + 1);
    printf("absolute path is: %s\n", p.c_str());
    printf("relative path is: %s\n", n.c_str());
  }
}

std::pair<int, std::string> pipeTest(const std::string &cmd) {
  auto fd = popen(cmd.c_str(), "r");
  if (fd == nullptr) {
    std::cerr << "Create pipe error." << std::endl;
    return {0, PIPE_ERR};
  }
  std::stringstream ss;
  char buf[1024];
  while (fgets(buf, sizeof(buf), fd) != nullptr) {
    ss << buf;
  }
  // std::cout << ss.str() << std::endl;

  // pclose() 返回该命令执行返回值，0 表示成功，其他表示失败
  int res = pclose(fd);
  std::cout << cmd << " exit with: " << res << std::endl;
  return {res, ss.str()};
}

struct UserData {
  int epoll_fd;
  int inotify_fd;
};

const char *map_epoll_events(uint32_t events) {
  char *buf = new char[1024];
  int offset = 0;
  if (events & EPOLLIN) {
    offset += sprintf(buf + offset, "EPOLLIN|");
  }
  if (events & EPOLLPRI) {
    offset += sprintf(buf + offset, "EPOLLPRI|");
  }
  if (events & EPOLLRDNORM) {
    offset += sprintf(buf + offset, "EPOLLRDNORM|");
  }
  if (events & EPOLLRDBAND) {
    offset += sprintf(buf + offset, "EPOLLRDBAND|");
  }
  if (events & EPOLLWRNORM) {
    offset += sprintf(buf + offset, "EPOLLWRNORM|");
  }
  if (events & EPOLLWRBAND) {
    offset += sprintf(buf + offset, "EPOLLWRBAND|");
  }
  if (events & EPOLLMSG) {
    offset += sprintf(buf + offset, "EPOLLMSG|");
  }
  if (events & EPOLLERR) {
    offset += sprintf(buf + offset, "EPOLLERR|");
  }
  if (events & EPOLLHUP) {
    offset += sprintf(buf + offset, "EPOLLHUP|");
  }
  if (events & EPOLLRDHUP) {
    offset += sprintf(buf + offset, "EPOLLRDHUP|");
  }
  if (events & EPOLLEXCLUSIVE) {
    offset += sprintf(buf + offset, "EPOLLEXCLUSIVE|");
  }
  if (events & EPOLLWAKEUP) {
    offset += sprintf(buf + offset, "EPOLLWAKEUP|");
  }
  if (events & EPOLLONESHOT) {
    offset += sprintf(buf + offset, "EPOLLONESHOT|");
  }
  if (events & EPOLLET) {
    offset += sprintf(buf + offset, "EPOLLET|");
  }
  return buf;
}

void dump_epoll_event(int index, struct epoll_event *e) {
  printf("epoll event[%d] -> events: %s, fd: %d, ptr: %s, u32: %d, u64: %lu\n",
         index, map_epoll_events(e->events), e->data.fd, (char *) e->data.ptr, e->data.u32, e->data.u64);
}

const char *map_inotify_mask(uint32_t mask) {
  char *buf = new char[1024];
  int offset = 0;
  if (mask & IN_ACCESS) {
    offset += sprintf(buf + offset, "IN_ACCESS|");
  }
  if (mask & IN_MODIFY) {
    offset += sprintf(buf + offset, "IN_MODIFY|");
  }
  if (mask & IN_ATTRIB) {
    offset += sprintf(buf + offset, "IN_ATTRIB|");
  }
  if (mask & IN_CLOSE_WRITE) {
    offset += sprintf(buf + offset, "IN_CLOSE_WRITE|");
  }
  if (mask & IN_CLOSE_NOWRITE) {
    offset += sprintf(buf + offset, "IN_CLOSE_NOWRITE|");
  }
  if (mask & IN_CLOSE) {
    offset += sprintf(buf + offset, "IN_CLOSE|");
  }
  if (mask & IN_OPEN) {
    offset += sprintf(buf + offset, "IN_OPEN|");
  }
  if (mask & IN_MOVED_FROM) {
    offset += sprintf(buf + offset, "IN_MOVED_FROM|");
  }
  if (mask & IN_MOVED_TO) {
    offset += sprintf(buf + offset, "IN_MOVED_TO|");
  }
  if (mask & IN_MOVE) {
    offset += sprintf(buf + offset, "IN_MOVE|");
  }
  if (mask & IN_CREATE) {
    offset += sprintf(buf + offset, "IN_CREATE|");
  }
  if (mask & IN_DELETE) {
    offset += sprintf(buf + offset, "IN_DELETE|");
  }
  if (mask & IN_DELETE_SELF) {
    offset += sprintf(buf + offset, "IN_DELETE_SELF|");
  }
  if (mask & IN_MOVE_SELF) {
    offset += sprintf(buf + offset, "IN_MOVE_SELF");
  }
  // if (mask & IN_ALL_EVENTS) {
  //   offset += sprintf(buf + offset, "IN_ALL_EVENTS|");
  // }
  return buf;
}

void dump_inotify_event(int index, struct inotify_event *e) {
  printf("inotify event[%d] -> name: '%s', fd: %d, masks: %s, len: %d, cookie: %d\n",
         index, e->name, e->wd, map_inotify_mask(e->mask), e->len, e->cookie);
}

void inotify_event_handler(int inotify_fd) {
  // 读取 inotify 事件
  struct inotify_event buf[8];
  int rs = (int) read(inotify_fd, buf, sizeof(buf));
  if (rs > 0) {
    int N = rs / (int) sizeof(struct inotify_event);
    std::cout << "read " << N << " events, size: " << rs << " bytes" << std::endl;
    for (int j = 0; j < N; ++j) {
      auto event = (struct inotify_event *) &buf[j];
      dump_inotify_event(j, event);
    }
  } else {
    std::cerr << "read err, continue.\n";
  }
}

void epoll_event_handler(int num, struct epoll_event event_buf[], int inotify_fd) {
  std::cout << "epoll_event_handler() received " << num << " events." << std::endl;
  for (int i = 0; i < num; ++i) {
    // handle one epoll event
    struct epoll_event e = event_buf[i];
    dump_epoll_event(i, &e);
    inotify_event_handler(inotify_fd);
  }
}

void fileObserverTest() {
  auto cwd = getcwd(nullptr, 0);
  std::string prj_root = fs::path(cwd).parent_path();
  std::cout << "cwd: " << cwd << ", prj root: " << prj_root << std::endl;
  if (cwd != nullptr) {
    free(cwd);
  }

  int epoll_fd = epoll_create1(EPOLL_CLOEXEC);
  if (epoll_fd == -1) {
    std::cerr << "Failed to initialize fd observer.\n";
  }

  int inotify_fd = inotify_init1(IN_CLOEXEC);
  if (inotify_fd == -1) {
    std::cerr << "Failed to initialize fd observer.\n";
  }
  std::cout << "Initialize observer fd: " << inotify_fd << std::endl;
  int watched_fd = inotify_add_watch(inotify_fd, (prj_root + "/testdata").data(), IN_CLOSE_WRITE);
  if (watched_fd == -1) {
    std::cerr << "Failed to add path to observer.\n";
    close(inotify_fd);
    close(epoll_fd);
    return;
  }
  std::cout << "Initialize watched fd: " << watched_fd << std::endl;

  struct epoll_event eevent{};
  eevent.events = EPOLLIN | EPOLLWAKEUP;
  eevent.data.fd = inotify_fd;
  eevent.data.ptr = (void *) "hello, epoll";
  epoll_ctl(epoll_fd, EPOLL_CTL_ADD, inotify_fd, &eevent);

  struct UserData userData{
      .epoll_fd = epoll_fd,
      .inotify_fd = inotify_fd,
  };

  pthread_t observer_t;
  pthread_create(&observer_t, nullptr, [](void *user) {
    struct UserData *data = ((struct UserData *) user);
    std::cout << "observer proc enter with efd: " << data->epoll_fd << ", ifd: " << data->inotify_fd << std::endl;
    struct epoll_event event_buf[32];
    while (true) {
      // 等待 epoll 事件
      int num = epoll_wait(data->epoll_fd, event_buf, 16, 5000);
      if (num < 0) {
        // failed
        std::cerr << "epoll_wait() failed: " << errno << std::endl;
        break;
      } else if (num == 0) {
        // timeout, no events received
        std::cout << "epoll_wait() after 5000ms, timout..." << std::endl;
        usleep(1500000);
        continue;
      } else {
        // ok, num is events count
        epoll_event_handler(num, event_buf, data->inotify_fd);
        usleep(1500000);
      }
    }
    return (void *) nullptr;
  }, &userData);

  // std::string cmd = std::string("python3 ") + prj_root + "/scripts/write_file.py";
  // std::cout << "cmd is: " << cmd << std::endl;
  // int res = system(cmd.data());
  // std::cout << "cmd exit with: " << res << std::endl;

  // 等待子线程执行完成
  pthread_join(observer_t, nullptr);

  // 关闭文件句柄
  close(inotify_fd);
  close(epoll_fd);
  std::cout << __func__ << "() exit.\n";
}

void readFileTest() {
  auto cwd = getcwd(nullptr, 0);
  std::string prj_root = fs::path(cwd).parent_path();
  auto test_file = prj_root + "/testdata/results.json";
  // 打开文件
  auto fp = fopen(test_file.data(), "rb");
  struct stat s{};
  stat(test_file.data(), &s);
  char buf[s.st_size];
  fread(buf, sizeof(buf), sizeof(*buf), fp);
  std::cout << "file size: " << s.st_size << " is: " << buf << std::endl;
}

std::string timeFormatTest(float time) {
  // 时间格式为： 00:01:30.500
  int h = 0, m = 0;
  if (time > 3600) {
    h = (int) time / 3600;
    time = time - h * 3600;
  }
  if (time > 60) {
    m = (int) time / 60;
    time = time - m * 60;
  }
  float s = time;
  std::stringstream r;
  if (h < 10) {
    r << "0" << h << ":";
  } else {
    r << h << ":";
  }
  if (m < 10) {
    r << "0" << m << ":";
  } else {
    r << m << ":";
  }
  if (s < 10) {
    r << "0" << s;
  } else {
    r << s;
  }
  return r.str();
}

void jsonTest() {
  // 从字符串解析
  auto j = nlohmann::json::parse(R"({"hello":"World","world":"Hello","duration": 2.79})");
  std::string h = j["hello"].get<std::string>();
  printf("%s() hello is : %s\n", __func__, h.c_str());
  std::string w = j["hello"].get<std::string>();
  printf("%s() world is : %s\n", __func__, w.c_str());
  float duration = j["duration"].get<float>();
  printf("%s() duration is : %.2f\n", __func__, duration);
  std::cout << j.dump() << std::endl;
  // 从文件解析
  std::ifstream fs("/home/binlee/code/Clearn/basic/testdata/results.json");
  j = nlohmann::json::parse(fs);
  std::cout << j.dump() << std::endl;
  fs.close();
  std::vector<float> v = {1.0f, 2.0f, 3.0f};
  v.push_back(4.0f);
  v.push_back(5.0f);
  v.push_back(6.0f);
  std::cout << nlohmann::json(v).dump() << std::endl;
  j = nlohmann::json();
  j["arr"] = {1.0f, 2.0f, 3.0f};
  j["str"] = "this is as string";
  if (j["arr"].is_array()) {
    std::cout << j["arr"].dump() << std::endl;
    auto arr = j["arr"].get<std::vector<float>>();
    for (const auto &item: arr) {
      std::cout << item << std::endl;
    }
  }
  j["obj"] = R"({"a":"a-","b":"b-"})"_json;
  j["obj_"] = {
      {"pi", 3.1415926},
      {"happy", true}
  };
  std::cout << j.dump() << std::endl;

  // 写入文件中
  std::ofstream o("/home/binlee/code/Clearn/basic/testdata/out.json");
  o << j << std::endl;
  o.close();
}

void ffmpegVideoReverseTest(const std::string &input_file, const std::string &output_file) {
  auto tmp_dir = std::string ("/tmp/") + std::to_string(random()) + "x" + std::to_string(random());
  std::cout << "tmp dir: " << tmp_dir << std::endl;
  pipeTest("mkdir -p " + tmp_dir);
  // 1、切割视频
  // ffmpeg -i input.mp4 -c copy -map 0 -segment_time 300 -f segment output%d.mp4
  pipeTest("ffmpeg -v error -i " + input_file + " -c copy -map 0 -segment_time 5 -f segment -y " +
            tmp_dir + "/part-%d.mp4");
  // 2、 获取视频时长并计算文件分片数
  auto _command = std::string("ffprobe -v error -show_format -show_streams -of json ") + input_file;
  auto probe = nlohmann::json::parse(pipeTest(_command).second);
  assert(probe.is_object());
  auto dj = probe["format"]["duration"];
  auto duration = dj.is_number_float() ? dj.get<float>() * 1000 : std::stof(dj.get<std::string>()) * 1000;
  std::cout << "duration is: " << duration << "ms" << ", segments: " << ceilf(duration / 5000) << std::endl;

  // 检测切割是否成功
  auto last_part = tmp_dir + "/part-" + std::to_string(((int) ceilf(duration/5000)) - 1) + ".mp4";
  struct stat s{};
  if (stat(last_part.c_str(), &s) != 0) {
    std::cerr << "split file failed" << std::endl;
    std::cout << pipeTest("ls -alh " + tmp_dir).second << std::endl;
    return;
  }

  auto part_files = std::vector<std::string>((int) ceilf(duration / 5000));
  auto part_rfiles = std::vector<std::string>(part_files.capacity());
  for (int i = 0; i < part_files.capacity(); ++i) {
    part_files[i] = tmp_dir + "/part-" + std::to_string(i) + ".mp4";
    part_rfiles[i] = tmp_dir + "/part-r-" + std::to_string(i) + ".mp4";
  }
  // 3、分段倒放
  for (int i = 0; i < part_files.size(); ++i) {
    // ffmpeg -i output1.mp4 -vf reverse reversed_output1.mp4
    pipeTest("ffmpeg -v error -i " + part_files[i] + " -vf reverse -y " + part_rfiles[i]);
  }
  // 4.1、倒放后的视频逆序后合并
  std::reverse(part_rfiles.begin(), part_rfiles.end());
  auto merge_file = tmp_dir + "/all-r-files.txt";
  auto fs = std::ofstream(merge_file);
  if (!fs.is_open()) return;
  for (const auto &prf: part_rfiles) {
    fs << "file '" << prf << "'" << std::endl;
  }
  fs.close();
  // 4.2、合并视频
  // ffmpeg -i "concat:reversed_output1.mp4|reversed_output2.mp4|reversed_output3.mp4" -c copy reversed_full.mp4
  pipeTest("ffmpeg -v error -f concat -safe 0 -i " + merge_file + " -c copy -y " + output_file);

  // 播放倒放后的视频
  std::cout << pipeTest("ls -alh " + tmp_dir).second << std::endl;
  // pipeTest("ffplay -v error " + input_file);
  pipeTest("ffplay -v error " + output_file);

  // std::__fs::filesystem::remove_all(tmp_dir);
}

void ffmpegAudioSplitTest(const std::string &input_file, const std::string &config_file) {
  // // ffmpeg -i input.mp3 -ss 00:01:30 -to 00:02:00 output.mp3
  std::ifstream ifs(config_file);
  auto json = nlohmann::json::parse(ifs);
  ifs.close();

  nlohmann::json split_result(json["data"]);
  for (auto &item: split_result) {
    std::cout << item.dump() << std::endl;
    auto start = timeFormatTest(item["start"].get<float>());
    auto end = timeFormatTest(item["end"].get<float>());
    std::stringstream sub_wav;
    sub_wav << "/home/binlee/code/Clearn/basic/testdata/split/" << start << "-" << end << ".wav";
    std::cout << sub_wav.str() << std::endl;
    // 根据起始和结束时间来分割音频
    // ffmpeg -i input.mp3 -ss 00:01:30 -to 00:02:00 output.mp3
    std::stringstream cmd_;
    cmd_ << "ffmpeg -v error -i " << input_file << " -ss " << start << " -to " << end << " -y " << sub_wav.str();
    pipeTest(cmd_.str());
    item.erase("start");
    item.erase("end");
    item["url"] = sub_wav.str();
  }

  // 结果写入文件中
  std::ofstream out("/home/binlee/code/Clearn/basic/testdata/split_result.json");
  out << split_result << std::endl;
  out.close();
}

void ffmpegAudioConvertTest(const std::string &input_file, const std::string &output_file) {
  pipeTest("ffmpeg -v error -i " + input_file + " -acodec pcm_s16le -y " + output_file);
  pipeTest("ffprobe " + output_file + "; ffplay -v error " + output_file);
}

void ffmpegVideoMergeTest(const std::vector<std::string> &input_files, const std::string &output_file) {
  // ffmpeg -i input1.mp4 -i input2.mp4 -filter_complex "[0:v][0:a][1:v][1:a]concat=n=2:v=1:a=1" -strict -2 -y output.mp4
  auto cmd = std::string("ffmpeg -v error");
  for (const auto &item: input_files) {
    cmd += (" -i " + item);
  }
  cmd += " -filter_complex \"";
  for (int i = 0; i < input_files.size(); ++i) {
    cmd += ("[" + std::to_string(i) + ":v][" + std::to_string(i) + ":a]");
  }
  cmd += ("concat=n=" + std::to_string(input_files.size()) + ":v=1:a=1");
  cmd += ("\" -strict -2 -y " + output_file);
  std::cout << "cmd is: " << cmd << std::endl;
  auto r = pipeTest(cmd);
  if (r.second == PIPE_ERR) return;

  std::cout << pipeTest("ls -alh " + output_file).second << std::endl;
  pipeTest("vlc " + output_file);
}

void classInstanceRefTest() {
  auto setField = [](const std::string &value, std::string &dst, const std::string &msg) {
    dst = value;
    std::cout << "set '" + value << "' to '" << msg << "'" << std::endl;
  };

  struct Meta {
    std::string version;
    std::string name;
  };

  Meta meta{};
  setField("v1", meta.version, "version");
  setField("prj parse", meta.name, "name");

  std::cout << "meta detail: name: " << meta.name << ", version: " << meta.version << std::endl;
}

void eraseMapTest() {
  auto m = std::map<std::string, uint64_t>();
  m.insert({"a", 100});
  m.insert({"b", 200});
  std::cout << m.erase("a") << std::endl;
  std::cout << m.erase("a") << std::endl;
  std::cout << m.erase("a") << std::endl;
  std::cout << m.erase("a") << std::endl;
}

void threadTest() {
  auto id = std::this_thread::get_id();
  std::cout << "threadTest() main thread id: " << id << std::endl;

  auto task = []() {
    auto id = std::this_thread::get_id();
    std::cout << "task() sub thread id: " << id << std::endl;
    for (int i = 0; i < 5; ++i) {
      std::cout << "loop " << i << std::endl;
      usleep(i * 200 * 1000);
    }
  };

  auto t = std::thread(task);
  t.join();

  std::cout << "threadTest() exit " << std::endl;
}

void cmdRunnerTest() {
  auto runner = cmd_runner("tree");
  runner.add_argument(".", "\"\"");
  runner.add_argument("-L", "2");
  auto r = runner.run();
  std::cout << "res: " << r.first << ", out: " << r.second << std::endl;
}

int cpp_samples() {
  std::cout << "\n>>>>>>>Welcome to C++ World!<<<<<<<<\n" << std::endl;
  // cpp_string();
  // person_main();
  // searchTest();
  // matchTest();
  // parseTest();
  // getFileNameTest();
  // getFileExtTest();
  // tryCatchTest();
  // trimPathTest();
  // std::cout << pipeTest("ls -alh .") << std::endl;
  // std::cout << pipeTest("ffmpeg -version") << std::endl;
  // fileObserverTest();
  // readFileTest();
  // classInstanceRefTest();
  // jsonTest();
  // pipeTest("ls -vervose");
  // ffmpegVideoReverseTest("/home/binlee/Downloads/audio/dance-4k.mp4", "/tmp/dance-4k-reversed.mp4");
  // ffmpegVideoReverseTest("/tmp/ff/res.mp4", "/tmp/ff/res-reversed.mp4");
  // auto float_time_list = std::vector<float>({3.6f, 36.f, 366.f, 3666.f, 3666.6f});
  // for (const auto &item: float_time_list) {
  //   std::cout << timeFormatTest(item) << std::endl;
  // }
  // // /home/binlee/code/open-source/quvideo/algo-audio-whisper/testdata/results-zh.json
  // // /home/binlee/code/open-source/quvideo/algo-audio-whisper/testdata/ok.wav
  // ffmpegAudioSplitTest("/home/binlee/code/open-source/quvideo/algo-audio-whisper/testdata/ok.wav",
  //                         "/home/binlee/code/open-source/quvideo/algo-audio-whisper/testdata/results-zh.json");
  // ffmpegAudioConvertTest("/home/binlee/Downloads/18s-audio.m4a", "/home/binlee/Downloads/18s-audio.wav");
  // ffmpegVideoMergeTest({"/tmp/1804289383x846930886/part-0.mp4", "/tmp/1804289383x846930886/part-r-0.mp4"},
  //                      "/tmp/1804289383x846930886/hello.mp4");
  // eraseMapTest();
  // threadTest();
  // cmdRunnerTest();

  std::any a;
  a = 1;
  std::cout << "type: " << a.type().name() << " \n";// << std::any_cast<int>(a) << std::endl;
  a = "hello world";
  std::cout << "type: " << a.type().name() << " \n";// << std::any_cast<std::string>(a) << std::endl;
  return 0;
}

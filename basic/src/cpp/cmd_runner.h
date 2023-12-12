#ifndef CMD_RUNNER_H_INCLUDE_
#define CMD_RUNNER_H_INCLUDE_

#include <string>
#include <vector>
#include <map>
#include <iostream>
#include <sstream>

static std::string NULL_VAL("_null_value_");
static std::string PIPE_ERROR("_pipe_error_");

/**
 * 执行一条命令并捕获其输出
 * @param cmd 命令
 * @param rt_output 是否实时输出，默认 true
 * @return 返回命令执行错误码及控制台输出
 */
std::pair<int, std::string> run_pipe(const std::string &cmd, const bool rt_output = true) {
  auto fd = popen(cmd.c_str(), "r");
  if (fd == nullptr) {
    std::cerr << "Create pipe error." << std::endl;
    return {-1, PIPE_ERROR};
  }
  std::stringstream ss;
  char buf[1024];
  while (fgets(buf, sizeof(buf), fd) != nullptr) {
    ss << buf;
    if (rt_output) std::cout << buf;
  }

  // pclose() 返回该命令执行返回值，0 表示成功，其他表示失败
  int res = pclose(fd);
  printf("'%s' exit with: %d -> %x\n", cmd.c_str(), res, res);
  return {res, ss.str()};
}

class cmd_runner {
private:
  std::string cmd_;
  std::vector<std::string> args_;
  bool rt_output_;
public:
  explicit cmd_runner(const std::string &cmd, bool rt_output = true) {
    this->cmd_ = cmd;
    this->args_ = {};
    this->rt_output_ = rt_output;
  }

  ~cmd_runner() {
    this->cmd_.clear();
    this->args_.clear();
  }

  void add_argument(const std::string &name, const std::string &value = NULL_VAL) {
    this->args_.push_back(name);
    if (value != NULL_VAL) {
      this->args_.push_back(value);
    }
  }

  [[nodiscard]] std::pair<int, std::string> run() const {
    auto cmd = this->cmd_ + " ";
    for (int i = 0; i < this->args_.size(); ++i) {
      if (i != this->args_.size() - 1) {
        cmd += this->args_[i] + " ";
      } else {
        cmd += this->args_[i];
      }
    }
    return run_pipe(cmd, this->rt_output_);
  }
};

#endif  // CMD_RUNNER_H_INCLUDE_
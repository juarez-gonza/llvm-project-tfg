#include <climits>
#include <memory>
#include <random>
#include <vector>
#include <algorithm>

#include <cstring>
#include <cassert>
#include <string>
#include <unistd.h>

#include <iostream>

// Class for managing perf monitoring
class perf_manager {

  // Control and ack fifo from perf
  int ctl_fd = -1;
  int ack_fd = -1;

  // If perf is enabled
  bool enable = false;

  // Commands and acks to/from perf
  static constexpr const char *enable_cmd = "enable";
  static constexpr const char *disable_cmd = "disable";
  static constexpr const char *ack_cmd = "ack\n";

  // Send command to perf via fifo and confirm ack
  void send_command(const char *command) {
    if (enable) {
      write(ctl_fd, command, strlen(command));
      char ack[5];
      read(ack_fd, ack, 5);
      assert(strcmp(ack, ack_cmd) == 0);
    }
  }

public:
  perf_manager() {
    // Setup fifo file descriptors
    char *ctl_fd_env = std::getenv("PERF_CTL_FD");
    char *ack_fd_env = std::getenv("PERF_ACK_FD");
    if (ctl_fd_env && ack_fd_env) {
      enable = true;
      ctl_fd = std::stoi(ctl_fd_env);
      ack_fd = std::stoi(ack_fd_env);
    }
  }

  // Public apis

  void pause() { send_command(disable_cmd); }

  void resume() { send_command(enable_cmd); }
};

// Inheritance

class comparator {
public:
  virtual bool cmp(int, int) = 0;
  virtual ~comparator() = default;
};

class lt : public comparator {
public:
  bool cmp(int x, int y) override {
    return x < y;
  }
};

class gt : public comparator {
public:
  bool cmp(int x, int y) override {
    return x > y;
  }
};

// Data initialization

std::vector<int>
random_data_init(size_t count, int lowerbound = INT_MIN,
                 int upperbound = INT_MAX) {
  std::random_device rd;
  std::mt19937 mt(rd());
  std::uniform_int_distribution<int> uniform(lowerbound, upperbound);
  std::vector<int> out;
  for (size_t i = 0; i < count; ++i)
    out.emplace_back(uniform(mt));
  return out;
}

// Algorithm

void bubble_sort(std::vector<int>& xs, comparator& c) {
  using size_type = decltype(xs.size());
  for (size_type i = 0; i < xs.size(); ++i) {
    bool swapped = false;
    for (size_type j = 0; j < xs.size() - i - 1; ++j) {
      if (c.cmp(xs[j + 1], xs[j])) {
	std::iter_swap(xs.begin() + j + 1, xs.begin() + j);
	swapped = true;
      }
    }
  }
}

int main() {
  perf_manager pm;
  pm.pause();
  auto xs = random_data_init(10000);
  pm.resume();
  lt c;
  bubble_sort(xs, c);
  pm.pause();
  // for (auto x : xs)
  //   std::cout << x << "\n";
  return 0;
}

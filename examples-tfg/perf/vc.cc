#include <climits>
#include <memory>
#include <random>
#include <vector>

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

struct ord_a {
public:
  ord_a(int x) : x_{x} {}
  bool lt(int x) { return x_ < x; }
  int get_as_int() { return x_; }

private:
  int x_;
};

struct ord_b {
public:
  ord_b(int x) : x_{x} {}
  bool lt(int x) { return x_ < x; }
  int get_as_int() { return x_; }

private:
  int x_;
};

struct ord_c {
public:
  ord_c(int x) : x_{x} {}
  bool lt(int x) { return x_ < x; }
  int get_as_int() { return x_; }

private:
  int x_;
};


template<typename T>
concept int_entity = requires(T x) {
  { x.get_as_int() } -> int;
  { x.lt(1) } -> bool;
};

// Data initialization

std::unique_ptr<int_entity virtual>
make_rnd_init_entity(std::uniform_int_distribution<int> &uniform,
                     std::mt19937 &mt, int instantiation_choice) {
    ord_a x{uniform(mt)};
    int_entity virtual ret(x);
    std::unique_ptr<int_entity virtual> ret_ = std::make_unique<decltype(ret)>(ret);
    return ret_;
//  switch (instantiation_choice) {
//  case 1: {
//    int_entity virtual ret(ord_a{uniform(mt)});
//    auto ret_ = std::make_unique<decltype(ret)>(ret);
//    return ret_;
//  }
//  case 2: {
//    int_entity virtual ret(ord_b{uniform(mt)});
//    auto ret_ = std::make_unique<decltype(ret)>(ret);
//    return ret_;
//  }
//  default: {
//    int_entity virtual ret(ord_c{uniform(mt)});
//    auto ret_ = std::make_unique<decltype(ret)>(ret);
//    return ret_;
//  }
//  }
}

std::vector<std::unique_ptr<int_entity virtual>>
random_data_init(size_t count, int lowerbound = INT_MIN,
                 int upperbound = INT_MAX) {
  std::random_device rd;
  std::mt19937 mt(rd());
  std::uniform_int_distribution<int> uniform(lowerbound, upperbound);
  std::vector<std::unique_ptr<int_entity virtual>> out;
  for (size_t i = 0; i < count; ++i) // TODO: randomize instantiation choice
    out.emplace_back(make_rnd_init_entity(uniform, mt, i % 3));
  return out;
}

// Algorithm

void bubble_sort(std::vector<std::unique_ptr<int_entity virtual>>& xs) {
  using size_type = decltype(xs.size());
  for (size_type i = 0; i < xs.size(); ++i) {
    bool swapped = false;
    for (size_type j = 0; j < xs.size() - i - 1; ++j) {
      if (xs[j + 1]->lt(xs[j]->get_as_int())) {
	xs[j + 1].swap(xs[j]);
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
  bubble_sort(xs);
  pm.pause();
  // for (auto& x : xs)
  //   std::cout << x->get_as_int() << "\n";
  return 0;
}

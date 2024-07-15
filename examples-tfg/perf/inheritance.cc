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

class entity {
public:
  virtual bool lt(int) = 0;
  virtual int get_as_int() = 0;
  virtual ~entity() = default;
};

class ord_a : public entity {
public:
  ord_a(int x) : x_{x} {}
  bool lt(int x) override { return x_ < x; }
  int get_as_int() override { return x_; }

private:
  int x_;
};

class ord_b : public entity {
public:
  ord_b(int x) : x_{x} {}
  bool lt(int x) override { return x_ < x; }
  int get_as_int() override { return x_; }

private:
  int x_;
};

class ord_c : public entity {
public:
  ord_c(int x) : x_{x} {}
  bool lt(int x) override { return x_ < x; }
  int get_as_int() override { return x_; }

private:
  int x_;
};

// Data initialization

std::unique_ptr<entity>
make_rnd_init_entity(std::uniform_int_distribution<int> &uniform,
                     std::mt19937 &mt, int instantiation_choice) {
  switch (instantiation_choice) {
  case 1:
    return std::make_unique<ord_a>(uniform(mt));
  case 2:
    return std::make_unique<ord_b>(uniform(mt));
  default:
    return std::make_unique<ord_c>(uniform(mt));
  }
}

std::vector<std::unique_ptr<entity>>
random_data_init(size_t count, int lowerbound = INT_MIN,
                 int upperbound = INT_MAX) {
  std::random_device rd;
  std::mt19937 mt(rd());
  std::uniform_int_distribution<int> uniform(lowerbound, upperbound);
  std::vector<std::unique_ptr<entity>> out;
  for (size_t i = 0; i < count; ++i) // TODO: randomize instantiation choice
    out.emplace_back(make_rnd_init_entity(uniform, mt, i % 3));
  return out;
}

// Algorithm

void bubble_sort(std::vector<std::unique_ptr<entity>>& xs) {
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

#include <stdio.h>

int use(char x) {
  puts("\n### use(char) ###\n");
  return (int)x;
}

int use(int x) {
  puts("\n### use(int) ###\n");
  return x;
}

int use(void * x) {
  puts("\n### use(void *) ###\n");
  return 0;
}

template<typename T>
concept Usable = requires(T x) {
  { use(x) } -> int;
};

Usable virtual *make_usable(int x) {
  if (x % 2 == 0)
    return 'a';
  else
    return 1;
}

int sum_usables(Usable virtual *x, Usable virtual *y) {
  return use(*x) + use(*y);
}

int print_use(std::unique_ptr<Usable virtual> x) {
  auto result = use(*x);
  printf("\n%d\n");
  return result;
}

int main() {
  void *x = nullptr;
  Usable virtual y = 'a';
  return
  print_use(
    std::make_unique<Usable virtual>(
      sum_usables(&y,
                  sum_usables(&x,
                              sum_usables(make_usable(1),
                                           make_usable(2))))));
}

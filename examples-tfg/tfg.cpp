//#include <iostream>
// #include <string>
#include <stdio.h>
//#include <memory>

/*
// Non-virtualizable concept
template<typename T>
concept Fooable = requires (T x) {
  // Only compound statements accepted so far
  foo(x);
};

// Non-virtualizable concept
template<typename T>
// T present twice in the requires parameter list
concept Barable = requires (T x, T y, int z) {
  // Only compound statements accepted so far
  bar(x);
};

// Non-virtualizable
template<typename T>
concept Zetable = requires(T x) {
  // T used twice as argument
  { zeta(x, x) } noexcept;
};


// Virtualizable
//template<typename T>
//concept Usable = requires(T x, int y) {
//  { use(x, y) } noexcept -> int;
//};
*/

// Non-virtualizable
/*
template<typename T>
concept Unusable = requires(T x, int y) {
  { unuse(x, y) } noexcept; // Missing return type constraint
};
*/

struct Asd {
private:
int m;
public:
explicit Asd(int x = 1) : m{1} {}
~Asd() {
  puts("~Asd()\n");
}
};

int use(Asd x) {
  puts("use Asd\n");
  return 0;
}

int use(char x) {
  puts("use char\n");
  return x;
}

template<typename T>
concept Usable = requires(T x) {
  { use(x) } -> int;
};

void use_usable(Usable virtual *x) {
  use(*x);
}

template <typename T>
struct ptr {
  T* x;
  T* operator->() {
    return x;
  }

  T& operator*() {
    return *x;
  }
};

void use_usable_ptr(ptr<Usable virtual> x) {
  use(*x);
}

int main() {
  Usable virtual c('a');
  Usable virtual d(Asd{1});
  use_usable_ptr(ptr<Usable virtual>(&c));
  use_usable(&d);
  use_usable(&c);
  return 0;
}

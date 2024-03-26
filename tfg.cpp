// #include <iostream>
// #include <string>
#include <stdio.h>

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

int use(char x) {
  return x;
}

/*
double use_double(char x) {
  return (int)x;
}

template<typename T>
concept UsableDouble = requires(T x) {
  { use_double(x) } -> double;
};
*/

template<typename T>
concept Usable = requires(T x) {
  { use(x) } -> int;
};

void use_usable(tfg_virtual_Usable *x) {
  x->tfg_virtual_use();
  delete x;
}

struct usable_char: tfg_virtual_Usable {
  char x;
  usable_char() : x{'a'} {}
  usable_char(char x) : x{x} {}
  int tfg_virtual_use() override {
    puts("usable_char::use()");
    return use(x);
  }
  ~usable_char() override { puts("~usable_char()"); }
};

struct asd {
  virtual int Do() = 0;
  virtual ~asd() = default;
};

struct asdasd : public asd {
  virtual int Do() override {
    return 0;
  }
  ~asdasd() override { puts("\n~asdasd()\n"); }
};


static_assert(Usable<char>, "LA PUTA MADRE");

int main() {
  asd* x  = new asdasd;
  delete x;

  auto* c = new usable_char('a');
  use_usable(c);
  return use('a');
}

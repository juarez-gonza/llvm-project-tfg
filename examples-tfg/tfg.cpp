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

int use(char x) {
  return x;
}

struct Asd {
private:
int m;
public:
explicit Asd(int x = 1) : m{1} {}
};

int use(Asd x) {
  puts("Asd\n");
  return 0;
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

void use_usable(Usable virtual *x) {
  x->_tfg_virtual_use();
}

struct _tfg_virtual_Usable_int : _tfg_virtual_Usable {
  int x;
  _tfg_virtual_Usable_int(int x) : x{x} {}
  int _tfg_virtual_use() override {
    //puts("usable_int::use()");
    return use(x);
  }
  //~_tfg_virtual_Usable_int() override = default;
};

/*
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
*/

//template <typename T>
//struct ptr {
//  T* x;
//  T* operator->() {
//    return x;
//  }
//};
//
//void use_usable_ptr(ptr<Usable virtual> x) {
//  x->_tfg_virtual_use();
//}


//static_assert(Usable<char>, "LA PUTA MADRE");

int main() {
  Usable virtual c('a');
  Usable virtual d(Asd{1});
  //use_usable_ptr(ptr<Usable virtual>(&c));
  use_usable(&d);
  return 0;
}

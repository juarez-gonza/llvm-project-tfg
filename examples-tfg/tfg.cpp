//#include <iostream>
// #include <string>
//#include <stdio.h>

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

namespace A {

struct Asd {
private:
int m;
public:
/*explicit*/ Asd(int x) : m{1} {}
int foo(int x) const {
  //puts("Asd::foo(int)\n");
  return 0;
}
//~Asd() {
//  //puts("~Asd()\n");
//}
};

int use(Asd x, double y) {
  //puts("use(Asd, double)\n");
  return 0;
}

} // namespace A

//int use(char x) {
//  puts("use char\n");
//  return x;
//}

namespace B {

template<typename T>
concept Usable = requires(T x) {
  { use(x, 1.0) } -> int;
  { x.foo(1) } -> int;
};

} // namespace B

void use_usable(B::Usable virtual &x) {
  double y = 1.0;
  use(x, y);
  x.foo(1);
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

//void use_usable_ptr(ptr<Usable virtual> x) {
//  use(*x);
//}

B::Usable virtual &ret(B::Usable virtual *x) {
  return *x;
}

int main() {
  //Usable virtual c('a');
  B::Usable virtual d(A::Asd{1});
  ptr<B::Usable virtual> x{&d};
  B::Usable virtual &b = ret(&d);
  b.foo(2);
  //use_usable_ptr(ptr<Usable virtual>(&c));
  use_usable(d);
  //use_usable(&c);
  return 0;
}

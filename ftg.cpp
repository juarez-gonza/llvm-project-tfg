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

// Non-virtualizable
template<typename T>
concept Unusable = requires(T x, int y) {
  { unuse(x, y) } noexcept; // Missing return type constraint
};

// Virtualizable
template<typename T>
concept Usable = requires(T x, int y) {
  { use(x, y) } noexcept -> Unusable;
};

int main() {
  return 0;
}

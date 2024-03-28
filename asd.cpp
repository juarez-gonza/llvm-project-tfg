template<typename T>
concept IsInt = __is_same(T, int);

/*
auto asd(IsInt auto y) {
  return y;
}
*/

int main() {
  // IsInt auto x = 1;
  IsInt virtual y = 1;
  return 0; // x + y;
  //return 0;
}

#include "tuple.h"
#include <iostream>

using namespace stl;

void TestConstruction() {
  tuple<int, double, std::string> t(17, 3.14, "Hello, World!");
  std::cout << get<0>(t) << ' ' << get<1>(t) << ' ' << get<2>(t) << '\n';
  
  // Class Template Argument Deduction
  tuple t2(t);

  auto t3 = make_tuple(26, 3.142, "yejin");
  assert(get<0>(t3) == 26);
  static_assert(!is_same_v<decltype(t2), decltype(t3)>);
}

void TestPrintTuple() {
  tuple<int, double, std::string> t(17, 3.14, "Hello, World!");
  std::cout << t << '\n';
  tuple<> empty;
  std::cout << empty << '\n';
}

int main() {
  TestConstruction();
  TestPrintTuple();
  
  return 0;
}
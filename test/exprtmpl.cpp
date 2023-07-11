#include "exprtmpl.h"

#include <iostream>

using namespace stl;

void TestAddition() {
  std::cout << "==========Test Addition==========\n";
  Array<int> x(4);
  x[0] = 2;
  x[1] = 4;
  x[2] = 3;
  x[3] = 6;

  std::cout << "x = ";
  x.print();
  
  std::cout << "y = x + 4 = ";
  auto y = x + 4;
  y.print();

  std::cout << "k = x + y = ";
  auto k = x + y;
  k.print();

  std::cout << "z = 4 + x = ";
  auto z = 4 + x;
  z.print();
}

void TestMultiplication() {
  std::cout << "==========Test Multiplication==========\n";
  Array<int> x(4);
  x[0] = 1;
  x[1] = 2;
  x[2] = 3;
  x[3] = 4;
  std::cout << "x = ";
  x.print();

  auto y = 2 * x;
  std::cout << "y = x * 2 = ";
  y.print();

  auto z = x + y;
  std::cout << "z = x + y = ";
  z.print();

  auto t = y * z;
  std::cout << "t = y * z = ";
  t.print();

  std::cout << t[0] << '\n';
}

int main() {
  TestAddition();
  TestMultiplication();

  return 0;
}
#include "array.h"

#include <iostream>
#include <string>

using namespace stl;

void TestConstruction() {
  [[maybe_unused]] array<int, 3> a1 = {1, 2, 3};
  [[maybe_unused]] array<std::string, 2> a2 = {std::string("hello"), "bc"};
}

void TestAt() {
  array<int, 6> data = {1, 2, 4, 5, 5, 6};

  data.at(1) = 88;
  assert(data[1] == 88);
  assert(data[4] = 5);
}

void TestIndexOperator() {
  array<int, 4> numbers{2, 4, 6, 8};

  assert(numbers[1] == 4);
  numbers[0] = 5;
  assert(numbers[0] == 5);
}

void TestFront() {
  constexpr array<char, 6> letters{'a', 'b', 'c', 'd', 'e', 'f'};
  static_assert(letters.front() == 'a');
}

void TestBack() {
  constexpr array<char, 6> letters{'a', 'b', 'c', 'd', 'e', 'f'};
  static_assert(letters.back() == 'f');
}

void TestData() {
  array<int, 4> container{1, 2, 3, 4};
  auto p = container.data();
  assert(p[0] == 1);
  assert(p[3] == 4);
}

void TestIterator() {
  array<int, 4> numbers{5, 2, 3, 4};
  assert(!(numbers.begin() == numbers.end()));
  assert(!(numbers.cbegin() == numbers.cend()));
  assert(*(numbers.begin()) == 5);
  assert(*(numbers.cbegin()) == 5);

  *numbers.begin() = 1;
  assert(*(numbers.begin()) == 1);

  // print out all elements
  std::for_each(numbers.cbegin(), numbers.cend(),
                [](int x) { std::cout << x << ' '; });
  std::cout << '\n';

  constexpr std::array constants{'A', 'B', 'C'};
  static_assert(constants.begin() != constants.end());    // OK
  static_assert(constants.cbegin() != constants.cend());  // OK
  static_assert(*constants.begin() == 'A');               // OK
  static_assert(*constants.cbegin() == 'A');              // OK
}

void TestSize() {
  constexpr array<int, 4> nums {1, 3, 5, 7};
  static_assert(nums.size() == 4);
}

void TestFill() {
  constexpr size_t xy = 4;

  using Cell = array<unsigned char, 8>;

  array<Cell, xy * xy> board;

  board.fill({{0xE2, 0x96, 0x84, 0xE2, 0x96, 0x80, 0, 0}});  // "▄▀";

  for (size_t count{}; Cell c : board) {
    std::cout << c.data() << ((++count % xy) ? "" : "\n");
  }
}

int main() {
  TestConstruction();
  TestAt();
  TestIndexOperator();
  TestFront();
  TestBack();
  TestData();
  TestIterator();
  TestSize();
  TestFill();

  return 0;
}
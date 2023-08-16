#include "vector.h"

#include <iostream>
#include <numeric>
#include <utility>
#include <vector>

using std::cout;

template <typename T>
std::ostream& operator<<(std::ostream& s, const stl::vector<T>& v) {
  s.put('{');
  char comma[3] = {'\0', ' ', '\0'};
  for (const auto& e : v) {
    s << comma << e;
    comma[0] = ',';
  }

  return s << "}\n";
}

void TestConstructor() {
  cout << "==========TEST CONSTRUCTOR==========\n";
  stl::vector<std::string> words1{"momo", "yoona", "yejin", "joy", "yang"};
  cout << "1: " << words1;

  stl::vector<std::string> words2(words1.begin(), words1.end());
  cout << "2: " << words2;

  stl::vector<std::string> words3(words1);
  cout << "3: " << words3;

  stl::vector<std::string> words4(5, "momo");
  cout << "4: " << words4;

  stl::vector<std::string> words5(std::move(words1));
  cout << "5: " << words5;

  // C++ standards guarantees `words1` to be empty
  assert(words1.empty());
  cout << "PASS\n";
}

void TestAssignment() {
  cout << "==========TEST ASSIGNMENT==========\n";
  stl::vector<int> x{1, 2, 3}, y, z;
  const auto w = {4, 5, 6, 7};
  cout << "Initially\n";
  cout << "x = " << x;
  cout << "y = " << y;
  cout << "z = " << z;

  cout << "Copy assignment copies data from x to y:\n";
  y = x;
  cout << "x = " << x;
  cout << "y = " << y;

  cout << "Move assignment moves data from x to z, modifying both x and z:\n";
  z = std::move(x);
  cout << "x = " << x;
  cout << "z = " << z;

  cout << "Assignment of initializer_list w to z:\n";
  z = w;
  cout << "z = " << z;

  stl::vector<int> v1 = {1, 2, 3};
  assert(v1.capacity() == 3);
  v1 = w;
  cout << "v1 = " << v1;
  assert(v1.capacity() == 4);
  cout << "PASS\n";
}

void TestAssign() {
  cout << "==========TEST ASSIGN FUNCTION==========\n";
  stl::vector<char> characters;

  characters.assign(5, 'a');
  cout << characters;

  const std::string extra(6, 'b');
  characters.assign(extra.begin(), extra.end());
  cout << characters;

  characters.assign({'C', '+', '+', '2', '0'});
  cout << characters;
  cout << "PASS\n";
}

void TestAt() {
  cout << "==========TEST AT FUNCTION==========\n";
  stl::vector<int> data = {1, 2, 4, 5, 5, 6};
  data.at(1) = 88;
  assert(data.at(2) == 4);
  assert(data.size() == 6);
  cout << data;
  cout << "PASS\n";
}

void TestIndexingOperator() {
  cout << "==========TEST INDEXING OPERATOR==========\n";
  stl::vector<int> numbers{2, 4, 6, 8};
  assert(numbers[1] == 4);
  numbers[0] = 5;
  assert(numbers[0] == 5);
  cout << numbers;
  cout << "PASS\n";
}

void TestFront() {
  cout << "==========TEST FRONT FUNCTION==========\n";
  stl::vector<char> letters{'a', 'b', 'c', 'd', 'e', 'f'};
  assert(letters.front() == 'a');
  letters.front() = 'g';
  assert(letters.front() == 'g');
  cout << "PASS\n";
}

void TestBack() {
  cout << "==========TEST BACK FUNCTION==========\n";
  stl::vector<char> letters{'a', 'b', 'c', 'd', 'e', 'f'};
  assert(letters.back() == 'f');
  letters.back() = 'g';
  assert(letters.back() == 'g');
  cout << "PASS\n";
}

void TestData() {
  cout << "==========TEST DATA FUNCTION==========\n";
  stl::vector<int> container{1, 2, 3, 4};

  stl::vector<int>::pointer p = container.data();

  cout << "data = ";
  for (int i = 0; i < container.size(); i++) {
    cout << p[i] << ' ';
  }
  cout << '\n';
  cout << "PASS\n";
}

void TestIterators() {
  cout << "==========TEST ITERATORS==========\n";

  stl::vector<int> nums{1, 2, 4, 8, 16};
  stl::vector<std::string> fruits{"orange", "apple", "raspberry"};
  stl::vector<char> empty;

  std::for_each(nums.begin(), nums.end(),
                [](const int n) { cout << n << ' '; });
  cout << '\n';

  assert(std::accumulate(nums.begin(), nums.end(), 0) == 31);
  assert(*fruits.begin() == "orange");
  assert(*(fruits.end() - 1) == "raspberry");
  assert(empty.begin() == empty.end());
  cout << "PASS\n";
}

void TestEmpty() {
  cout << "==========TEST EMPTY FUNCTION==========\n";
  stl::vector<int> numbers;
  assert(numbers.empty());
  // numbers.push_back(42);
  // assert(!numbers.empty());
  cout << "PASS\n";
}

void TestSize() {
  cout << "==========TEST SIZE FUNCTION==========\n";
  stl::vector<int> nums {1, 3, 5, 7};
  assert(nums.size() == 4);
  cout << "PASS\n";
}

void TestClear() {
  cout << "==========TEST CLEAR FUNCTION==========\n";
  stl::vector<int> container{1, 2, 3};
  assert(container.size() == 3);
  assert(container.capacity() == 3);
  container.clear();
  assert(container.size() == 0);
  assert(container.capacity() == 3);
  cout << "PASS\n";
}

void TestInsert() {
  cout << "==========TEST INSERT FUNCTION==========\n";
  stl::vector<int> c1(3, 100);
  cout << "1: " << c1;

  auto it = c1.begin();
  it = c1.insert(it, 200);
  cout << "2: " << c1;

  c1.insert(it, 2, 300);
  cout << "3: " << c1;

  it = c1.begin();
  it += 2;
  stl::vector<int> c2(2, 400);
  c1.insert(it, c2.begin(), c2.end());
  cout << "4: " << c1;

  int arr[] = {501, 502, 503};
  c1.insert(c1.begin(), arr, arr + std::size(arr));
  cout << "5: " << c1;

  c1.insert(c1.end(), {601, 602, 603});
  cout << "6: " << c1;
}

int main() {
  TestConstructor();
  TestAssignment();
  TestAssign();
  TestAt();
  TestIndexingOperator();
  TestFront();
  TestBack();
  TestData();
  TestIterators();
  TestEmpty();
  TestSize();
  TestClear();
  TestInsert();

  return 0;
}
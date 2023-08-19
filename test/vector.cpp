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

class X {
public:
  X() {
    cout << "Default constructor\n";
  }

  X(const X&) {
    cout << "Copy constructor\n";
  }
};

void TestConstructor() {
  cout << "==========TEST CONSTRUCTOR==========\n";
  using std::string;
  stl::vector<string> words1{"momo", "yoona", "yejin", "joy", "yang"};
  cout << "1: " << words1;

  stl::vector<string> words2(words1.begin(), words1.end());
  words2[2] = "bc";
  assert(words1[2] != words2[2]);
  cout << "2: " << words2;

  stl::vector<string> words3(words1);
  cout << "3: " << words3;

  stl::vector<string> words4(5, "momo");
  cout << "4: " << words4;

  stl::vector<string> words5(std::move(words1));
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

  stl::vector<std::string> strs1 = {"abc", "def", "qwe"};
  stl::vector<std::string> strs2;
  assert(strs2.size() == 0);
  strs2 = strs1;
  strs2[0] = "xyz";
  assert(strs1[0] != strs2[0]);
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
  for (decltype(container.size()) i = 0; i < container.size(); i++) {
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
  stl::vector<int> nums{1, 3, 5, 7};
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

  stl::vector<std::string> data2;
  data2.insert(data2.begin(), "abc");
  assert(data2.size() == 1);
  assert(data2[0] == "abc");
  data2.insert(data2.end(), "def");
  data2.insert(data2.begin() + 1, "ghi");
  assert(data2[1] == "ghi");
  assert(data2[2] == "def");
  data2.insert(data2.end(), "xyz");
  assert(data2[3] == "xyz");
  cout << "PASS\n";
}

struct A {
  std::string s;
  A() = default;

  A(std::string str) : s(std::move(str)) { cout << " constructed\n"; }

  A(const A& o) : s(o.s) { cout << " copy constructed\n"; }

  A(A&& o) : s(std::move(o.s)) { cout << " move constructed\n"; }

  A& operator=(const A& other) {
    s = other.s;
    cout << " copy assigned\n";
    return *this;
  }

  A& operator=(A&& other) {
    s = std::move(other.s);
    cout << " move assigned\n";
    return *this;
  }
};

void TestEmplace() {
  cout << "==========TEST EMPLACE FUNCTION==========\n";
  stl::vector<A> container;
  // reserve enough place so vector does not have to resize
  container.reserve(10);
  cout << "construct 2 times A:\n";
  A two{"two"};
  A three{"three"};
  assert(container.capacity() == 10);

  cout << "emplace:\n";
  container.emplace(container.end(), "one");

  cout << "emplace with A&:\n";
  container.emplace(container.end(), two);

  cout << "emplace with A&&:\n";
  container.emplace(container.end(), std::move(three));

  cout << "content:\n";
  for (const auto& obj : container) {
    cout << ' ' << obj.s;
  }
  cout << '\n';
  cout << "PASS\n";
}

void TestErase() {
  cout << "==========TEST ERASE FUNCTION==========\n";
  stl::vector<int> c{0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
  cout << c;

  c.erase(c.begin());
  cout << c;

  // Erase all even numbers
  for (auto it = c.begin(); it != c.end();) {
    if (*it % 2 == 0)
      it = c.erase(it);
    else
      ++it;
  }
  cout << c;

  stl::vector<std::string> data;
  data.insert(data.begin(), "abc");
  data.insert(data.end(), "def");
  data.insert(data.begin() + 1, "ghi");
  data.insert(data.begin(), "xyz");
  data.erase(data.begin());
  assert(data[0] == "abc");
  data.erase(data.begin());
  assert(data[0] == "ghi");
  cout << "PASS\n";
}

void TestPushBack() {
  cout << "==========TEST PUSH BACK FUNCTION==========\n";
  stl::vector<std::string> letters;
  letters.push_back("abc");
  std::string s{"def"};
  letters.push_back(std::move(s));
  cout << letters;
  assert(s == "");

  stl::vector<std::string> data1;
  data1.push_back("abc");
  assert(data1[0] == "abc");

  int i = data1.size();
  for (; i < 10; i++) {
    data1.push_back("def");
  }
  assert(data1[i - 1] == "def");
  cout << "PASS\n";
}

struct President {
  std::string name;
  std::string country;
  int year;

  President() {
    cout << "Default constructed\n";
  }

  President(std::string p_name, std::string p_country, int p_year)
      : name(std::move(p_name)), country(std::move(p_country)), year(p_year) {
    cout << "I am being constructed.\n";
  }

  President(President&& other)
      : name(std::move(other.name)),
        country(std::move(other.country)),
        year(other.year) {
    cout << "I am being moved.\n";
  }

  President& operator=(const President& other) = default;
};

void TestEmplaceBack() {
  cout << "==========TEST EMPLACE BACK FUNCTION==========\n";
  stl::vector<President> elections;
  cout << "emplace_back:\n";
  auto& ref = elections.emplace_back("Nelson Mandela", "South Africa", 1994);
  assert(ref.year == 1994 && "uses a reference to the created object (C++17)");
  assert(ref.country == "South Africa");
  assert(ref.name == "Nelson Mandela");

  stl::vector<President> reElections;
  cout << "\npush_back:\n";
  reElections.push_back(
      President("Franklin Delano Roosevelt", "the USA", 1936));

  cout << "\nContents:\n";
  for (President const& president : elections)
    cout << president.name << " was elected president of " << president.country
         << " in " << president.year << ".\n";

  for (President const& president : reElections)
    cout << president.name << " was re-elected president of "
         << president.country << " in " << president.year << ".\n";

  // Test ADL
  stl::vector<std::string> data;
  std::string s = "abc";
  data.emplace_back(s);
  
  cout << "PASS\n";
}

void TestPopBack() {
  cout << "==========TEST POP BACK FUNCTION==========\n";
  stl::vector<int> numbers;
  numbers.push_back(5);
  numbers.push_back(3);
  numbers.push_back(4);

  cout << numbers;
  numbers.pop_back();
  cout << numbers;
  cout << "PASS\n";
}

void print(auto rem, const stl::vector<int>& c) {
  for (cout << rem; const int el : c) cout << el << ' ';
  cout << '\n';
}

void TestResize() {
  cout << "==========TEST RESIZE FUNCTION==========\n";
  stl::vector<int> c = {1, 2, 3};
  print("The vector holds: ", c);

  c.resize(5);
  print("After resize up to 5: ", c);

  c.resize(2);
  print("After resize down to 2: ", c);

  c.resize(6, 4);
  print("After resize up to 6 (initializer = 4): ", c);
  cout << "PASS\n";
}

int main() {
  // TestConstructor();
  // TestAssignment();
  // TestAssign();
  // TestAt();
  // TestIndexingOperator();
  // TestFront();
  // TestBack();
  // TestData();
  // TestIterators();
  // TestEmpty();
  // TestSize();
  // TestClear();
  
  // Test modifiers
  // TestInsert();
  // TestEmplace();
  // TestErase();
  // TestPushBack();
  // TestEmplaceBack();
  // TestPopBack();
  // TestResize();

  return 0;
}
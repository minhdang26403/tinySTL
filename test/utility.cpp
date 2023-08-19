#include "utility.h"

#include <iomanip>
#include <iostream>
#include <string>

#include "type_traits.h"

#include "vector.h"

using std::cout;

struct A {
  A(int&& n) { cout << "rvalue overload, n=" << n << '\n'; }
  A(int& n) { cout << "lvalue overload, n=" << n << '\n'; }
};

class B {
 public:
  template <class T1, class T2, class T3>
  B(T1&& t1, T2&& t2, T3&& t3)
      : a1_{stl::forward<T1>(t1)},
        a2_{stl::forward<T2>(t2)},
        a3_{stl::forward<T3>(t3)} {}

 private:
  A a1_, a2_, a3_;
};

template <class T, class U>
std::unique_ptr<T> make_unique1(U&& u) {
  return std::unique_ptr<T>(new T(stl::forward<U>(u)));
}

template <class T, class... U>
std::unique_ptr<T> make_unique2(U&&... u) {
  return std::unique_ptr<T>(new T(stl::forward<U>(u)...));
}

auto make_B(auto&&... args)  // since C++20
{
  return B(stl::forward<decltype(args)>(args)...);
}

void TestForward() {
  cout << "==========TEST FORWARD FUNCTION==========\n";
  auto p1 = make_unique1<A>(2);  // rvalue
  int i = 1;
  auto p2 = make_unique1<A>(i);  // lvalue

  cout << "B\n";
  auto t = make_unique2<B>(2, i, 3);

  cout << "make_B\n";
  [[maybe_unused]] B b = make_B(4, i, 5);
  cout << "PASS\n";
}

void TestMove() {
  cout << "==========TEST MOVE FUNCTION==========\n";
  std::string str = "Salut";
  stl::vector<std::string> v;

  // uses the push_back(const T&) overload, which means
  // we'll incur the cost of copying str
  v.push_back(str);
  assert(str == "Salut");

  // uses the rvalue reference push_back(T&&) overload,
  // which means no strings will be copied; instead, the contents
  // of str will be moved into the vector. This is less
  // expensive, but also means str might now be empty.
  v.push_back(std::move(str));
  assert(str == "");

  cout << "The contents of the vector are { " << std::quoted(v[0]) << ", "
       << std::quoted(v[1]) << " }\n";
  cout << "PASS\n";
}

int main() {
  TestForward();
  TestMove();
}
#include "memory.h"

#include <iostream>

using std::cout;

// default delete
void TestDefaultDelete() {
  {
    std::unique_ptr<int> ptr(new int(5));
  }  // unique_ptr<int> uses default_delete<int>
}

// object to manage
struct Foo {
  Foo() { cout << "Foo ctor\n"; }
  Foo(const Foo&) { cout << "Foo copy ctor\n"; }
  Foo(Foo&&) { cout << "Foo move ctor\n"; }
  ~Foo() { cout << "~Foo dtor\n"; }
  void bar() { cout << "Foo::bar\n"; }
};

// deleter
struct D {
  D(){};
  D(const D&) { cout << "D copy ctor\n"; }
  D(D&) { cout << "D non-const copy ctor\n"; }
  D(D&&) { cout << "D move ctor \n"; }
  void operator()(Foo* p) const {
    cout << "D is deleting a Foo\n";
    delete p;
  };
};

void TestConstructor() {
  cout << "==========TEST CONSTRUCTOR==========\n";
  cout << "Example constructor(1)...\n";
  stl::unique_ptr<Foo> up1;            // up1 is empty
  stl::unique_ptr<Foo> up1b(nullptr);  // up1b is empty

  cout << "Example constructor(2)...\n";
  {
    stl::unique_ptr<Foo> up2(new Foo);  // up2 now owns a Foo
  }                                     // Foo deleted

  cout << "Example constructor(5)...\n";
  {
    stl::unique_ptr<Foo> up5a(new Foo);
    stl::unique_ptr<Foo> up5b(std::move(up5a));  // ownership transfer
  }

  cout << "Example array constructor...\n";
  { stl::unique_ptr<Foo[]> up(new Foo[3]); }  // three Foo objects deleted

  cout << "PASS\n";
}

void TestDestructor() {
  cout << "==========TEST DESTRUCTOR==========\n";
  auto deleter = [](int* ptr) {
    cout << "[deleter called]\n";
    delete ptr;
  };

  std::unique_ptr<int, decltype(deleter)> uniq(new int, deleter);
  cout << (uniq ? "not empty\n" : "empty\n");
  uniq.reset();
  cout << (uniq ? "not empty\n" : "empty\n");

  cout << "PASS\n";
}

struct Bar {
  int id;
  Bar(int id) : id(id) { std::cout << "Bar " << id << '\n'; }
  ~Bar() { std::cout << "~Bar " << id << '\n'; }
};

void TestAssignmentOperator() {
  cout << "==========TEST ASSIGNMENT OPERATOR==========\n";
  stl::unique_ptr<Bar> p1(stl::make_unique<Bar>(1));

  {
    cout << "Creating new Bar...\n";
    stl::unique_ptr<Bar> p2(stl::make_unique<Bar>(2));
    // p1 = p2; // Error ! can't copy unique_ptr
    p1 = stl::move(p2);
    cout << "About to leave inner block...\n";

    // Foo instance will continue to live,
    // despite p2 going out of scope
  }

  cout << "About to leave program...\n";

  cout << "PASS\n";
}

// Ownership of the Foo resource is transferred when calling this function
void legacy_api(Foo* owning_foo) {
  cout << __func__ << '\n';
  // [legacy code that no one understands or dares touch anymore]
  // [...]
  delete owning_foo;
}

void TestRelease() {
  cout << "==========TEST RELEASE FUNCTION==========\n";
  stl::unique_ptr<Foo> managed_foo(new Foo);
  // [code that might return or throw or some such]
  // [...]
  legacy_api(managed_foo.release());

  assert(managed_foo == nullptr);

  cout << "PASS\n";
}

void TestReset() {
  cout << "==========TEST RESET FUNCTION==========\n";
  cout << "Creating new Foo...\n";
  stl::unique_ptr<Foo> up(new Foo());  // up owns the Foo pointer (deleter D)

  cout << "Replace owned Foo with a new Foo...\n";
  up.reset(new Foo());  // calls deleter for the old one

  cout << "Release and delete the owned Foo...\n";
  up.reset(nullptr);

  cout << "PASS\n";
}

struct Momo {
  Momo(int _val) : val(_val) { cout << "Momo...\n"; }
  ~Momo() { cout << "~Momo...\n"; }
  int val;
};

void TestSwap() {
  cout << "==========TEST SWAP FUNCTION==========\n";
  int val1 = 1, val2 = 2;
  stl::unique_ptr<Momo> up1(new Momo(val1));
  stl::unique_ptr<Momo> up2(new Momo(val2));

  up1.swap(up2);
  assert(up1->val == val2);
  assert(up2->val == val1);

  cout << "PASS\n";
}

class Res {
  std::string s;

 public:
  Res(std::string arg) : s{stl::move(arg)} {
    cout << "Res::Res(" << s << ");\n";
  }

  ~Res() { cout << "Res::~Res();\n"; }

 private:
  friend std::ostream& operator<<(std::ostream& os, Res const& r) {
    return os << "Res { s = " << r.s << "; }";
  }
};

void TestGet() {
  cout << "==========TEST GET FUNCTION==========\n";
  stl::unique_ptr<Res> up(new Res{"Hello, world!"});
  Res* res = up.get();
  cout << *res << '\n';

  cout << "PASS\n";
}

void TestOperatorBool() {
  cout << "==========TEST OPERATOR BOOL==========\n";
  stl::unique_ptr<int> ptr(new int(42));

  if (ptr) cout << "before reset, ptr is: " << *ptr << '\n';
  ptr.reset();
  ptr ? (cout << "after reset, ptr is: " << *ptr)
      : (cout << "after reset ptr is empty") << '\n';

  cout << "PASS\n";
}

void f(const Foo&) { cout << "f(const Foo&)\n"; }

void TestAccessMethod() {
  cout << "==========TEST ACCESS METHOD==========\n";
  stl::unique_ptr<Foo> ptr(new Foo);

  ptr->bar();
  f(*ptr);

  const int size = 10;
  stl::unique_ptr<int[]> fact(new int[size]);

  for (int i = 0; i < size; ++i) {
    fact[i] = (i == 0) ? 1 : i * fact[i - 1];
  }

  for (int i = 0; i < size; ++i) {
    cout << i << "! = " << fact[i] << '\n';
  }

  cout << "PASS\n";
}

int main() {
  TestDefaultDelete();
  TestConstructor();
  // TestDestructor();
  TestAssignmentOperator();
  TestRelease();
  TestReset();
  TestSwap();
  TestGet();
  TestOperatorBool();
  TestAccessMethod();

  return 0;
}
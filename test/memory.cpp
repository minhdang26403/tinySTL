#include "memory.h"

#include <iostream>

using namespace stl;

// default delete
void TestDefaultDelete() {
  {
    std::unique_ptr<int> ptr(new int(5));
  }  // unique_ptr<int> uses default_delete<int>
}

// object to manage
struct Foo {
  Foo() { std::cout << "Foo ctor\n"; }
  Foo(const Foo&) { std::cout << "Foo copy ctor\n"; }
  Foo(Foo&&) { std::cout << "Foo move ctor\n"; }
  ~Foo() { std::cout << "~Foo dtor\n"; }
  void bar() { std::cout << "Foo::bar\n"; }
};

// deleter
struct D {
  D(){};
  D(const D&) { std::cout << "D copy ctor\n"; }
  D(D&) { std::cout << "D non-const copy ctor\n"; }
  D(D&&) { std::cout << "D move ctor \n"; }
  void operator()(Foo* p) const {
    std::cout << "D is deleting a Foo\n";
    delete p;
  };
};

void TestConstructor() {
  std::cout << "Example constructor(1)...\n";
  unique_ptr<Foo> up1;            // up1 is empty
  unique_ptr<Foo> up1b(nullptr);  // up1b is empty

  std::cout << "Example constructor(2)...\n";
  {
    unique_ptr<Foo> up2(new Foo);  // up2 now owns a Foo
  }                                // Foo deleted

  std::cout << "Example constructor(5)...\n";
  {
    unique_ptr<Foo> up5a(new Foo);
    unique_ptr<Foo> up5b(std::move(up5a));  // ownership transfer
  }
}

void f(const Foo&) { std::cout << "f(const Foo&)\n"; }

void TestAccessMethod() {
  unique_ptr<Foo> ptr(new Foo);

  ptr->bar();
  f(*ptr);
}

class Res {
  std::string s;

 public:
  Res(std::string arg) : s{std::move(arg)} {
    std::cout << "Res::Res(" << s << ");\n";
  }

  ~Res() { std::cout << "Res::~Res();\n"; }

 private:
  friend std::ostream& operator<<(std::ostream& os, Res const& r) {
    return os << "Res { s = " << r.s << "; }";
  }
};

void TestGet() {
  unique_ptr<Res> up(new Res{"Hello, world!"});
  Res* res = up.get();
  std::cout << *res << '\n';
}

int main() {
  TestDefaultDelete();
  TestConstructor();
  TestAccessMethod();
  TestGet();

  return 0;
}
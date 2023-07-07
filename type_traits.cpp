#include <iostream>

#include "type_traits.h"

// integral_constant
void TestIntegralConstant() {
  using stl::integral_constant;
  using stl::is_same_v;
  using two_t = integral_constant<int, 2>;
  using four_t = integral_constant<int, 4>;

  static_assert(!is_same_v<two_t, four_t>, "two_t and four_t are equal!");

  static_assert(two_t::value * 2 == four_t::value, "2*2 != 4");

  enum class my_e { e1, e2 };

  using my_e_e1 = integral_constant<my_e, my_e::e1>;
  using my_e_e2 = integral_constant<my_e, my_e::e2>;

  static_assert(my_e_e1() == my_e::e1);

  static_assert(my_e_e1::value != my_e::e2, "my_e_e1::value == my_e::e2");

  static_assert(is_same_v<my_e_e2, my_e_e2>, "my_e_e2 != my_e_e2");
}

// is_void
void TestIsVoid() {
  using stl::is_void_v;
  static_assert(is_void_v<void>);
  static_assert(!is_void_v<int>);
  static_assert(is_void_v<const void>);
}

// is_null_pointer
void TestIsNullPointer() {
  using stl::is_null_pointer_v;
  static_assert(is_null_pointer_v<decltype(nullptr)>);
  static_assert(!is_null_pointer_v<int*>);
}

// is_integral
namespace testIsIntegral {
class A {};

struct B {
  int x : 4 {2};
};
using BF = decltype(B::x);  // bit-field's type

enum E : int {};

template <class T>
T f(T i) {
  static_assert(stl::is_integral<T>::value, "Integral required.");
  return i;
}
}  // namespace testIsIntegral

void TestIsIntegral() {
  using namespace testIsIntegral;
  using stl::is_integral_v;
  static_assert(!is_integral_v<A>);
  static_assert(!is_integral_v<E>);
  static_assert(!is_integral_v<float>);
  static_assert(!is_integral_v<int*>);
  static_assert(is_integral_v<int>);
  static_assert(is_integral_v<const int>);
  static_assert(is_integral_v<bool>);
  static_assert(is_integral_v<char>);
  static_assert(is_integral_v<BF>);
}

// is_floating_point
void TestIsFloatingPoint() {
  class A {};
  using stl::is_floating_point_v;
  static_assert(!is_floating_point_v<A>);
  static_assert(is_floating_point_v<float>);
  static_assert(!is_floating_point_v<float&>);
  static_assert(is_floating_point_v<double>);
  static_assert(!is_floating_point_v<double&>);
  static_assert(!is_floating_point_v<int>);
}

// is_array
void TestIsArray() {
  class A {};
  using stl::is_array_v;
  static_assert(is_array_v<A[]>);
  static_assert(!is_array_v<A>);
  static_assert(is_array_v<A[3]>);
  static_assert(!is_array_v<float>);
  static_assert(!is_array_v<int>);
  static_assert(is_array_v<int[]>);
  static_assert(is_array_v<int[3]>);
  static_assert(!is_array_v<std::array<int, 3>>);
}

// is_enum
void TestIsEnum() {
  struct A {
    enum E {};
  };
  enum E {};
  enum class Ec : int {};
  using stl::is_enum;
  using stl::is_enum_v;

  static_assert(!is_enum_v<A>);
  static_assert(is_enum_v<E>);
  static_assert(is_enum<A::E>());
  static_assert(!is_enum_v<int>);
  static_assert(is_enum_v<Ec>);
}

// is_union
void TestIsUnion() {
  struct A {};
  typedef union {
    int a;
    float b;
  } B;
  struct C {
    B d;
  };
  using stl::is_union_v;

  static_assert(!is_union_v<A>);
  static_assert(is_union_v<B>);
  static_assert(!is_union_v<C>);
  static_assert(!is_union_v<int>);
}

// is_class
void TestIsClass() {
  struct A {};
  class B {};
  enum class E {};
  union U {
    class UC {};
  };
  using stl::is_class_v;

  static_assert(!is_class_v<U>);
  static_assert(is_class_v<U::UC>);

  static_assert(is_class_v<A>);
  static_assert(is_class_v<B>);
  static_assert(!is_class_v<B*>);
  static_assert(!is_class_v<B&>);
  static_assert(is_class_v<const B>);
  static_assert(!is_class_v<E>);
  static_assert(!is_class_v<int>);
  static_assert(is_class_v<struct S>);
  static_assert(is_class_v<class C>);
}

// is_function
namespace testIsFunction {
struct A {
  int fun() const&;
};
template <typename>
struct PM_traits {};
template <class T, class U>
struct PM_traits<U T::*> {
  using member_type = U;
};
int f();
}  // namespace testIsFunction

void TestIsFunction() {
  using namespace testIsFunction;
  using stl::is_function_v;
  static_assert(!is_function_v<A>);
  static_assert(is_function_v<int(int)>);
  static_assert(is_function_v<decltype(f)>);
  static_assert(!is_function_v<int>);

  using T = PM_traits<decltype(&A::fun)>::member_type;  // T is int() const&
  static_assert(is_function_v<T>);
}

// is_pointer
void TestIsPointer() {
  struct A {
    int m;
    void f() {}
  };
  int A::*mem_data_ptr = &A::m;
  void (A::*mem_fun_ptr)() = &A::f;
  using stl::is_pointer;
  using stl::is_pointer_v;

  static_assert(!is_pointer_v<A>);
  static_assert(!is_pointer_v<A>);
  static_assert(!is_pointer<A>());
  static_assert(!is_pointer<A>{});
  static_assert(!is_pointer<A>()());
  static_assert(!is_pointer<A>{}());
  static_assert(is_pointer_v<A*>);
  static_assert(is_pointer_v<A const* volatile>);
  static_assert(!is_pointer_v<A&>);
  static_assert(!is_pointer_v<decltype(mem_data_ptr)>);
  static_assert(!is_pointer_v<decltype(mem_fun_ptr)>);
  static_assert(is_pointer_v<void*>);
  static_assert(!is_pointer_v<int>);
  static_assert(is_pointer_v<int*>);
  static_assert(is_pointer_v<int**>);
  static_assert(!is_pointer_v<int[10]>);
  static_assert(!is_pointer_v<stl::nullptr_t>);
}

// is_lvalue_reference
void TestIsLvalueReference() {
  class A {};
  using stl::is_lvalue_reference_v;
  static_assert(!is_lvalue_reference_v<A>);
  static_assert(is_lvalue_reference_v<A&>);
  static_assert(!is_lvalue_reference_v<A&&>);
  static_assert(!is_lvalue_reference_v<int>);
  static_assert(is_lvalue_reference_v<int&>);
  static_assert(!is_lvalue_reference_v<int&&>);
  static_assert(is_lvalue_reference_v<int&>);
}

// is_rvalue_reference
void TestIsRvalueReference() {
  class A {};
  using stl::is_rvalue_reference_v;
  static_assert(!is_rvalue_reference_v<A>);
  static_assert(!is_rvalue_reference_v<A&>);
  static_assert(is_rvalue_reference_v<A&&>);
  static_assert(!is_rvalue_reference_v<int>);
  static_assert(!is_rvalue_reference_v<int&>);
  static_assert(is_rvalue_reference_v<int&&>);
}

// is_member_object_pointer
void TestIsMemberObjectPointer() {
  class C {};
  using stl::is_member_object_pointer_v;
  static_assert(is_member_object_pointer_v<int(C::*)>);
  static_assert(!is_member_object_pointer_v<int (C::*)()>);
}

// is_member_function_pointer
void TestIsMemberFunctionPointer() {
  class A {
   public:
    void member() {}
  };

  static_assert(stl::is_member_function_pointer_v<decltype(&A::member)>);
}

// is_fundamental
void TestIsFundamental() {
  class A {};
  using stl::is_fundamental_v;
  static_assert(!is_fundamental_v<A>);
  static_assert(is_fundamental_v<int>);
  static_assert(!is_fundamental_v<int&>);
  static_assert(!is_fundamental_v<int*>);
  static_assert(is_fundamental_v<float>);
  static_assert(!is_fundamental_v<float&>);
  static_assert(!is_fundamental_v<float*>);
}

// is_arithmetic
void TestIsArithmetic() {
  class A {};
  using stl::is_arithmetic_v;

  static_assert(
      is_arithmetic_v<A> == false && is_arithmetic_v<bool> == true &&
      is_arithmetic_v<int> == true && is_arithmetic_v<int const> == true &&
      is_arithmetic_v<int&> == false && is_arithmetic_v<int*> == false &&
      is_arithmetic_v<float> == true && is_arithmetic_v<float const> == true &&
      is_arithmetic_v<float&> == false && is_arithmetic_v<float*> == false &&
      is_arithmetic_v<char> == true && is_arithmetic_v<char const> == true &&
      is_arithmetic_v<char&> == false && is_arithmetic_v<char*> == false);
}

// is_scalar
void TestIsScalar() {
  struct S {
    int m;
  } s;
  int S::*mp = &S::m;
  enum class E { e };
  using stl::is_scalar_v;

  static_assert(is_scalar_v<decltype(42)>);
  static_assert(is_scalar_v<decltype(3.14)>);
  static_assert(is_scalar_v<decltype(E::e)>);
  static_assert(is_scalar_v<decltype(mp)>);
  static_assert(is_scalar_v<decltype(nullptr)>);
  static_assert(is_scalar_v<decltype(nullptr)>);
}

// is_object
void TestIsObject() {
  class cls {};
  using stl::is_object_v;
  static_assert(!is_object_v<void>);
  static_assert(is_object_v<int>);
  static_assert(!is_object_v<int&>);
  static_assert(is_object_v<int*>);
  static_assert(is_object_v<cls>);
  static_assert(!is_object_v<cls&>);
  static_assert(is_object_v<cls*>);
  static_assert(!is_object_v<int()>);
  static_assert(is_object_v<int (*)()>);
}

// is_compound
void TestIsCompound() {
  struct S {
    int i : 8;
    int j;
    void foo();
  };
  using stl::is_compound_v;
  static_assert(!is_compound_v<decltype(S::i)>);
  static_assert(!is_compound_v<decltype(S::j)>);
  static_assert(is_compound_v<decltype(&S::j)>);
  static_assert(is_compound_v<decltype(&S::foo)>);
}

// is_reference
void TestIsReference() {
  class A {};
  using stl::is_reference_v;
  static_assert(!is_reference_v<A>);
  static_assert(is_reference_v<A&>);
  static_assert(is_reference_v<A&&>);
  static_assert(!is_reference_v<long>);
  static_assert(is_reference_v<long&>);
  static_assert(is_reference_v<long&&>);
  static_assert(!is_reference_v<double*>);
  static_assert(is_reference_v<double*&>);
  static_assert(is_reference_v<double*&&>);
}

// is_member_pointer
void TestIsMemberPointer() {
  class C {};
  using stl::is_member_pointer_v;
  static_assert(is_member_pointer_v<int(C::*)>);
  static_assert(is_member_pointer_v<int(C::*)>);
  static_assert(is_member_pointer_v<int (C::*)()>);
  static_assert(!is_member_pointer_v<void>);
}

// is_const
void TestIsConst() {
  using stl::is_const_v;
  static_assert(!is_const_v<int>);
  static_assert(is_const_v<const int>);
  static_assert(!is_const_v<const int*>);
  static_assert(is_const_v<int* const>);
  static_assert(!is_const_v<const int&>);
  static_assert(is_const_v<int* const>);
}

// is_volatile
void TestIsVolatile() {
  using stl::is_volatile_v;
  static_assert(!is_volatile_v<int>);
  static_assert(is_volatile_v<volatile int>);
  static_assert(is_volatile_v<volatile const int>);
}

// is_trivial
void TestIsTrivial() {
  struct A {
    int m;
  };
  struct B {
    B() {}
  };
  using stl::is_trivial_v;
  static_assert(is_trivial_v<A>);
  static_assert(!is_trivial_v<B>);
}

// is_trivially_copyable
void TestIsTriviallyCopyable() {
  struct A {
    int m;
  };
  struct B {
    B(B const&) {}
  };
  struct C {
    virtual void foo();
  };
  struct D {
    int m;
    D(D const&) = default;  // -> trivially copyable
    D(int x) : m(x + 1) {}
  };
  using stl::is_trivially_copyable_v;
  static_assert(is_trivially_copyable_v<A>);
  static_assert(!is_trivially_copyable_v<B>);
  static_assert(!is_trivially_copyable_v<C>);
  static_assert(is_trivially_copyable_v<D>);
}

// is_standard_layout
void TestIsStandardLayout() {
  struct A {
    int m;
  };
  struct B {
    int m1;

   private:
    int m2;
  };
  struct C {
    virtual void foo();
  };
  using stl::is_standard_layout_v;

  static_assert(is_standard_layout_v<A>);
  static_assert(!is_standard_layout_v<B>);
  static_assert(!is_standard_layout_v<C>);
}

// is_pod
void TestIsPod() {
  struct A {
    int m;
  };
  struct B {
    int m1;

   private:
    int m2;
  };
  struct C {
    virtual void foo();
  };
  using stl::is_pod_v;

  static_assert(is_pod_v<A>);
  static_assert(!is_pod_v<B>);
  static_assert(!is_pod_v<C>);
}

// is_empty
void TestIsEmpty() {
  using stl::is_empty_v;

  struct A {};
  struct B {
    int m;
  };
  struct D {
    virtual ~D();
  };
  union E {};

  static_assert(is_empty_v<D> == false);
  static_assert(is_empty_v<A> == true);
  static_assert(is_empty_v<B> == false);
  static_assert(is_empty_v<E> == false);
}

// is_polymorphic
void TestIsPolymorphic() {
  using stl::is_polymorphic_v;
  struct A {
    int m;
  };
  static_assert(!is_polymorphic_v<A>);

  struct B {
    virtual void foo();
  };
  static_assert(is_polymorphic_v<B>);

  struct C : B {};
  static_assert(is_polymorphic_v<C>);

  struct D {
    virtual ~D() = default;
  };
  static_assert(is_polymorphic_v<D>);

  // Uses inheritance, but not the virtual keyword:
  struct E : A {};
  static_assert(!is_polymorphic_v<E>);

  struct F : virtual A {};
  static_assert(!is_polymorphic_v<F>);

  struct AX : A {};
  struct AY : A {};
  struct XY : virtual AX, virtual AY {};
  static_assert(!is_polymorphic_v<XY>);
}

// is_abstract
void TestIsAbstract() {
  struct A {
    int m;
  };
  struct B {
    virtual void foo();
  };
  struct C {
    virtual void foo() = 0;
  };
  struct D : C {};
  using stl::is_abstract_v;

  static_assert(!is_abstract_v<A>);
  static_assert(!is_abstract_v<B>);
  static_assert(is_abstract_v<C>);
  static_assert(is_abstract_v<D>);
}

// is_final
void TestIsFinal() {
  class A {};
  class B final {};
  using stl::is_final_v;

  static_assert(!is_final_v<A>);
  static_assert(is_final_v<B>);
}

// is_aggregate
void TestIsAggregate() {
  struct A {
    int x, y;
  };
  struct B {
    B(int, const char*) {}
  };

  using stl::is_aggregate_v;
  static_assert(is_aggregate_v<A>);
  static_assert(!is_aggregate_v<B>);
}

// is_signed
void TestIsSigned() {
  class A {};
  enum B : int {};
  enum class C : int {};
  using stl::is_signed;
  using stl::is_signed_v;

  static_assert(is_signed_v<A> == false);
  static_assert(is_signed_v<float> == true);
  static_assert(is_signed_v<signed int> == true);
  static_assert(is_signed_v<unsigned int> == false);
  static_assert(is_signed_v<B> == false);
  static_assert(is_signed_v<C> == false);

  static_assert(is_signed_v<bool> == false);
  static_assert(is_signed<signed int>() == true);
  static_assert(is_signed<unsigned int>{} == false);
  static_assert(is_signed_v<signed char> == true);
  static_assert(is_signed_v<unsigned char> == false);
}

// is_unsigned
void TestIsUnsigned() {
  using stl::is_unsigned_v;
  class A {};
  static_assert(is_unsigned_v<A> == false);
  enum B : unsigned {};
  static_assert(is_unsigned_v<B> == false);
  enum class C : unsigned {};
  static_assert(is_unsigned_v<C> == false);
  struct S {
    unsigned p : 1;
    int q : 1;
  };
  static_assert(is_unsigned_v<decltype(S::p)> not_eq
                is_unsigned_v<decltype(S::q)>);
  static_assert(
      is_unsigned_v<float> == false && is_unsigned_v<signed int> == false &&
      is_unsigned_v<unsigned int> == true && is_unsigned_v<bool> == true);
}

// is_bounded_array
void TestIsBoundedArray() {
  class A {};
  using stl::is_bounded_array_v;
  static_assert(!is_bounded_array_v<A>);
  static_assert(!is_bounded_array_v<A[]>);
  static_assert(is_bounded_array_v<A[3]>);
  static_assert(!is_bounded_array_v<float>);
  static_assert(!is_bounded_array_v<int>);
  static_assert(!is_bounded_array_v<int[]>);
  static_assert(is_bounded_array_v<int[3]>);
}

// is_unbounded_array
void TestIsUnboundedArray() {
  class A {};
  using stl::is_unbounded_array_v;
  static_assert(!is_unbounded_array_v<A>);
  static_assert(is_unbounded_array_v<A[]>);
  static_assert(!is_unbounded_array_v<A[3]>);
  static_assert(!is_unbounded_array_v<float>);
  static_assert(!is_unbounded_array_v<int>);
  static_assert(is_unbounded_array_v<int[]>);
  static_assert(!is_unbounded_array_v<int[3]>);
}

// is_default_constructible / is_trivially_constructible /
// is_nothrow_constructible
void TestIsConstructible() {
  class Foo {
    int v1;
    double v2;

   public:
    Foo(int n) : v1(n), v2() {}
    Foo(int n, double f) noexcept : v1(n), v2(f) {}
  };
  using stl::is_constructible_v;
  using stl::is_trivially_constructible_v;

  static_assert(is_constructible_v<Foo, int>);
  static_assert(is_trivially_constructible_v<Foo, const Foo&>);
  static_assert(!is_trivially_constructible_v<Foo, int>);
}

// is_default_constructible / is_trivially_default_constructible /
// is_nothrow_default_constructible

void TestIsDefaultConstructible() {
  struct Ex1 {
    std::string str;  // member has a non-trivial default ctor
  };

  struct Ex2 {
    int n;
    Ex2() = default;  // trivial and non-throwing
  };
  using stl::is_default_constructible_v;
  using stl::is_nothrow_default_constructible_v;
  using stl::is_trivially_default_constructible_v;

  static_assert(is_default_constructible_v<Ex1>);
  static_assert(!is_trivially_default_constructible_v<Ex1>);
  static_assert(is_trivially_default_constructible_v<Ex2>);
  static_assert(is_nothrow_default_constructible_v<Ex2>);
}

// is_copy_constructible / is_trivially_copy_constructible /
// is_nothrow_copy_constructible
void TestIsCopyConstructible() {
  struct Ex1 {
    std::string str;  // member has a non-trivial copy ctor
  };

  struct Ex2 {
    int n;
    Ex2(const Ex2&) = default;  // trivial and non-throwing
  };
  using stl::is_copy_constructible_v;
  using stl::is_nothrow_copy_constructible_v;
  using stl::is_trivially_copy_constructible_v;

  static_assert(is_copy_constructible_v<Ex1>);
  static_assert(!is_trivially_copy_constructible_v<Ex1>);
  static_assert(is_trivially_copy_constructible_v<Ex2>);
  static_assert(is_nothrow_copy_constructible_v<Ex2>);
}

// is_move_constructible / is_trivially_move_constructible /
// is_nothrow_move_constructible
void TestIsMoveConstructible() {
  struct Ex1 {
    std::string str;  // member has a non-trivial but non-throwing move ctor
  };
  struct Ex2 {
    int n;
    Ex2(Ex2&&) = default;  // trivial and non-throwing
  };
  struct NoMove {
    // prevents implicit declaration of default move constructor
    // however, the class is still move-constructible because its
    // copy constructor can bind to an rvalue argument
    NoMove(const NoMove&) {}
  };

  using stl::is_move_constructible_v;
  using stl::is_nothrow_move_constructible_v;
  using stl::is_trivially_move_constructible_v;

  static_assert(is_move_constructible_v<Ex1>);
  static_assert(!is_trivially_move_constructible_v<Ex1>);
  static_assert(is_nothrow_move_constructible_v<Ex1>);
  static_assert(is_trivially_move_constructible_v<Ex2>);
  static_assert(is_nothrow_move_constructible_v<Ex2>);
  static_assert(is_move_constructible_v<NoMove>);
  static_assert(!is_nothrow_move_constructible_v<NoMove>);
}

// is_assignable / is_trivially_assignable / is_nothrow_assignable
void TestIsAssignable() {
  struct Ex1 {
    int n;
  };
  using stl::is_assignable_v;
  using stl::is_nothrow_assignable_v;
  using stl::is_trivially_assignable_v;

  static_assert(!is_assignable_v<int, int>);  // 1 = 1; wouldn't compile
  static_assert(is_assignable_v<int&, int>);  // int a; a = 1; works
  static_assert(!is_assignable_v<int, double>);
  static_assert(is_nothrow_assignable_v<int&, double>);
  static_assert(is_assignable_v<std::string, double>);
  static_assert(is_trivially_assignable_v<Ex1&, const Ex1&>);
}

// is_copy_assignable / is_trivially_copy_assignable /
// is_nothrow_copy_assignable
void TestIsCopyAssignable() {
  struct Foo {
    int n;
  };
  using stl::is_copy_assignable_v;
  using stl::is_nothrow_copy_assignable_v;
  using stl::is_trivially_copy_assignable_v;

  static_assert(is_trivially_copy_assignable_v<Foo>);
  static_assert(!is_copy_assignable_v<int[2]>);
  static_assert(is_nothrow_copy_assignable_v<int>);
}

// is_move_assignable / is_trivially_move_assignable /
// is_nothrow_move_assignable
void TestIsMoveAssignable() {
  struct Foo {
    int n;
  };

  struct NoMove {
    // prevents implicit declaration of default move assignment operator
    // however, the class is still move-assignable because its
    // copy assignment operator can bind to an rvalue argument
    NoMove& operator=(const NoMove&) { return *this; }
  };
  using stl::is_move_assignable_v;
  using stl::is_nothrow_move_assignable_v;
  using stl::is_trivially_move_assignable_v;

  static_assert(is_nothrow_move_assignable_v<std::string>);
  static_assert(!is_move_assignable_v<int[2]>);
  static_assert(is_trivially_move_assignable_v<Foo>);
  static_assert(is_move_assignable_v<NoMove>);
  static_assert(!is_nothrow_move_assignable_v<NoMove>);
}

// is_destructible / is_trivially_destructible / is_nothrow_destructible
void TestIsDestructible() {
  struct Foo {
    std::string str;
    ~Foo() noexcept {};
  };
  struct Bar {
    ~Bar() = default;
  };
  using stl::is_destructible_v;
  using stl::is_nothrow_destructible_v;
  using stl::is_trivially_destructible_v;
  static_assert(is_destructible_v<std::string>);
  static_assert(!is_trivially_destructible_v<Foo>);
  static_assert(is_nothrow_destructible_v<Foo>);
  static_assert(is_trivially_destructible_v<Bar>);
}

// has_virtual_destructor
void TestHasVirtualDestructor() {
  using stl::has_virtual_destructor_v;
  struct S {};
  static_assert(!has_virtual_destructor_v<S>);

  struct B {
    virtual ~B() { std::puts("B::~B"); }
  };
  static_assert(has_virtual_destructor_v<B>);

  struct D : B {
    ~D() { std::puts("D::~D"); }
  };
  static_assert(has_virtual_destructor_v<D>);
}

// alignment_of
void TestAlignmentOf() {
  struct A {};
  struct B {
    std::int8_t p;
    std::int16_t q;
  };
  using stl::alignment_of;
  using stl::alignment_of_v;

  static_assert(alignment_of_v<A> == 1);
  static_assert(alignment_of_v<B> == 2);
  static_assert(alignment_of<int>() == 4);
  static_assert(alignment_of_v<double> == 8);
}

// rank
void TestRank() {
  using stl::rank;
  using stl::rank_v;
  static_assert(rank<int>{} == 0 && rank<int[5]>{} == 1 &&
                rank<int[5][5]>{} == 2 && rank<int[][5][5]>{} == 3);

  [[maybe_unused]] int ary[][3] = {{1, 2, 3}};
  // The reason of rank of "ary[0]" is calculated as 0
  static_assert(rank_v<decltype(ary[0])> == 0);
  // is that rank cannot deal with reference type. i.e. int(&)[3]
  static_assert(std::is_same_v<decltype(ary[0]), int(&)[3]>);
  // The solution is to remove reference type
  static_assert(rank_v<stl::remove_cvref_t<decltype(ary[0])>> == 1);
}

// extent
void TestExtent() {
  using stl::extent_v;
  static_assert(extent_v<int[3]> == 3);
  static_assert(extent_v<int[3][4], 0> == 3);
  static_assert(extent_v<int[3][4], 1> == 4);
  static_assert(extent_v<int[3][4], 2> == 0);
  static_assert(extent_v<int[]> == 0);

  const int ints[] = {1, 2, 3, 4};
  static_assert(extent_v<decltype(ints)> == 4);
}

// is_same
void TestIsSame() {
  using stl::is_same;
  using stl::is_same_v;
  // usually true if 'int' is 32 bit
  static_assert(is_same_v<int, std::int32_t>);  // maybe true
  // possibly true if ILP64 data model is used
  static_assert(!is_same_v<int, std::int64_t>);  // maybe false

  // same tests as above, except using C++17's std::is_same_v<T, U> format
  static_assert(is_same_v<int, std::int32_t>);
  static_assert(!is_same_v<int, std::int64_t>);

  // compare the types of a couple variables
  long double num1 = 1.0;
  long double num2 = 2.0;
  static_assert(is_same_v<decltype(num1), decltype(num2)> == true);

  // 'float' is never an integral type
  static_assert(is_same_v<float, std::int32_t> == false);

  // 'int' is implicitly 'signed'
  static_assert(is_same_v<int, int> == true);
  static_assert(is_same_v<int, unsigned int> == false);
  static_assert(is_same_v<int, signed int> == true);

  // unlike other types, 'char' is neither 'unsigned' nor 'signed'
  static_assert(is_same_v<char, char> == true);
  static_assert(is_same_v<char, unsigned char> == false);
  static_assert(is_same_v<char, signed char> == false);

  // const-qualified type T is not same as non-const T
  static_assert(!is_same<const int, int>());
}

// is_base_of
void TestIsBaseOf() {
  class A {};
  class B : A {};
  class C : B {};
  class D {};
  union E {};
  using stl::is_base_of_v;

  static_assert(is_base_of_v<A, A>);
  static_assert(is_base_of_v<A, B>);
  static_assert(is_base_of_v<A, C>);
  static_assert(!is_base_of_v<A, D>);
  static_assert(!is_base_of_v<B, A>);
  static_assert(!is_base_of_v<E, E>);
  static_assert(!is_base_of_v<int, int>);
}

// is_convertible
void TestIsConvertible() {
  class A {};
  class B : public A {};
  class C {};
  class D {
   public:
    operator C() { return c; }
    C c;
  };
  using stl::is_convertible_v;

  static_assert(is_convertible_v<B*, A*>);
  static_assert(!is_convertible_v<A*, B*>);
  static_assert(is_convertible_v<D, C>);
  static_assert(!is_convertible_v<B*, C*>);
}

// is_invocable / is_invocable_r
auto func2(char) -> int (*)() { return nullptr; }

void TestIsInvocable() {
  using stl::is_invocable_r_v;
  using stl::is_invocable_v;

  static_assert(is_invocable_v<int()>);
  static_assert(!is_invocable_v<int(), int>);
  static_assert(is_invocable_r_v<int, int()>);
  static_assert(!is_invocable_r_v<int*, int()>);
  static_assert(is_invocable_r_v<void, void(int), int>);
  static_assert(!is_invocable_r_v<void, void(int), void>);
  static_assert(is_invocable_r_v<int (*)(), decltype(func2), char>);
  static_assert(!is_invocable_r_v<int (*)(), decltype(func2), void>);
}

// conjunction
// func is enabled if all Ts... have the same type as T
template <typename T, typename... Ts>
stl::enable_if_t<stl::conjunction_v<stl::is_same<T, Ts>...>> func(T, Ts...) {
  std::cout << "all types in pack are T\n";
}

// otherwise
template <typename T, typename... Ts>
stl::enable_if_t<!stl::conjunction_v<stl::is_same<T, Ts>...>> func(T, Ts...) {
  std::cout << "not all types in pack are T\n";
}

template <typename T, typename... Ts>
constexpr bool all_types_are_same = stl::conjunction_v<stl::is_same<T, Ts>...>;

void TestConjunction() {
  static_assert(all_types_are_same<int, int, int>);
  static_assert(not all_types_are_same<int, int&, int>);
  static_assert(stl::conjunction_v<>);
}

// disjunction
// values_equal<a, b, T>::value is true if and only if a == b.
template <auto V1, decltype(V1) V2, typename T>
struct values_equal : stl::bool_constant<V1 == V2> {
  using type = T;
};

// default_type<T>::value is always true
template <typename T>
struct default_type : stl::true_type {
  using type = T;
};

// Now we can use disjunction like a switch statement:
template <int I>
using int_of_size = typename stl::disjunction<  //
    values_equal<I, 1, std::int8_t>,            //
    values_equal<I, 2, std::int16_t>,           //
    values_equal<I, 4, std::int32_t>,           //
    values_equal<I, 8, std::int64_t>,           //
    default_type<void>                          // must be last!
    >::type;

void TestDisjunction() {
  static_assert(sizeof(int_of_size<1>) == 1);
  static_assert(sizeof(int_of_size<2>) == 2);
  static_assert(sizeof(int_of_size<4>) == 4);
  static_assert(sizeof(int_of_size<8>) == 8);
  static_assert(stl::is_same_v<int_of_size<13>, void>);
}

// negation
void TestNegation() {
  using stl::bool_constant;
  using stl::is_same_v;
  using stl::negation;

  static_assert(is_same_v<bool_constant<false>,
                          typename negation<bool_constant<true>>::type>);
  static_assert(is_same_v<bool_constant<true>,
                          typename negation<bool_constant<false>>::type>);
}

// remove_cv
void RemoveCV() {
  using stl::is_same_v;
  using stl::remove_cv_t;
  static_assert(
      is_same_v<remove_cv_t<int>, int> &&
      is_same_v<remove_cv_t<const int>, int> &&
      is_same_v<remove_cv_t<volatile int>, int> &&
      is_same_v<remove_cv_t<const volatile int>, int> &&
      is_same_v<remove_cv_t<const volatile int*>, const volatile int*> &&
      is_same_v<remove_cv_t<const int* volatile>, const int*> &&
      is_same_v<remove_cv_t<int* const volatile>, int*>);
}

// add_cv
void AddCV() {
  struct foo {
    void m() { std::cout << "Non-cv\n"; }
    void m() const { std::cout << "Const\n"; }
    void m() volatile { std::cout << "Volatile\n"; }
    void m() const volatile { std::cout << "Const-volatile\n"; }
  };

  foo{}.m();
  stl::add_const_t<foo>{}.m();
  stl::add_volatile_t<foo>{}.m();
  stl::add_cv_t<foo>{}.m();
}

// remove_reference
void RemoveReference() {
  using stl::is_same_v;
  using stl::remove_reference_t;
  static_assert(is_same_v<int, remove_reference_t<int>>);
  static_assert(is_same_v<int, remove_reference_t<int&>>);
  static_assert(is_same_v<int, remove_reference_t<int&&>>);
  static_assert(is_same_v<const int, remove_reference_t<const int&>>);
}

// add_lvalue_reference / add_rvalue_reference
void AddReference() {
  using non_ref = int;
  using l_ref = typename stl::add_lvalue_reference_t<non_ref>;
  using r_ref = typename stl::add_rvalue_reference_t<non_ref>;
  using void_ref = stl::add_lvalue_reference_t<void>;

  static_assert(stl::is_lvalue_reference_v<non_ref> == false &&
                stl::is_lvalue_reference_v<l_ref> == true &&
                stl::is_rvalue_reference_v<r_ref> == true &&
                stl::is_reference_v<void_ref> == false);
}

// remove_pointer
void RemovePointer() {
  using stl::is_same_v;
  using stl::remove_pointer_t;
  static_assert(is_same_v<int, int> == true && is_same_v<int, int*> == false &&
                is_same_v<int, int**> == false &&
                is_same_v<int, remove_pointer_t<int>> == true &&
                is_same_v<int, remove_pointer_t<int*>> == true &&
                is_same_v<int, remove_pointer_t<int**>> == false &&
                is_same_v<int, remove_pointer_t<int* const>> == true &&
                is_same_v<int, remove_pointer_t<int* volatile>> == true &&
                is_same_v<int, remove_pointer_t<int* const volatile>> == true);
}

// add_pointer
template <typename F, typename Class>
void ptr_to_member_func_cvref_test(F Class::*) {
  // F is an "abominable function type"
  using FF = stl::add_pointer_t<F>;
  static_assert(stl::is_same_v<F, FF>, "FF should be precisely F");
}

// add_pointer
void TestAddPointer() {
  struct S {
    void f_ref() & {}
    void f_const() const {}
  };
  using stl::is_pointer_v;
  using stl::is_same_v;

  int i = 123;
  int& ri = i;
  using IntPtr = stl::add_pointer_t<decltype(i)>;
  using IntPtr2 = stl::add_pointer_t<decltype(ri)>;
  IntPtr pi = &i;

  static_assert(is_pointer_v<IntPtr>, "IntPtr should be a pointer");
  static_assert(is_same_v<IntPtr, int*>, "IntPtr should be a pointer to int");
  static_assert(is_same_v<IntPtr2, IntPtr>,
                "IntPtr2 should be equal to IntPtr");

  using IntAgain = stl::remove_pointer_t<IntPtr>;
  IntAgain j = i;
  std::cout << "j = " << j << '\n';

  static_assert(!is_pointer_v<IntAgain>, "IntAgain should not be a pointer");
  static_assert(is_same_v<IntAgain, int>, "IntAgain should be equal to int");

  ptr_to_member_func_cvref_test(&S::f_ref);
  ptr_to_member_func_cvref_test(&S::f_const);
}

// remove_all_extents
template <class A>
void info(const A&) {
  typedef typename stl::remove_all_extents<A>::type Type;
  std::cout << "underlying type: " << typeid(Type).name() << '\n';
}

void RemoveAllExtents() {
  float a0;
  float a1[1][2][3];
  float a2[1][1][1][1][2];
  float* a3;
  int a4[3][2];
  double a5[2][3];
  struct X {
    int m;
  } x0[3][3];

  info(a0);
  info(a1);
  info(a2);
  info(a3);
  info(a4);
  info(a5);
  info(x0);
}

// decay
template <typename T, typename U>
constexpr bool is_decay_equ = stl::is_same_v<stl::decay_t<T>, U>;

void TestDecay() {
  static_assert(is_decay_equ<int, int> && !is_decay_equ<int, float> &&
                is_decay_equ<int&, int> && is_decay_equ<int&&, int> &&
                is_decay_equ<const int&, int> && is_decay_equ<int[2], int*> &&
                !is_decay_equ<int[4][2], int*> &&
                !is_decay_equ<int[4][2], int**> &&
                is_decay_equ<int[4][2], int(*)[2]> &&
                is_decay_equ<int(int), int (*)(int)>);
}

// remove_cvref
void RemoveCVRef() {
  using stl::is_same_v;
  using stl::remove_cvref_t;
  static_assert(is_same_v<remove_cvref_t<int>, int>);
  static_assert(is_same_v<remove_cvref_t<int&>, int>);
  static_assert(is_same_v<remove_cvref_t<int&&>, int>);
  static_assert(is_same_v<remove_cvref_t<const int&>, int>);
  static_assert(is_same_v<remove_cvref_t<const int[2]>, int[2]>);
  static_assert(is_same_v<remove_cvref_t<const int(&)[2]>, int[2]>);
  static_assert(is_same_v<remove_cvref_t<int(int)>, int(int)>);
}

// common_type
template <class T>
struct Number {
  T n;
};

template <class T, class U>
constexpr Number<stl::common_type_t<T, U>> operator+(const Number<T>& lhs,
                                                     const Number<U>& rhs) {
  return {lhs.n + rhs.n};
}

void describe(const char* expr, const Number<int>& x) {
  std::cout << expr << "  is  Number<int>{" << x.n << "}\n";
}

void describe(const char* expr, const Number<double>& x) {
  std::cout << expr << "  is  Number<double>{" << x.n << "}\n";
}

void TestCommonType() {
  Number<int> i1 = {1}, i2 = {2};
  Number<double> d1 = {2.3}, d2 = {3.5};
  describe("i1 + i2", i1 + i2);
  describe("i1 + d2", i1 + d2);
  describe("d1 + i2", d1 + i2);
  describe("d1 + d2", d1 + d2);
}

int main() {

  AddCV();
  RemoveAllExtents();
  TestCommonType();
}

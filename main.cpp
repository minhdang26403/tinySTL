#include <iostream>

#include "type_traits.h"

// integral_constant
void TestIntegralConstant() {
  typedef stl::integral_constant<int, 2> two_t;
  typedef stl::integral_constant<int, 4> four_t;

  static_assert(not stl::is_same<two_t, four_t>::value,
                "two_t and four_t are equal!");

  static_assert(two_t::value * 2 == four_t::value, "2*2 != 4");

  enum class my_e { e1, e2 };

  typedef stl::integral_constant<my_e, my_e::e1> my_e_e1;
  typedef stl::integral_constant<my_e, my_e::e2> my_e_e2;

  static_assert(my_e_e1() == my_e::e1);

  static_assert(my_e_e1::value != my_e::e2, "my_e_e1::value == my_e::e2");

  static_assert(stl::is_same<my_e_e2, my_e_e2>::value, "my_e_e2 != my_e_e2");
}

// is_same
void TestIsSame() {
  // compare the types of a couple variables
  long double num1 = 1.0;
  long double num2 = 2.0;
  static_assert(stl::is_same_v<decltype(num1), decltype(num2)> == true);

  // 'float' is never an integral type
  static_assert(stl::is_same<float, int>::value == false);

  // 'int' is implicitly 'signed'
  static_assert(stl::is_same_v<int, int> == true);
  static_assert(stl::is_same_v<int, unsigned int> == false);
  static_assert(stl::is_same_v<int, signed int> == true);

  // unlike other types, 'char' is neither 'unsigned' nor 'signed'
  static_assert(stl::is_same_v<char, char> == true);
  static_assert(stl::is_same_v<char, unsigned char> == false);
  static_assert(stl::is_same_v<char, signed char> == false);

  // const-qualified type T is not same as non-const T
  static_assert(!stl::is_same<const int, int>());
}

// is_void
void TestIsVoid() {
  static_assert(stl::is_void_v<void>);
  static_assert(!stl::is_void_v<int>);
  static_assert(stl::is_void_v<const void>);
}

// is_null_pointer
void TestIsNullPointer() {
  static_assert(stl::is_null_pointer_v<decltype(nullptr)>);
  static_assert(!stl::is_null_pointer_v<int*>);
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
  static_assert(std::is_integral<T>::value, "Integral required.");
  return i;
}
}  // namespace testIsIntegral

void TestIsIntegral() {
  using namespace testIsIntegral;
  static_assert(!stl::is_integral<A>::value);
  static_assert(!stl::is_integral_v<E>);
  static_assert(!stl::is_integral_v<float>);
  static_assert(!stl::is_integral_v<int*>);
  static_assert(stl::is_integral_v<int>);
  static_assert(stl::is_integral_v<const int>);
  static_assert(stl::is_integral_v<bool>);
  static_assert(stl::is_integral_v<char>);
  static_assert(stl::is_integral_v<BF>);
}

// is_floating_point
void TestIsFloatingPoint() {
  class A {};
  static_assert(!stl::is_floating_point_v<A>);
  static_assert(stl::is_floating_point_v<float>);
  static_assert(!stl::is_floating_point_v<float&>);
  static_assert(stl::is_floating_point_v<double>);
  static_assert(!stl::is_floating_point_v<double&>);
  static_assert(!stl::is_floating_point_v<int>);
}

// is_array
void TestIsArray() {
  class A {};
  static_assert(stl::is_array_v<A[]>);
  static_assert(!stl::is_array_v<A>);
  static_assert(stl::is_array_v<A[3]>);
  static_assert(!stl::is_array_v<float>);
  static_assert(!stl::is_array_v<int>);
  static_assert(stl::is_array_v<int[]>);
  static_assert(stl::is_array_v<int[3]>);
  static_assert(!stl::is_array_v<std::array<int, 3>>);
}

// is_enum
void TestIsEnum() {
  struct A {
    enum E {};
  };
  enum E {};
  enum class Ec : int {};

  static_assert(!stl::is_enum<A>::value);
  static_assert(stl::is_enum<E>::value);
  static_assert(stl::is_enum<A::E>());
  static_assert(!stl::is_enum_v<int>);
  static_assert(stl::is_enum_v<Ec>);
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

  static_assert(!stl::is_union<A>::value);
  static_assert(stl::is_union<B>::value);
  static_assert(!stl::is_union<C>::value);
  static_assert(!stl::is_union<int>::value);
}

// is_class
void TestIsClass() {
  struct A {};
  class B {};
  enum class E {};
  union U {
    class UC {};
  };
  static_assert(!stl::is_class_v<U>);
  static_assert(stl::is_class_v<U::UC>);

  static_assert(stl::is_class<A>::value);
  static_assert(stl::is_class_v<B>);
  static_assert(!stl::is_class_v<B*>);
  static_assert(!stl::is_class_v<B&>);
  static_assert(stl::is_class_v<const B>);
  static_assert(!stl::is_class<E>::value);
  static_assert(!stl::is_class_v<int>);
  static_assert(stl::is_class_v<struct S>);
  static_assert(stl::is_class_v<class C>);
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
  static_assert(!stl::is_function_v<A>);
  static_assert(stl::is_function_v<int(int)>);
  static_assert(stl::is_function_v<decltype(f)>);
  static_assert(!stl::is_function_v<int>);

  using T = PM_traits<decltype(&A::fun)>::member_type;  // T is int() const&
  static_assert(stl::is_function_v<T>);
}

// is_pointer
void TestIsPointer() {
  struct A {
    int m;
    void f() {}
  };
  int A::*mem_data_ptr = &A::m;
  void (A::*mem_fun_ptr)() = &A::f;

  static_assert(!stl::is_pointer<A>::value);
  static_assert(!stl::is_pointer_v<A>);
  static_assert(!stl::is_pointer<A>());
  static_assert(!stl::is_pointer<A>{});
  static_assert(!stl::is_pointer<A>()());
  static_assert(!stl::is_pointer<A>{}());
  static_assert(stl::is_pointer_v<A*>);
  static_assert(stl::is_pointer_v<A const* volatile>);
  static_assert(!stl::is_pointer_v<A&>);
  static_assert(!stl::is_pointer_v<decltype(mem_data_ptr)>);
  static_assert(!stl::is_pointer_v<decltype(mem_fun_ptr)>);
  static_assert(stl::is_pointer_v<void*>);
  static_assert(!stl::is_pointer_v<int>);
  static_assert(stl::is_pointer_v<int*>);
  static_assert(stl::is_pointer_v<int**>);
  static_assert(!stl::is_pointer_v<int[10]>);
  static_assert(!stl::is_pointer_v<stl::nullptr_t>);
}

// is_lvalue_reference
void TestIsLvalueReference() {
  class A {};
  static_assert(!stl::is_lvalue_reference<A>::value);
  static_assert(stl::is_lvalue_reference<A&>::value);
  static_assert(!stl::is_lvalue_reference<A&&>::value);
  static_assert(!stl::is_lvalue_reference<int>::value);
  static_assert(stl::is_lvalue_reference<int&>::value);
  static_assert(!stl::is_lvalue_reference<int&&>::value);
  static_assert(stl::is_lvalue_reference<int&>::value);
}

// is_rvalue_reference
void TestIsRvalueReference() {
  class A {};
  static_assert(!stl::is_rvalue_reference_v<A>);
  static_assert(!stl::is_rvalue_reference_v<A&>);
  static_assert(stl::is_rvalue_reference_v<A&&>);
  static_assert(!stl::is_rvalue_reference_v<int>);
  static_assert(!stl::is_rvalue_reference_v<int&>);
  static_assert(stl::is_rvalue_reference_v<int&&>);
}

// is_member_object_pointer
void TestIsMemberObjectPointer() {
  class C {};
  static_assert(stl::is_member_object_pointer_v<int(C::*)>);
  static_assert(!stl::is_member_object_pointer_v<int (C::*)()>);
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
  static_assert(!std::is_fundamental<A>::value);
  static_assert(std::is_fundamental<int>::value);
  static_assert(!std::is_fundamental<int&>::value);
  static_assert(!std::is_fundamental<int*>::value);
  static_assert(std::is_fundamental<float>::value);
  static_assert(!std::is_fundamental<float&>::value);
  static_assert(!std::is_fundamental<float*>::value);
}

// is_arithmetic
void TestIsArithmetic() {
  class A {};

  static_assert(stl::is_arithmetic_v<A> == false &&
                stl::is_arithmetic_v<bool> == true &&
                stl::is_arithmetic_v<int> == true &&
                stl::is_arithmetic_v<int const> == true &&
                stl::is_arithmetic_v<int&> == false &&
                stl::is_arithmetic_v<int*> == false &&
                stl::is_arithmetic_v<float> == true &&
                stl::is_arithmetic_v<float const> == true &&
                stl::is_arithmetic_v<float&> == false &&
                stl::is_arithmetic_v<float*> == false &&
                stl::is_arithmetic_v<char> == true &&
                stl::is_arithmetic_v<char const> == true &&
                stl::is_arithmetic_v<char&> == false &&
                stl::is_arithmetic_v<char*> == false);
}

// is_scalar
void TestIsScalar() {
  struct S {
    int m;
  } s;
  int S::*mp = &S::m;
  enum class E { e };

  // need std::decay
  static_assert(stl::is_scalar_v<decltype(42)>);
  static_assert(stl::is_scalar_v<decltype(3.14)>);
  static_assert(stl::is_scalar_v<decltype(E::e)>);
  // static_assert(stl::is_scalar_v<decltype("str")>);
  static_assert(stl::is_scalar_v<decltype(mp)>);
  static_assert(stl::is_scalar_v<decltype(nullptr)>);
  static_assert(stl::is_scalar_v<decltype(nullptr)>);
}

// is_object
void TestIsObject() {
  class cls {};
  static_assert(!stl::is_object_v<void>);
  static_assert(stl::is_object_v<int>);
  static_assert(!stl::is_object_v<int&>);
  static_assert(stl::is_object_v<int*>);
  static_assert(stl::is_object_v<cls>);
  static_assert(!stl::is_object_v<cls&>);
  static_assert(stl::is_object_v<cls*>);
  static_assert(!stl::is_object_v<int()>);
  static_assert(stl::is_object_v<int (*)()>);
}

// is_compound
void TestIsCompound() {
  struct S {
    int i : 8;
    int j;
    void foo();
  };
  static_assert(!stl::is_compound_v<decltype(S::i)>);
  static_assert(!stl::is_compound_v<decltype(S::j)>);
  static_assert(stl::is_compound_v<decltype(&S::j)>);
  static_assert(stl::is_compound_v<decltype(&S::foo)>);
}

// is_reference
void TestIsReference() {
  class A {};
  static_assert(!stl::is_reference_v<A>);
  static_assert(stl::is_reference_v<A&>);
  static_assert(stl::is_reference_v<A&&>);
  static_assert(!stl::is_reference_v<long>);
  static_assert(stl::is_reference_v<long&>);
  static_assert(stl::is_reference_v<long&&>);
  static_assert(!stl::is_reference_v<double*>);
  static_assert(stl::is_reference_v<double*&>);
  static_assert(stl::is_reference_v<double*&&>);
}

// is_member_pointer
void TestIsMemberPointer() {
  class C {};
  static_assert(stl::is_member_pointer_v<int(C::*)>);
  static_assert(stl::is_member_pointer_v<int(C::*)>);
  static_assert(stl::is_member_pointer_v<int (C::*)()>);
  static_assert(!stl::is_member_pointer_v<void>);
}

// is_const
void TestIsConst() {
  static_assert(!stl::is_const_v<int>);
  static_assert(stl::is_const_v<const int>);
  static_assert(!stl::is_const_v<const int*>);
  static_assert(stl::is_const_v<int* const>);
  static_assert(!stl::is_const_v<const int&>);
  static_assert(stl::is_const_v<int* const>);
}

// is_volatile
void TestIsVolatile() {
  static_assert(!std::is_volatile_v<int>);
  static_assert(std::is_volatile_v<volatile int>);
  static_assert(std::is_volatile_v<volatile const int>);
}

// is_trivial
void TestIsTrivial() {
  struct A {
    int m;
  };
  struct B {
    B() {}
  };
  static_assert(stl::is_trivial_v<A>);
  static_assert(!stl::is_trivial_v<B>);
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

  static_assert(stl::is_trivially_copyable_v<A>);
  static_assert(!stl::is_trivially_copyable_v<B>);
  static_assert(!stl::is_trivially_copyable_v<C>);
  static_assert(stl::is_trivially_copyable_v<D>);
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

  static_assert(stl::is_standard_layout_v<A>);
  static_assert(!stl::is_standard_layout_v<B>);
  static_assert(!stl::is_standard_layout_v<C>);
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

  static_assert(stl::is_pod_v<A>);
  static_assert(!stl::is_pod_v<B>);
  static_assert(!stl::is_pod_v<C>);
}

// is_empty
void TestIsEmpty() {
  struct A {};
  static_assert(stl::is_empty_v<A> == true);

  struct B {
    int m;
  };
  static_assert(stl::is_empty_v<B> == false);

  struct D {
    virtual ~D();
  };
  static_assert(stl::is_empty_v<D> == false);

  union E {};
  static_assert(stl::is_empty_v<E> == false);
}

// is_polymorphic
void TestIsPolymorphic() {
  struct A {
    int m;
  };
  static_assert(!std::is_polymorphic_v<A>);

  struct B {
    virtual void foo();
  };
  static_assert(std::is_polymorphic_v<B>);

  struct C : B {};
  static_assert(std::is_polymorphic_v<C>);

  struct D {
    virtual ~D() = default;
  };
  static_assert(std::is_polymorphic_v<D>);

  // Uses inheritance, but not the virtual keyword:
  struct E : A {};
  static_assert(!std::is_polymorphic_v<E>);

  struct F : virtual A {};
  static_assert(!std::is_polymorphic_v<F>);

  struct AX : A {};
  struct AY : A {};
  struct XY : virtual AX, virtual AY {};
  static_assert(!std::is_polymorphic_v<XY>);
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

  static_assert(!stl::is_abstract_v<A>);
  static_assert(!stl::is_abstract_v<B>);
  static_assert(stl::is_abstract_v<C>);
  static_assert(stl::is_abstract_v<D>);
}

// is_final
void TestIsFinal() {
  class A {};
  class B final {};

  static_assert(!stl::is_final_v<A>);
  static_assert(stl::is_final_v<B>);
}

// is_aggregate
void TestIsAggregate() {
  struct A {
    int x, y;
  };
  struct B {
    B(int, const char*) {}
  };
  static_assert(stl::is_aggregate_v<A>);
  static_assert(!stl::is_aggregate_v<B>);
}

// is_signed
void TestIsSigned() {
  class A {};
  enum B : int {};
  enum class C : int {};

  static_assert(stl::is_signed<A>::value == false);
  static_assert(stl::is_signed<float>::value == true);
  static_assert(stl::is_signed<signed int>::value == true);
  static_assert(stl::is_signed<unsigned int>::value == false);
  static_assert(stl::is_signed<B>::value == false);
  static_assert(stl::is_signed<C>::value == false);

  static_assert(stl::is_signed_v<bool> == false);
  static_assert(stl::is_signed<signed int>() == true);
  static_assert(stl::is_signed<unsigned int>{} == false);
  static_assert(stl::is_signed_v<signed char> == true);
  static_assert(stl::is_signed_v<unsigned char> == false);
}

// is_unsigned
void TestIsUnsigned() {
  class A {};
  static_assert(std::is_unsigned_v<A> == false);
  enum B : unsigned {};
  static_assert(std::is_unsigned_v<B> == false);
  enum class C : unsigned {};
  static_assert(std::is_unsigned_v<C> == false);
  struct S {
    unsigned p : 1;
    int q : 1;
  };
  static_assert(std::is_unsigned_v<decltype(S::p)> not_eq
                std::is_unsigned_v<decltype(S::q)>);
  static_assert(std::is_unsigned_v<float> == false &&
                std::is_unsigned_v<signed int> == false &&
                std::is_unsigned_v<unsigned int> == true &&
                std::is_unsigned_v<bool> == true);
}

// is_bounded_array
void TestIsBoundedArray() {
  class A {};
  static_assert(!stl::is_bounded_array_v<A>);
  static_assert(!stl::is_bounded_array_v<A[]>);
  static_assert(stl::is_bounded_array_v<A[3]>);
  static_assert(!stl::is_bounded_array_v<float>);
  static_assert(!stl::is_bounded_array_v<int>);
  static_assert(!stl::is_bounded_array_v<int[]>);
  static_assert(stl::is_bounded_array_v<int[3]>);
}

// is_unbounded_array
void TestIsUnboundedArray() {
  class A {};
  static_assert(!stl::is_unbounded_array_v<A>);
  static_assert(stl::is_unbounded_array_v<A[]>);
  static_assert(!stl::is_unbounded_array_v<A[3]>);
  static_assert(!stl::is_unbounded_array_v<float>);
  static_assert(!stl::is_unbounded_array_v<int>);
  static_assert(stl::is_unbounded_array_v<int[]>);
  static_assert(!stl::is_unbounded_array_v<int[3]>);
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

  static_assert(stl::is_constructible_v<Foo, int>);
  static_assert(stl::is_trivially_constructible_v<Foo, const Foo&>);
  static_assert(!stl::is_trivially_constructible_v<Foo, int>);
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
  static_assert(stl::is_default_constructible<Ex1>::value);
  static_assert(!stl::is_trivially_default_constructible_v<Ex1>);
  static_assert(stl::is_trivially_default_constructible_v<Ex2>);
  static_assert(stl::is_nothrow_default_constructible_v<Ex2>);
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
  static_assert(stl::is_copy_constructible_v<Ex1>);
  static_assert(!stl::is_trivially_copy_constructible_v<Ex1>);
  static_assert(stl::is_trivially_copy_constructible_v<Ex2>);
  static_assert(stl::is_nothrow_copy_constructible_v<Ex2>);
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

  static_assert(stl::is_move_constructible_v<Ex1>);
  static_assert(!stl::is_trivially_move_constructible_v<Ex1>);
  static_assert(stl::is_nothrow_move_constructible_v<Ex1>);
  static_assert(stl::is_trivially_move_constructible_v<Ex2>);
  static_assert(stl::is_nothrow_move_constructible_v<Ex2>);
  static_assert(stl::is_move_constructible_v<NoMove>);
  static_assert(!stl::is_nothrow_move_constructible_v<NoMove>);
}

// is_assignable / is_trivially_assignable / is_nothrow_assignable
void TestIsAssignable() {
  struct Ex1 {
    int n;
  };
  static_assert(!stl::is_assignable_v<int, int>);  // 1 = 1; wouldn't compile
  static_assert(stl::is_assignable_v<int&, int>);  // int a; a = 1; works
  static_assert(!stl::is_assignable_v<int, double>);
  static_assert(stl::is_nothrow_assignable_v<int&, double>);
  static_assert(stl::is_assignable_v<std::string, double>);
  static_assert(stl::is_trivially_assignable_v<Ex1&, const Ex1&>);
}

// is_copy_assignable / is_trivially_copy_assignable /
// is_nothrow_copy_assignable
void TestIsCopyAssignable() {
  struct Foo {
    int n;
  };
  static_assert(stl::is_trivially_copy_assignable<Foo>::value);
  static_assert(!stl::is_copy_assignable<int[2]>::value);
  static_assert(stl::is_nothrow_copy_assignable<int>::value);
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
  static_assert(stl::is_nothrow_move_assignable_v<std::string>);
  static_assert(!stl::is_move_assignable_v<int[2]>);
  static_assert(stl::is_trivially_move_assignable_v<Foo>);
  static_assert(stl::is_move_assignable_v<NoMove>);
  static_assert(!stl::is_nothrow_move_assignable_v<NoMove>);
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
  static_assert(stl::is_destructible<std::string>::value);
  static_assert(!stl::is_trivially_destructible_v<Foo>);
  static_assert(stl::is_nothrow_destructible<Foo>());
  static_assert(stl::is_trivially_destructible<Bar>{});
}

// has_virtual_destructor
void TestHasVirtualDestructor() {
  struct S {};
  static_assert(!stl::has_virtual_destructor_v<S>);

  struct B {
    virtual ~B() { std::puts("B::~B"); }
  };
  static_assert(stl::has_virtual_destructor_v<B>);

  struct D : B {
    ~D() { std::puts("D::~D"); }
  };
  static_assert(stl::has_virtual_destructor_v<D>);
}

// alignment_of
void TestAlignmentOf() {
  struct A {};
  struct B {
    std::int8_t p;
    std::int16_t q;
  };

  static_assert(stl::alignment_of<A>::value == 1);
  static_assert(stl::alignment_of<B>::value == 2);
  static_assert(stl::alignment_of<int>() == 4);
  static_assert(stl::alignment_of_v<double> == 8);
}

// rank
void TestRank() {
  static_assert(std::rank<int>{} == 0 && std::rank<int[5]>{} == 1 &&
                std::rank<int[5][5]>{} == 2 && std::rank<int[][5][5]>{} == 3);

  [[maybe_unused]] int ary[][3] = {{1, 2, 3}};
  // The reason of rank of "ary[0]" is calculated as 0
  static_assert(std::rank_v<decltype(ary[0])> == 0);
  // is that rank cannot deal with reference type. i.e. int(&)[3]
  static_assert(std::is_same_v<decltype(ary[0]), int(&)[3]>);
  // The solution is to remove reference type
  static_assert(std::rank_v<stl::remove_cvref_t<decltype(ary[0])>> == 1);
}

// extent
void TestExtent() {
  static_assert(stl::extent_v<int[3]> == 3);
  static_assert(stl::extent_v<int[3][4], 0> == 3);
  static_assert(stl::extent_v<int[3][4], 1> == 4);
  static_assert(stl::extent_v<int[3][4], 2> == 0);
  static_assert(stl::extent_v<int[]> == 0);

  const int ints[] = {1, 2, 3, 4};
  static_assert(stl::extent<decltype(ints)>::value == 4);
}

// is_same
void TestIsSame() {
  // usually true if 'int' is 32 bit
  static_assert(stl::is_same<int, std::int32_t>::value);  // maybe true
  // possibly true if ILP64 data model is used
  static_assert(!stl::is_same<int, std::int64_t>::value);  // maybe false

  // same tests as above, except using C++17's std::is_same_v<T, U> format
  static_assert(stl::is_same_v<int, std::int32_t>);
  static_assert(!stl::is_same_v<int, std::int64_t>);

  // compare the types of a couple variables
  long double num1 = 1.0;
  long double num2 = 2.0;
  static_assert(stl::is_same_v<decltype(num1), decltype(num2)> == true);

  // 'float' is never an integral type
  static_assert(stl::is_same<float, std::int32_t>::value == false);

  // 'int' is implicitly 'signed'
  static_assert(stl::is_same_v<int, int> == true);
  static_assert(stl::is_same_v<int, unsigned int> == false);
  static_assert(stl::is_same_v<int, signed int> == true);

  // unlike other types, 'char' is neither 'unsigned' nor 'signed'
  static_assert(stl::is_same_v<char, char> == true);
  static_assert(stl::is_same_v<char, unsigned char> == false);
  static_assert(stl::is_same_v<char, signed char> == false);

  // const-qualified type T is not same as non-const T
  static_assert(!stl::is_same<const int, int>());
}

// remove_cv
void RemoveCV() {
  static_assert(
      stl::is_same_v<stl::remove_cv_t<int>, int> &&
      stl::is_same_v<stl::remove_cv_t<const int>, int> &&
      stl::is_same_v<stl::remove_cv_t<volatile int>, int> &&
      stl::is_same_v<stl::remove_cv_t<const volatile int>, int> &&
      stl::is_same_v<stl::remove_cv_t<const volatile int*>,
                     const volatile int*> &&
      stl::is_same_v<stl::remove_cv_t<const int* volatile>, const int*> &&
      stl::is_same_v<stl::remove_cv_t<int* const volatile>, int*>);
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
  stl::add_const<foo>::type{}.m();
  stl::add_volatile<foo>::type{}.m();
  stl::add_cv<foo>::type{}.m();
}

// remove_reference
void RemoveReference() {
  static_assert(stl::is_same_v<int, stl::remove_reference<int>::type>);
  static_assert(stl::is_same_v<int, stl::remove_reference<int&>::type>);
  static_assert(stl::is_same_v<int, stl::remove_reference<int&&>::type>);
  static_assert(
      stl::is_same_v<const int, stl::remove_reference<const int&>::type>);
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
  static_assert(
      stl::is_same_v<int, int> == true && stl::is_same_v<int, int*> == false &&
      stl::is_same_v<int, int**> == false &&
      stl::is_same_v<int, stl::remove_pointer_t<int>> == true &&
      stl::is_same_v<int, stl::remove_pointer_t<int*>> == true &&
      stl::is_same_v<int, stl::remove_pointer_t<int**>> == false &&
      stl::is_same_v<int, stl::remove_pointer_t<int* const>> == true &&
      stl::is_same_v<int, stl::remove_pointer_t<int* volatile>> == true &&
      stl::is_same_v<int, stl::remove_pointer_t<int* const volatile>> == true);
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

  int i = 123;
  int& ri = i;
  typedef stl::add_pointer<decltype(i)>::type IntPtr;
  typedef stl::add_pointer<decltype(ri)>::type IntPtr2;
  IntPtr pi = &i;

  static_assert(stl::is_pointer_v<IntPtr>, "IntPtr should be a pointer");
  static_assert(stl::is_same_v<IntPtr, int*>,
                "IntPtr should be a pointer to int");
  static_assert(stl::is_same_v<IntPtr2, IntPtr>,
                "IntPtr2 should be equal to IntPtr");

  typedef stl::remove_pointer<IntPtr>::type IntAgain;
  IntAgain j = i;
  std::cout << "j = " << j << '\n';

  static_assert(!std::is_pointer_v<IntAgain>,
                "IntAgain should not be a pointer");
  static_assert(std::is_same_v<IntAgain, int>,
                "IntAgain should be equal to int");

  ptr_to_member_func_cvref_test(&S::f_ref);
  ptr_to_member_func_cvref_test(&S::f_const);
}

// remove_all_extents
template <class A>
void info(const A&) {
  typedef typename std::remove_all_extents<A>::type Type;
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
  static_assert(stl::is_same_v<stl::remove_cvref_t<int>, int>);
  static_assert(stl::is_same_v<stl::remove_cvref_t<int&>, int>);
  static_assert(stl::is_same_v<stl::remove_cvref_t<int&&>, int>);
  static_assert(stl::is_same_v<stl::remove_cvref_t<const int&>, int>);
  static_assert(stl::is_same_v<stl::remove_cvref_t<const int[2]>, int[2]>);
  static_assert(stl::is_same_v<stl::remove_cvref_t<const int(&)[2]>, int[2]>);
  static_assert(stl::is_same_v<stl::remove_cvref_t<int(int)>, int(int)>);
}

int main() {
  TestIntegralConstant();
  TestIsSame();
  TestIsVoid();
  TestIsNullPointer();
  TestIsFloatingPoint();
  TestIsArray();

  AddCV();
  RemoveAllExtents();
}

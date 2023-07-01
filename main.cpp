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

#include "typelist.h"

#include "type_traits.h"

using namespace stl;

using signed_integral_types = typelist<signed char, short, int, long long>;

// is_empty
void TestIsEmpty() {
  static_assert(is_list_empty_v<typelist<>>);
  static_assert(!is_list_empty_v<signed_integral_types>);
}

// front
void TestFront() {
  static_assert(is_same_v<signed char, front_t<signed_integral_types>>);
}

// pop_front
void TestPopFront() {
  using result_list = typelist<short, int, long long>;
  static_assert(is_same_v<pop_front_t<signed_integral_types>, result_list>);
}

// push_front
void TestPushFront() {
  static_assert(
      is_same_v<push_front_t<pop_front_t<signed_integral_types>, signed char>,
                signed_integral_types>);
}

// nth_element
void TestNthElement() {
  static_assert(
      is_same_v<nth_element_t<signed_integral_types, 0>, signed char>);
  static_assert(is_same_v<nth_element_t<signed_integral_types, 2>, int>);
  static_assert(is_same_v<nth_element_t<signed_integral_types, 3>, long long>);
}

// largest_type
void TestLargestType() {
  using list = typelist<bool, int, long, short>;
  static_assert(is_same_v<largest_type_t<list>, long>);
}

// push_back
void TestPushBack() {
  using list = typelist<signed char, short, int>;
  static_assert(is_same_v<push_back_t<list, long long>, signed_integral_types>);
  static_assert(!is_same_v<push_back_t<list, long>, signed_integral_types>);
}

// reverse
void TestReverse() {
  using result = typelist<long long, int, short, signed char>;
  static_assert(is_same_v<result, reverse_t<signed_integral_types>>);
}

// pop_back
void TestPopBack() {
  using result = typelist<signed char, short, int>;
  static_assert(is_same_v<result, pop_back_t<signed_integral_types>>);
}

// transform
void TestTransform() {
  using result =
      typelist<const signed char, const short, const int, const long long>;
  static_assert(
      is_same_v<transform_t<signed_integral_types, add_const_t>, result>);
}

// accumulate
void TestAccumulate() {
  using result_push_front =
      accumulate_t<signed_integral_types, push_front_t, typelist<>>;
  static_assert(is_same_v<result_push_front, reverse_t<signed_integral_types>>);
  using result_push_back =
      accumulate_t<signed_integral_types, push_back_t, typelist<>>;
  static_assert(is_same_v<result_push_back, signed_integral_types>);
}

int main() { return 0; }

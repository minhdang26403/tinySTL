#ifndef TYPE_TRAITS_H_
#define TYPE_TRAITS_H_

namespace stl {

using nullptr_t = decltype(nullptr);
using size_t = decltype(sizeof(unsigned int));

/*============================================================
==========================Declaration=========================
==============================================================*/

/*============================================================
========================Type property=========================
==============================================================*/

/*===================Primary type categories==================*/

// is_void
template <typename T>
struct is_void;
template <typename T>
inline constexpr bool is_void_v = is_void<T>::value;

// is_null_pointer
template <typename T>
struct is_null_pointer;
template <typename T>
inline constexpr bool is_null_pointer_v = is_null_pointer<T>::value;

// is_integral
template <typename T>
struct is_integral;
template <typename T>
inline constexpr bool is_integral_v = is_integral<T>::value;

// is_floating_point
template <typename T>
struct is_floating_point;
template <typename T>
inline constexpr bool is_floating_point_v = is_floating_point<T>::value;

// is_array
template <typename T>
struct is_array;
template <typename T>
inline constexpr bool is_array_v = is_array<T>::value;

// is_enum
template <typename T>
struct is_enum;
template <typename T>
inline constexpr bool is_enum_v = is_enum<T>::value;

// is_union
template <typename T>
struct is_union;
template <typename T>
inline constexpr bool is_union_v = is_union<T>::value;

// is_class
template <typename T>
struct is_class;
template <typename T>
inline constexpr bool is_class_v = is_class<T>::value;

// is_function
template <typename T>
struct is_function;
template <typename T>
inline constexpr bool is_function_v = is_function<T>::value;

// is_pointer
template <typename T>
struct is_pointer;
template <typename T>
inline constexpr bool is_pointer_v = is_pointer<T>::value;

// is_lvalue_reference
template <typename T>
struct is_lvalue_reference;
template <typename T>
inline constexpr bool is_lvalue_reference_v = is_lvalue_reference<T>::value;

// is_rvalue_reference
template <typename T>
struct is_rvalue_reference;
template <typename T>
inline constexpr bool is_rvalue_reference_v = is_rvalue_reference<T>::value;

// is_member_object_pointer
template <typename T>
struct is_member_object_pointer;
template <typename T>
inline constexpr bool is_member_object_pointer_v =
    is_member_object_pointer<T>::value;

// is_member_function_pointer
template <typename T>
struct is_member_function_pointer;
template <typename T>
inline constexpr bool is_member_function_pointer_v =
    is_member_function_pointer<T>::value;

/*==================Composite type categories=================*/

// is_fundamental
template <typename T>
struct is_fundamental;
template <typename T>
inline constexpr bool is_fundamental_v = is_fundamental<T>::value;

// is_arithmetic
template <typename T>
struct is_arithmetic;
template <typename T>
inline constexpr bool is_arithmetic_v = is_arithmetic<T>::value;

// is_scalar
template <typename T>
struct is_scalar;
template <typename T>
inline constexpr bool is_scalar_v = is_scalar<T>::value;

// is_object
template <typename T>
struct is_object;
template <typename T>
inline constexpr bool is_object_v = is_object<T>::value;

// is_compound
template <typename T>
struct is_compound;
template <typename T>
inline constexpr bool is_compound_v = is_compound<T>::value;

// is_reference
template <typename T>
struct is_reference;
template <typename T>
inline constexpr bool is_reference_v = is_reference<T>::value;

// is_member_pointer
template <typename T>
struct is_member_pointer;
template <typename T>
inline constexpr bool is_member_pointer_v = is_member_pointer<T>::value;

/*=======================Type properties======================*/

// is_const
template <typename T>
struct is_const;
template <typename T>
inline constexpr bool is_const_v = is_const<T>::value;

// is_volatile
template <typename T>
struct is_volatile;
template <typename T>
inline constexpr bool is_volatile_v = is_volatile<T>::value;

// is_trivial
template <typename T>
struct is_trivial;
template <typename T>
inline constexpr bool is_trivial_v = is_trivial<T>::value;

// is_trivially_copyable
template <typename T>
struct is_trivially_copyable;
template <typename T>
inline constexpr bool is_trivially_copyable_v = is_trivially_copyable<T>::value;

// is_standard_layout
template <typename T>
struct is_standard_layout;
template <typename T>
inline constexpr bool is_standard_layout_v = is_standard_layout<T>::value;

// is_pod
template <typename T>
struct is_pod;
template <typename T>
inline constexpr bool is_pod_v = is_pod<T>::value;

// is_empty
template <typename T>
struct is_empty;
template <typename T>
inline constexpr bool is_empty_v = is_empty<T>::value;

// is_polymorphic
template <typename T>
struct is_polymorphic;
template <typename T>
inline constexpr bool is_polymorphic_v = is_polymorphic<T>::value;

// is_abstract
template <typename T>
struct is_abstract;
template <typename T>
inline constexpr bool is_abstract_v = is_abstract<T>::value;

// is_final
template <typename T>
struct is_final;
template <typename T>
inline constexpr bool is_final_v = is_final<T>::value;

// is_aggregate
template <typename T>
struct is_aggregate;
template <typename T>
inline constexpr bool is_aggregate_v = is_aggregate<T>::value;

// is_signed
template <typename T>
struct is_signed;
template <typename T>
inline constexpr bool is_signed_v = is_signed<T>::value;

// is_unsigned
template <typename T>
struct is_unsigned;
template <typename T>
inline constexpr bool is_unsigned_v = is_unsigned<T>::value;

// is_bounded_array
template <typename T>
struct is_bounded_array;
template <typename T>
inline constexpr bool is_bounded_array_v = is_bounded_array<T>::value;

// is_unbounded_array
template <typename T>
struct is_unbounded_array;
template <typename T>
inline constexpr bool is_unbounded_array_v = is_unbounded_array<T>::value;

/*====================Supported operations====================*/

/*============================================================
======================Type modifications======================
==============================================================*/

// remove_cv
template <typename T>
struct remove_cv;
template <typename T>
using remove_cv_t = typename remove_cv<T>::type;

// remove_const
template <typename T>
struct remove_const;
template <typename T>
using remove_const_t = typename remove_const<T>::type;

// remove_volatile
template <typename T>
struct remove_volatile;
template <typename T>
using remove_volatile_t = typename remove_volatile<T>::type;

// add_cv
template <typename T>
struct add_cv;
template <typename T>
using add_cv_t = typename add_cv<T>::type;

// add_const
template <typename T>
struct add_const;
template <typename T>
using add_const_t = typename add_const<T>::type;

// add_volatile
template <typename T>
struct add_volatile;
template <typename T>
using add_volatile_t = typename add_volatile<T>::type;

// remove_reference
template <typename T>
struct remove_reference;
template <typename T>
using remove_reference_t = typename remove_reference<T>::type;

// add_lvalue_reference
template <typename T>
struct add_lvalue_reference;
template <typename T>
using add_lvalue_reference_t = typename add_lvalue_reference<T>::type;

// add_rvalue_reference
template <typename T>
struct add_rvalue_reference;
template <typename T>
using add_rvalue_reference_t = typename add_rvalue_reference<T>::type;

// remove_pointer
template <typename T>
struct remove_pointer;
template <typename T>
using remove_pointer_t = typename remove_pointer<T>::type;

// add_pointer
template <typename T>
struct add_pointer;
template <typename T>
using add_pointer_t = typename add_pointer<T>::type;

// make_signed
template <typename T>
struct make_signed;
template <typename T>
using make_signed_t = typename make_signed<T>::type;

// make_unsigned
template <typename T>
struct make_unsigned;
template <typename T>
using make_unsigned_t = typename make_unsigned<T>::type;

// remove_extent
template <typename T>
struct remove_extent;
template <typename T>
using remove_extent_t = typename remove_extent<T>::type;

// remove_all_extents
template <typename T>
struct remove_all_extents;
template <typename T>
using remove_all_extents_t = typename remove_all_extents<T>::type;

// decay
template <typename T>
struct decay;
template <typename T>
using decay_t = typename decay<T>::type;

// remove_cvref
template <typename T>
struct remove_cvref;
template <typename T>
using remove_cvref_t = typename remove_cvref<T>::type;

// enable_if
template <bool B, typename T = void>
struct enable_if;
template <bool B, typename T = void>
using enable_if_t = typename enable_if<B, T>::type;

// conditional
template <bool B, typename T, typename F>
struct conditional;
template <bool B, typename T, typename F>
using conditional_t = typename conditional<B, T, F>::type;

// void_t
template <typename T>
using void_t = void;

// type_identity
template <typename T>
struct type_identity;
template <typename T>
using type_identity_t = typename type_identity<T>::type;

/*============================================================
=========================Implementation=======================
==============================================================*/

/*============================================================
=========================Type property========================
==============================================================*/

/*===================Primary type categories==================*/

template <typename T, T v>
struct integral_constant {
  static constexpr T value = v;
  using value_type = T;
  using type = integral_constant;  // injected class name

  constexpr operator value_type() const noexcept { return v; }
  constexpr value_type operator()() const noexcept { return v; }
};

template <bool B>
using bool_constant = integral_constant<bool, B>;

using true_type = bool_constant<true>;
using false_type = bool_constant<false>;

template <typename T, typename U>
struct is_same : false_type {};

template <typename T>
struct is_same<T, T> : true_type {};

template <typename T, typename U>
inline constexpr bool is_same_v = is_same<T, U>::value;

// is_void
template <typename T>
struct is_void : is_same<void, remove_cv_t<T>> {};

// is_null_pointer
template <typename T>
struct is_null_pointer : is_same<nullptr_t, remove_cv_t<T>> {};

// is_integral
template <typename T>
struct is_integral : bool_constant<__is_integral(T)> {};

// is_floating_point
template <typename T>
struct is_floating_point
    : bool_constant<is_same_v<float, remove_cv_t<T>> ||
                    is_same_v<double, remove_cv_t<T>> ||
                    is_same_v<long double, remove_cv_t<T>>> {};

// is_array
template <typename T>
struct is_array : false_type {};
template <typename T>
struct is_array<T[]> : true_type {};
template <typename T, size_t N>
struct is_array<T[N]> : true_type {};

// is_enum
template <typename T>
struct is_enum : bool_constant<__is_enum(T)> {};

// is_union
template <typename T>
struct is_union : bool_constant<__is_union(T)> {};

// is_class
template <typename T>
struct is_class : bool_constant<__is_class(T)> {};

// is_function
template <typename T>
struct is_function : bool_constant<__is_function(T)> {};

// is_pointer
template <typename T>
struct is_pointer : false_type {};
template <typename T>
struct is_pointer<T*> : true_type {};
template <typename T>
struct is_pointer<T* const> : true_type {};
template <typename T>
struct is_pointer<T* volatile> : true_type {};
template <typename T>
struct is_pointer<T* const volatile> : true_type {};

// is_lvalue_reference
template <typename T>
struct is_lvalue_reference : false_type {};
template <typename T>
struct is_lvalue_reference<T&> : true_type {};

// is_rvalue_reference
template <typename T>
struct is_rvalue_reference : false_type {};
template <typename T>
struct is_rvalue_reference<T&&> : true_type {};

// is_member_object_pointer
template <typename T>
struct is_member_object_pointer
    : bool_constant<is_member_pointer_v<T> &&
                    !is_member_function_pointer_v<T>> {};

// is_member_function_pointer
template <typename T>
struct is_member_function_pointer_helper : false_type {};
template <typename T, typename U>
struct is_member_function_pointer_helper<T U::*> : is_function<T> {};
template <typename T>
struct is_member_function_pointer
    : is_member_function_pointer_helper<remove_cv_t<T>> {};

/*==================Composite type categories=================*/

// is_fundamental
template <typename T>
struct is_fundamental : bool_constant<is_arithmetic_v<T> || is_void_v<T> ||
                                      is_null_pointer_v<T>> {};

// is_arithmetic
template <typename T>
struct is_arithmetic
    : bool_constant<is_integral_v<T> || is_floating_point_v<T>> {};

// is_scalar
template <typename T>
struct is_scalar
    : bool_constant<is_arithmetic_v<T> || is_enum_v<T> || is_pointer_v<T> ||
                    is_member_pointer_v<T> || is_null_pointer_v<T>> {};

// is_object
template <typename T>
struct is_object : bool_constant<is_scalar_v<T> || is_array_v<T> ||
                                 is_union_v<T> || is_class_v<T>> {};

// is_compound
template <typename T>
struct is_compound : bool_constant<!is_fundamental_v<T>> {};

// is_reference
template <typename T>
struct is_reference
    : bool_constant<is_lvalue_reference_v<T> || is_rvalue_reference_v<T>> {};

// is_member_pointer
template <typename T>
struct is_member_pointer_helper : false_type {};
template <typename T, typename U>
struct is_member_pointer_helper<T U::*> : true_type {};
template <typename T>
struct is_member_pointer : is_member_pointer_helper<remove_cv_t<T>> {};

/*=======================Type properties======================*/

// is_const
template <typename T>
struct is_const : false_type {};
template <typename T>
struct is_const<const T> : true_type {};

// is_volatile
template <typename T>
struct is_volatile : false_type {};
template <typename T>
struct is_volatile<volatile T> : true_type {};

// is_trivial
template <typename T>
struct is_trivial : bool_constant<__is_trivial(T)> {};

// is_trivially_copyable
template <typename T>
struct is_trivially_copyable : bool_constant<__is_trivially_copyable(T)> {};

// is_standard_layout
template <typename T>
struct is_standard_layout : bool_constant<__is_standard_layout(T)> {};

// // is_pod
template <typename T>
struct is_pod : bool_constant<__is_pod(T)> {};

// is_empty
template <typename T>
struct is_empty : bool_constant<__is_empty(T)> {};

// is_polymorphic
template <typename T>
true_type detect_is_polymorphic(
      decltype(dynamic_cast<const volatile void*>(static_cast<T*>(nullptr)))
    );

template<typename T>
false_type detect_is_polymorphic(...);

template <typename T>
struct is_polymorphic : decltype(detect_is_polymorphic<T>(nullptr)) {};

// is_abstract
template <typename T>
struct is_abstract : bool_constant<__is_abstract(T)> {};

// is_final
template <typename T>
struct is_final : bool_constant<__is_final(T)> {};

// is_aggregate
template <typename T>
struct is_aggregate : bool_constant<__is_aggregate(T)> {};

// // is_signed
// template <typename T>
// struct is_signed;
// template <typename T>
// inline constexpr bool is_signed_v = is_signed<T>::value;

// // is_unsigned
// template <typename T>
// struct is_unsigned;
// template <typename T>
// inline constexpr bool is_unsigned_v = is_unsigned<T>::value;

// // is_bounded_array
// template <typename T>
// struct is_bounded_array;
// template <typename T>
// inline constexpr bool is_bounded_array_v = is_bounded_array<T>::value;

// // is_unbounded_array
// template <typename T>
// struct is_unbounded_array;
// template <typename T>
// inline constexpr bool is_unbounded_array_v = is_unbounded_array<T>::value;

/*============================================================
======================Type modifications======================
==============================================================*/

// remove_cv
template <typename T>
struct remove_cv {
  using type = remove_const_t<remove_volatile_t<T>>;
};

// remove_const
template <typename T>
struct remove_const {
  using type = T;
};
template <typename T>
struct remove_const<const T> {
  using type = T;
};

// remove_volatile
template <typename T>
struct remove_volatile {
  using type = T;
};
template <typename T>
struct remove_volatile<volatile T> {
  using type = T;
};

// add_cv
template <typename T>
struct add_cv {
  using type = const volatile T;
};

// add_const
template <typename T>
struct add_const {
  using type = const T;
};

// add_volatile
template <typename T>
struct add_volatile {
  using type = volatile T;
};

// remove_reference
template <typename T>
struct remove_reference {
  using type = T;
};
template <typename T>
struct remove_reference<T&> {
  using type = T;
};
template <typename T>
struct remove_reference<T&&> {
  using type = T;
};

// add_lvalue_reference
template <typename T>  // `cv void&` is a substitution failure
auto try_add_lvalue_reference(int) -> type_identity<T&>;
template <typename T>  // Handle T = cv void case
auto try_add_lvalue_reference(...) -> type_identity<T>;
template <typename T>
struct add_lvalue_reference : decltype(try_add_lvalue_reference<T>(0)) {};

// add_rvalue_reference
template <typename T>
auto try_add_rvalue_reference(int) -> type_identity<T&&>;
template <typename T>
auto try_add_rvalue_reference(...) -> type_identity<T>;
template <typename T>
struct add_rvalue_reference : decltype(try_add_rvalue_reference<T>(0)) {};

// remove pointer
template <typename T>
struct remove_pointer {
  using type = T;
};
template <typename T>
struct remove_pointer<T*> {
  using type = T;
};
template <typename T>
struct remove_pointer<T* const> {
  using type = T;
};
template <typename T>
struct remove_pointer<T* volatile> {
  using type = T;
};
template <typename T>
struct remove_pointer<T* const volatile> {
  using type = T;
};

// add_pointer
template <typename T>
auto try_add_pointer(int) -> type_identity<remove_reference_t<T>*>;
template <typename T>
auto try_add_pointer(...) -> type_identity<T>;
template <typename T>
struct add_pointer : decltype(try_add_pointer<T>(0)) {};

// remove_extent
template <typename T>
struct remove_extent {
  using type = T;
};
template <typename T>
struct remove_extent<T[]> {
  using type = T;
};
template <typename T, size_t N>
struct remove_extent<T[N]> {
  using type = T;
};

// remove_all_extents
template <typename T>
struct remove_all_extents {
  using type = T;
};
template <typename T>
struct remove_all_extents<T[]> {
  using type = remove_all_extents_t<T>;
};
template <typename T, size_t N>
struct remove_all_extents<T[N]> {
  using type = remove_all_extents_t<T>;
};

// decay
template <typename T>
struct decay {
 private:
  using U = remove_reference_t<T>;

 public:
  using type = conditional_t<
      is_array_v<U>, add_pointer_t<remove_extent_t<U>>,
      conditional_t<is_function_v<U>, add_pointer_t<U>, remove_cv_t<U>>>;
};

// remove_cvref
template <typename T>
struct remove_cvref {
  using type = remove_cv_t<remove_reference_t<T>>;
};

// enable_if
template <bool B, typename T>
struct enable_if {};
template <typename T>
struct enable_if<true, T> {
  using type = T;
};

// conditional
template <bool B, class T, class F>
struct conditional {
  using type = T;
};
template <class T, class F>
struct conditional<false, T, F> {
  using type = F;
};

// void_t
template <typename T>
using void_t = void;

// type_identity
template <typename T>
struct type_identity {
  using type = T;
};

};  // namespace stl

#endif  // TYPE_TRAITS_H_

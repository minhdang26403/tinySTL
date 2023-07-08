#ifndef TYPELIST_H_
#define TYPELIST_H_

#include "type_traits.h"

namespace stl {

// typelist
template <typename... Elements>
struct typelist {};

// is_empty
template <typename List>
struct is_list_empty : false_type {};

template <>
struct is_list_empty<typelist<>> : true_type {};

template <typename List>
inline constexpr bool is_list_empty_v = is_list_empty<List>::value;

// front
template <typename List>
struct front;

template <typename Head, typename... Tail>
struct front<typelist<Head, Tail...>> {
  using type = Head;
};

template <typename List>
using front_t = typename front<List>::type;

// pop_front
template <typename List>
struct pop_front;

template <typename Head, typename... Tail>
struct pop_front<typelist<Head, Tail...>> {
  using type = typelist<Tail...>;
};

template <typename List>
using pop_front_t = typename pop_front<List>::type;

// push_front
template <typename List, typename NewElement>
struct push_front;

template <typename... Elements, typename NewElement>
struct push_front<typelist<Elements...>, NewElement> {
  using type = typelist<NewElement, Elements...>;
};

template <typename List, typename NewElement>
using push_front_t = typename push_front<List, NewElement>::type;

// nth_element
template <typename List, unsigned N>
struct nth_element : nth_element<pop_front_t<List>, N - 1> {};

template <typename List>
struct nth_element<List, 0> : front<List> {};

template <typename List, unsigned N>
using nth_element_t = typename nth_element<List, N>::type;

// largest_type
template <typename List, bool IsEmpty = is_list_empty_v<List>>
struct largest_type {
  using first = front_t<List>;
  using rest = typename largest_type<pop_front_t<List>>::type;
  using type = stl::conditional_t<sizeof(first) >= sizeof(rest), first, rest>;
};

template <typename List>
struct largest_type<List, true> {
  using type = char;
};

template <typename List>
using largest_type_t = typename largest_type<List>::type;

// push_back
template <typename List, typename NewElement>
struct push_back;

template <typename... Elements, typename NewElement>
struct push_back<typelist<Elements...>, NewElement> {
  using type = typelist<Elements..., NewElement>;
};

template <typename List, typename NewElement>
using push_back_t = typename push_back<List, NewElement>::type;

// reverse
template <typename List, bool IsEmpty = is_list_empty_v<List>>
struct reverse;

template <typename List>
using reverse_t = typename reverse<List>::type;

template <typename List>
struct reverse<List, false>
    : push_back<reverse_t<pop_front_t<List>>, front_t<List>> {};

template <typename List>
struct reverse<List, true> {
  using type = List;
};

// pop_back
template <typename List>
struct pop_back : reverse<pop_front_t<reverse_t<List>>> {};

template <typename List>
using pop_back_t = typename pop_back<List>::type;

// transform
template <typename List, template <typename> class MetaFun,
          bool IsEmpty = is_list_empty_v<List>>
struct transform;

template <typename List, template <typename> class MetaFun>
using transform_t = typename transform<List, MetaFun>::type;

template <typename List, template <typename> class MetaFun>
struct transform<List, MetaFun, false>
    : push_front<transform_t<pop_front_t<List>, MetaFun>,
                 MetaFun<front_t<List>>> {};

template <typename List, template <typename> class MetaFun>
struct transform<List, MetaFun, true> {
  using type = List;
};

// accumulate
template <typename List, template <typename, typename> class F, typename I,
          bool IsEmpty = is_list_empty_v<List>>
struct accumulate {};

template <typename List, template <typename, typename> class F, typename I>
struct accumulate<List, F, I, false>
    : accumulate<pop_front_t<List>, F, F<I, front_t<List>>> {};

template <typename List, template <typename, typename> class F, typename I>
struct accumulate<List, F, I, true> {
  using type = I;
};

template <typename List, template <typename, typename> class F, typename I>
using accumulate_t = typename accumulate<List, F, I>::type;



};  // namespace stl

#endif  // TYPELIST_H_

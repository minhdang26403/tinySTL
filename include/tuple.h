#ifndef TUPLE_H_
#define TUPLE_H_

#include <iostream>
#include "type_traits.h"
#include "typelist.h"

namespace stl {

template <typename... Types>
class tuple;

template <typename Head, typename... Tail>
class tuple<Head, Tail...> {
 public:
  template <typename VHead, typename... VTail,
            typename = enable_if_t<sizeof...(VTail) == sizeof...(Tail)>>
  tuple(VHead&& vhead, VTail&&... vtail)
      : head(std::forward<VHead>(vhead)), tail(std::forward<VTail>(vtail)...){};

  template <typename VHead, typename... VTail,
            typename = enable_if_t<sizeof...(VTail) == sizeof...(Tail)>>
  tuple(const tuple<VHead, VTail...>& other)
      : head(other.get_head()), tail(other.get_tail()){};

  Head& get_head() { return head; }

  const Head& get_head() const { return head; }

  tuple<Tail...>& get_tail() { return tail; }

  const tuple<Tail...>& get_tail() const { return tail; }

 private:
  Head head;
  tuple<Tail...> tail;
};

template <>
class tuple<> {};

/*====================Non-member functions====================*/

// make_tuple
template <typename... Types>
auto make_tuple(Types&&... elems) {
  return tuple<decay_t<Types>...>(std::forward<Types>(elems)...);
}

// get
template <unsigned N>
struct tuple_get {
  template <typename Head, typename... Tail>
  static auto apply(const tuple<Head, Tail...>& t) {
    return tuple_get<N - 1>::apply(t.get_tail());
  }
};

template <>
struct tuple_get<0> {
  template <typename Head, typename... Tail>
  static auto apply(const tuple<Head, Tail...>& t) {
    return t.get_head();
  }
};

template <unsigned N, typename... Types>
auto get(const tuple<Types...>& t) {
  return tuple_get<N>::apply(t);
}

// operator==
bool operator==(const tuple<>&, const tuple<>&) {
  return true;
}

template <typename Head1, typename... Tail1, typename Head2, typename... Tail2,
          typename = enable_if_t<sizeof...(Tail1) == sizeof...(Tail2)>>
bool operator==(const tuple<Head1, Tail1...>& lhs, const tuple<Head2, Tail2...> rhs) {
  return lhs.get_head() == rhs.get_head() && lhs.get_tail() == rhs.get_tail();
}

// operator<<
void print_tuple(std::ostream& strm, const tuple<>, bool is_first) {
  strm << ')';
}

template<typename Head, typename... Tail>
void print_tuple(std::ostream& strm, const tuple<Head, Tail...> t, bool is_first=true) {
  strm << (is_first ? "(" : ", ");
  strm << t.get_head();
  print_tuple(strm, t.get_tail(), false);
}

template<typename... Types>
std::ostream& operator<<(std::ostream& strm, const tuple<Types...>& t) {
  print_tuple(strm, t);
  return strm;
}

template <>
std::ostream& operator<<(std::ostream& strm, const tuple<>& t) {
  strm << "()";
  return strm;
}

// tuple - is_empty
template <typename Tuple>
struct is_tuple_empty : false_type {};

template<>
struct is_tuple_empty<tuple<>> : true_type {};

// tuple - front
template<typename Head, typename... Tail>
struct front<tuple<Head, Tail...>> {
  using type = Head;
};

// tuple - pop_front
template<typename Head, typename... Tail>
struct pop_front<tuple<Head, Tail...>> {
  using type = tuple<Tail...>;
};

// tuple - push_front
template<typename... Types, typename Element>
struct push_front<tuple<Types...>, Element> {
  using type = tuple<Element, Types...>;
};

// tuple - push_back
template <typename... Types, typename Element>
struct push_back<tuple<Types...>, Element> {
  using type = tuple<Types..., Element>;
};

};  // namespace stl

#endif  // TUPLE_H_

#ifndef ARRAY_H_
#define ARRAY_H_

#include <cstring>

#include "type_traits.h"

/*============================================================
=============================Array============================
==============================================================*/

namespace stl {

template <typename T, size_t N>
struct array {
  /*==========Member types==========*/
  using value_type = T;
  using size_type = size_t;
  using difference_type = ptrdiff_t;
  using reference = value_type&;
  using const_reference = const value_type&;
  using pointer = value_type*;
  using const_pointer = const value_type*;
  using iterator = value_type*;
  using const_iterator = const value_type*;

  /*==========Member functions==========*/

  // Element access
  // at
  constexpr reference at(size_type pos) { return data_[pos]; }
  constexpr const_reference at(size_type pos) const { return data_[pos]; }

  // operator[]
  constexpr reference operator[](size_type pos) { return data_[pos]; }
  constexpr const_reference operator[](size_type pos) const {
    return data_[pos];
  }

  // front
  constexpr reference front() { return data_[0]; }
  constexpr const_reference front() const { return data_[0]; }

  // back
  constexpr reference back() { return data_[N - 1]; }
  constexpr const_reference back() const { return data_[N - 1]; }

  // data
  constexpr pointer data() { return data_; }
  constexpr const_pointer data() const { return data_; }

  // Iterators
  // begin / end
  constexpr iterator begin() { return iterator(data()); }
  constexpr const_iterator begin() const { return const_iterator(data()); }
  constexpr iterator end() { return iterator(data() + N); }
  constexpr const_iterator end() const { return const_iterator(data() + N); }

  // cbegin / cend
  constexpr const_iterator cbegin() const { return begin(); }
  constexpr const_iterator cend() const { return end(); }

  // Capacity
  // empty
  constexpr bool empty() const { return N == 0; }

  // size
  constexpr size_type size() const { return N; }

  // max_size
  constexpr size_type max_size() const { return N; }

  // Operations
  // fill
  void fill(const T& value) {
    for (size_t i = 0; i < N; i++) {
      data_[i] = value;
    }
  }
  
  T data_[N];
};

};  // namespace stl

#endif  // ARRAY_H_
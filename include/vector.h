#ifndef VECTOR_H_
#define VECTOR_H_

#include <iostream>

#include "type_traits.h"

/*============================================================
=============================Vector===========================
==============================================================*/

namespace stl {

template <typename T>
class vector {
 public:
  /*==========Member types==========*/
  using value_type = T;
  using size_type = size_t;
  using difference_type = ptrdiff_t;
  using reference = value_type&;
  using const_reference = const value_type&;
  using pointer = value_type*;
  using const_pointer = const value_type*;
  using iterator = pointer;
  using const_iterator = const_pointer;

  /*==========Member functions==========*/

  // default constructor
  vector() : data_(new T[INITIAL_CAPACITY]) {}

  // custom constructor
  vector(size_type count, const T& value) { assign(count, value); }

  // custom constructor
  explicit vector(size_type count) { assign(count, T()); }

  template <typename InputIt, typename = enable_if_t<is_pointer_v<InputIt>>>
  vector(InputIt first, InputIt last) {
    assign(first, last);
  }

  // copy constructor
  vector(const vector& other)
      : data_(new T[other.capacity_]),
        size_(other.size_),
        capacity_(other.capacity_) {
    copy_data(data_, other.data_, size_);
  }

  // move constructor
  vector(vector&& src) noexcept { src.swap(*this); }

  // initializer list constructor
  vector(std::initializer_list<value_type> init) { assign(init); }

  // destructor (noexcept by default)
  ~vector() { delete[] data_; }

  // copy assignment operator
  vector& operator=(const vector& other) {
    vector copy(other);
    copy.swap(*this);
    return *this;
  }

  // move assignment operator
  vector& operator=(vector&& other) noexcept {
    other.swap(*this);
    other.size_ = 0;
    return *this;
  }

  vector& operator=(std::initializer_list<T> ilist) {
    assign(ilist);
    return *this;
  }

  // assign
  void assign(size_type count, const T& value) {
    if (capacity_ < count) {
      realloc(count);
    }

    for (size_type i = 0; i < count; i++) {
      data_[i] = value;
    }
    size_ = count;
  }

  template <typename InputIt>
  void assign(InputIt first, InputIt last) {
    size_type sz = last - first;
    if (capacity_ < sz) {
      realloc(sz);
    }
    int i = 0;
    for (auto it = first; it != last; it++) {
      data_[i++] = *it;
    }
    size_ = sz;
  }

  void assign(std::initializer_list<T> ilist) {
    auto ilist_sz = ilist.size();
    if (capacity_ < ilist_sz) {
      realloc(ilist_sz);
    }

    int i = 0;
    for (const auto& ele : ilist) {
      data_[i++] = ele;
    }
    size_ = ilist_sz;
  }

  // Element access
  // at
  reference at(size_type pos) { return data_[pos]; }
  const_reference at(size_type pos) const { return data_[pos]; }

  // operator[]
  reference operator[](size_type pos) { return data_[pos]; }
  const_reference operator[](size_type pos) const { return data_[pos]; }

  // front
  reference front() { return data_[0]; }
  const_reference front() const { return data_[0]; }

  // back
  reference back() { return data_[size_ - 1]; }
  const_reference back() const { return data_[size_ - 1]; }

  // data
  pointer data() noexcept { return data_; }
  const_pointer data() const noexcept { return data_; }

  // Iterators
  // begin / cbegin
  iterator begin() noexcept { return static_cast<iterator>(data()); }
  const_iterator begin() const noexcept {
    return static_cast<const_iterator>(data());
  }
  const_iterator cbegin() const noexcept { return begin(); }

  // end / cend
  iterator end() noexcept { return static_cast<iterator>(data() + size_); }
  const_iterator end() const noexcept {
    return static_cast<const_iterator>(data() + size_);
  }
  const_iterator cend() const noexcept { return end(); }

  // Capacity
  // empty
  bool empty() const noexcept { return size_ == 0; }

  // size
  size_type size() const noexcept { return size_; }

  // reserve
  void reserve(size_type new_cap) {
    if (capacity_ < new_cap) {
      expand_capacity(new_cap);
    }
  }

  // capacity
  constexpr size_type capacity() const noexcept { return capacity_; }

  // Modifiers
  // clear
  void clear() noexcept { size_ = 0; }

  // insert
  iterator insert(const_iterator pos, const T& value) {
    return insert(pos, 1, value);
  }

  iterator insert(const_iterator pos, T&& value) {
    size_type idx = pos - begin();
    prep_for_insertion(idx, 1);
    data_[idx] = std::move(value);
    size_++;

    return static_cast<iterator>(data_ + idx);
  }

  iterator insert(const_iterator pos, size_type count, const T& value) {
    // must calculate idx first; otherwise, `pos` iterator
    // may be invalidated due to reallocation
    size_type idx = pos - begin();
    if (count == 0) {
      return static_cast<iterator>(data_ + idx);
    }

    prep_for_insertion(idx, count);
    for (size_type i = 0; i < count; i++) {
      data_[idx + i] = value;
    }
    size_ += count;

    return static_cast<iterator>(data_ + idx);
  }

  template <typename InputIt, typename = enable_if_t<is_pointer_v<InputIt>>>
  iterator insert(const_iterator pos, InputIt first, InputIt last) {
    size_type idx = pos - begin();
    if (first == last) {
      return static_cast<iterator>(data_ + idx);
    }

    size_type count = last - first;
    prep_for_insertion(idx, count);
    size_type i = idx;
    for (auto it = first; it != last; it++) {
      data_[i++] = *it;
    }
    size_ += count;

    return static_cast<iterator>(data_ + idx);
  }

  iterator insert(const_iterator pos, std::initializer_list<T> ilist) {
    size_type idx = pos - begin();
    size_type count = ilist.size();
    if (count == 0) {
      return static_cast<iterator>(data_ + idx);
    }
    
    prep_for_insertion(idx, count);
    size_type i = idx;
    for (const auto& ele : ilist) {
      data_[i++] = ele;
    }
    size_ += count;

    return static_cast<iterator>(data_ + idx);
  }

 private:
  void swap(vector& rhs) noexcept {
    using std::swap;
    swap(data_, rhs.data_);
    swap(size_, rhs.size_);
    swap(capacity_, rhs.capacity_);
  }

  void realloc(size_type sz) {
    T* new_data = new T[sz];
    delete data_;
    data_ = new_data;
    capacity_ = sz;
  }

  void copy_data(T* dst, T* src, size_type sz) {
    for (size_type i = 0; i < sz; i++) {
      dst[i] = src[i];
    }
  }

  void move_data(T* dst, T* src, size_type sz) {
    for (size_type i = 0; i < sz; i++) {
      dst[i] = std::move(src[i]);
    }
  }

  void expand_capacity(size_type new_cap) {
    T* new_data = new T[new_cap];
    if constexpr (is_nothrow_move_assignable_v<T>) {
      move_data(new_data, data_, size_);
    } else {
      copy_data(new_data, data_, size_);
    }
    delete data_;
    data_ = new_data;
    capacity_ = new_cap;
  }

  // make room for inserting `count` elements at index `idx`
  void prep_for_insertion(size_type idx, size_type count) {
    if (size_ + count > capacity_) {
      expand_capacity(std::max(size_ + count, capacity_ * 2));
    }

    for (size_type i = size_ + count - 1; i > idx + count - 1; i--) {
      data_[i] = data_[i - count];
    }
  }

  static constexpr int INITIAL_CAPACITY = 4;

  T* data_{};
  size_t size_{};
  size_t capacity_{};
};

};  // namespace stl

#endif  // VECTOR_H_
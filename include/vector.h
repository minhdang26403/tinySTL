#ifndef VECTOR_H_
#define VECTOR_H_

#include <iostream>
#include <memory>

#include "type_traits.h"

/*============================================================
=============================Vector===========================
==============================================================*/

namespace stl {

template <typename T, typename Allocator = std::allocator<T>>
class vector {
 public:
  /*==========Member types==========*/
  using value_type = T;
  using allocator = Allocator;
  using size_type = size_t;
  using difference_type = ptrdiff_t;
  using reference = value_type&;
  using const_reference = const value_type&;
  using pointer = typename std::allocator_traits<Allocator>::pointer;
  using const_pointer =
      typename std::allocator_traits<Allocator>::const_pointer;
  using iterator = pointer;
  using const_iterator = const_pointer;

  /*==========Member functions==========*/

  // default constructor
  vector()
      : data_(std::allocator_traits<Allocator>::allocate(allocator_,
                                                         INITIAL_CAPACITY)),
        capacity_(INITIAL_CAPACITY) {}

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
      : data_(std::allocator_traits<Allocator>::allocate(allocator_,
                                                         other.capacity_)),
        size_(other.size_),
        capacity_(other.capacity_) {
    copy_data(data_, other.data_, size_);
  }

  // move constructor
  vector(vector&& src) noexcept { src.swap(*this); }

  // initializer list constructor
  vector(std::initializer_list<value_type> init) { assign(init); }

  // destructor (noexcept by default)
  ~vector() {
    std::allocator_traits<Allocator>::deallocate(allocator_, data_, capacity_);
  }

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
    assign_impl(count, [&]() {
      for (size_type i = 0; i < count; i++) {
        data_[i] = value;
      }
    });
  }

  template <typename InputIt>
  void assign(InputIt first, InputIt last) {
    assign_impl(last - first, [&]() {
      int i = 0;
      for (auto it = first; it != last; it++) {
        data_[i++] = *it;
      }
    });
  }

  void assign(std::initializer_list<T> ilist) {
    assign_impl(ilist.size(), [&]() {
      int i = 0;
      for (const auto& ele : ilist) {
        data_[i++] = ele;
      }
    });
  }

  template <typename AssignFunc>
  void assign_impl(size_type count, AssignFunc&& assign_func) {
    if (capacity_ < count) {
      realloc(count);
    }
    assign_func();
    size_ = count;
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
    return insert_impl(pos, 1, [&](size_type idx) { data_[idx] = value; });
  }

  iterator insert(const_iterator pos, T&& value) {
    return insert_impl(pos, 1,
                       [&](size_type idx) { data_[idx] = std::move(value); });
  }

  iterator insert(const_iterator pos, size_type count, const T& value) {
    return insert_impl(pos, count, [&](size_type idx) {
      for (size_type i = 0; i < count; i++) {
        data_[idx + i] = value;
      }
    });
  }

  template <typename InputIt, typename = enable_if_t<is_pointer_v<InputIt>>>
  iterator insert(const_iterator pos, InputIt first, InputIt last) {
    return insert_impl(pos, last - first, [&](size_type idx) {
      for (auto it = first; it != last; it++) {
        data_[idx++] = *it;
      }
    });
  }

  iterator insert(const_iterator pos, std::initializer_list<T> ilist) {
    return insert_impl(pos, ilist.size(), [&](size_type idx) {
      for (const auto& ele : ilist) {
        data_[idx++] = ele;
      }
    });
  }

  template <typename InsertFunc>
  iterator insert_impl(const_iterator pos, size_type count,
                       InsertFunc&& insert_func) {
    // must calculate idx first; otherwise, `pos` iterator
    // may be invalidated due to reallocation
    size_type idx = pos - begin();
    if (count == 0) {
      return static_cast<iterator>(data_ + idx);
    }
    // prep_for_insertion can invalidate iterator
    prep_for_insertion(idx, count);
    insert_func(idx);
    size_ += count;
    return static_cast<iterator>(data_ + idx);
  }

  template <typename... Args>
  iterator emplace(const_iterator pos, Args&&... args) {
    size_type idx = pos - begin();
    prep_for_insertion(idx, 1);
    iterator iter = &data_[idx];
    std::allocator_traits<Allocator>::construct(allocator_, iter,
                                                std::forward<Args>(args)...);
    size_++;
    return iter;
  }

  enable_if_t<is_move_assignable_v<T> ,iterator> erase(const_iterator pos) {
    size_--;
    if (pos == end()) {
      return end();
    }

    size_type idx = pos - begin();
    for (size_type i = idx; i < size_; i++) {
      data_[i] = data_[i + 1];
    }
    return static_cast<iterator>(data_ + idx);
  }

  void push_back(const T& value) { insert(end(), value); }

  void push_back(T&& value) { insert(end(), std::move(value)); }

  template <typename... Args>
  reference emplace_back(Args&&... args) {
    auto it = emplace(end(), std::forward<Args>(args)...);
    return data_[it - begin()];
  }

  void pop_back() { size_--; }

  void resize(size_type count) { resize_impl(count, value_type()); }

  void resize(size_type count, const value_type& value) {
    resize_impl(count, value);
  }

  void resize_impl(size_type count, const value_type& value) {
    if (size() >= count) {
      size_ = count;
      return;
    }

    if (count > capacity_) {
      expand_capacity(std::max(count, capacity_ * 2));
    }
    for (size_type i = 0; i < count - size_; i++) {
      data_[size_ + i] = value;
    }
    size_ = count;
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
    delete [] data_;
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
    delete [] data_;
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

  static constexpr size_t INITIAL_CAPACITY = 4;

  T* data_{};
  size_t size_{};
  size_t capacity_{};
  Allocator allocator_{};
};

};  // namespace stl

#endif  // VECTOR_H_
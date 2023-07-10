#ifndef MEMORY_H_
#define MEMORY_H_

#include "type_traits.h"

namespace stl {
template <typename T>
struct default_delete {
  constexpr default_delete() noexcept = default;

  template <typename U, typename = enable_if_t<is_convertible_v<U*, T*>>>
  default_delete(const default_delete<U>& d) noexcept;

  void operator()(T* ptr) { 
    static_assert(sizeof(T) > 0, "default delete cannot delete incomplete type");
    static_assert(!is_void_v<T>, "default delete cannot delete void type");
    delete ptr;
  }
};

template <typename T>
struct default_delete<T[]> {
  void operator()(T* ptr) { delete[] ptr; }
};

template <class T, class Deleter = default_delete<T>>
class unique_ptr {
 public:
  using pointer = T*;
  using element_type = T;
  using deleter_type = Deleter;

  // constructors
  constexpr unique_ptr() noexcept {};

  explicit unique_ptr(pointer p) noexcept : ptr_(p){};

  unique_ptr(unique_ptr&& u) noexcept : ptr_(u.ptr_) { ptr_ = nullptr; };

  unique_ptr(nullptr_t) noexcept : unique_ptr(){};

  // destructor
  ~unique_ptr() { deleter_(ptr_); }

  // assignment
  unique_ptr& operator=(unique_ptr&& u) noexcept {
    ptr_ = u.release();
    deleter_ = u.get_deleter();
  }
  unique_ptr& operator=(nullptr_t) noexcept;

  // observers
  add_lvalue_reference_t<T> operator*() const { return *ptr_; }

  pointer operator->() const noexcept { return ptr_; }

  pointer get() const noexcept { return ptr_; }

  deleter_type& get_deleter() noexcept { return deleter_; }

  const deleter_type& get_deleter() const noexcept { return deleter_; }

  explicit operator bool() const noexcept { return get() != nullptr; }

  // modifiers
  pointer release() noexcept {
    pointer ptr = ptr_;
    ptr_ = nullptr;
    return ptr;
  }
  void reset(pointer p = pointer()) noexcept {
    auto old_ptr = ptr_;
    ptr_ = p;
    if (old_ptr) {
      get_deleter()(old_ptr);
    }
  }

 private:
  pointer ptr_{};
  deleter_type deleter_{};
};

// template <class T, class D>
// class unique_ptr<T[], D> {
//  public:
//   typedef implementation - defined pointer;
//   typedef T element_type;
//   typedef D deleter_type;

//   // constructors
//   constexpr unique_ptr() noexcept;
//   explicit unique_ptr(pointer p) noexcept;
//   unique_ptr(pointer p, see below d) noexcept;
//   unique_ptr(pointer p, see below d) noexcept;
//   unique_ptr(unique_ptr&& u) noexcept;
//   unique_ptr(nullptr_t) noexcept : unique_ptr() {}

//   // destructor
//   ~unique_ptr();

//   // assignment
//   unique_ptr& operator=(unique_ptr&& u) noexcept;
//   unique_ptr& operator=(nullptr_t) noexcept;

//   // observers
//   T& operator[](size_t i) const;
//   pointer get() const noexcept;
//   deleter_type& get_deleter() noexcept;
//   const deleter_type& get_deleter() const noexcept;
//   explicit operator bool() const noexcept;

//   // modifiers
//   pointer release() noexcept;
//   void reset(pointer p = pointer()) noexcept;
//   void reset(nullptr_t) noexcept;
//   template <class U>
//   void reset(U) = delete;
//   void swap(unique_ptr& u) noexcept;
// };

};  // namespace stl

#endif // MEMORY_H_
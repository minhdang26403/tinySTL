#ifndef MEMORY_H_
#define MEMORY_H_

#include "type_traits.h"

namespace stl {
template <typename T>
struct default_delete {
  static_assert(!stl::is_function_v<T>,
                "default_delete cannot be instantiated for function types");
  /**
   * Constructs a default delete object
   */
  constexpr default_delete() noexcept = default;

  /**
   * Copy constructor. This converting constructor makes possible the implicit
   * conversion from unique_ptr<Derived> to unique_ptr<Base>
   */
  template <typename U, typename = stl::enable_if_t<stl::is_convertible_v<U*, T*>>>
  default_delete(const default_delete<U>&) noexcept {}

  /**
   * Calls `delete` on `ptr`
   * @param ptr an object to delete
   */
  void operator()(T* ptr) const noexcept {
    static_assert(sizeof(T) > 0,
                  "default_delete cannot delete incomplete type");
    static_assert(!stl::is_void_v<T>, "default_delete cannot delete void type");
    delete ptr;
  }
};

template <typename T>
struct default_delete<T[]> {
  /**
   * Constructs a default delete object
   */
  constexpr default_delete() noexcept = default;

  /**
   * Copy constructor. This converting constructor makes possible the implicit
   * conversion from unique_ptr<Derived> to unique_ptr<Base>
   */
  template <typename U,
            typename = stl::enable_if_t<stl::is_convertible_v<U (*)[], T (*)[]>>>
  default_delete(const default_delete<U[]>&) noexcept {}

  /**
   * Calls `delete` on `ptr`
   * @param ptr an array to delete
   */
  void operator()(T* ptr) const noexcept {
    static_assert(sizeof(T) > 0,
                  "default_delete cannot delete incomplete type");
    static_assert(!stl::is_void_v<T>, "default_delete cannot delete void type");
    delete[] ptr;
  }
};

template <typename T, typename Deleter = default_delete<T>>
class unique_ptr {
 public:
  using pointer = T*;
  using element_type = T;
  using deleter_type = Deleter;

  // constructors
  constexpr unique_ptr() noexcept {};

  explicit unique_ptr(pointer p) noexcept : ptr_(p){};

  unique_ptr(unique_ptr&& u) noexcept : ptr_(u.release()){};

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
//   using pointer = T*;
//   using element_type = T;
//   using deleter_type = Deleter;

//   // constructors
//   constexpr unique_ptr() noexcept;

//   explicit unique_ptr(pointer p) noexcept : ptr_(p){};

//   unique_ptr(unique_ptr&& u) noexcept : ptr_(u.release()){};

//   unique_ptr(nullptr_t) noexcept : unique_ptr() {}

//   // destructor
//   ~unique_ptr() { deleter_(ptr_); }

//   // assignment
//   unique_ptr& operator=(unique_ptr&& u) noexcept;
//   unique_ptr& operator=(nullptr_t) noexcept;

//   // observers
//   T& operator[](size_t i) const { return *(ptr_ + i); }
//   pointer get() const noexcept { return ptr_; }
//   deleter_type& get_deleter() noexcept { return deleter_; }
//   const deleter_type& get_deleter() const noexcept { return deleter_; }
//   explicit operator bool() const noexcept { return get() != nullptr; }

//   // modifiers
//   pointer release() noexcept {
//     pointer ptr = ptr_;
//     ptr_ = nullptr;
//     return ptr;
//   }

//   void reset(pointer p = pointer()) noexcept {
//     if (p != ptr_) {
//       auto old_ptr = release();
//       ptr_ = p;
//       deleter_(old_ptr);
//     }
//   }

//  private:
//   pointer ptr_{};
//   deleter_type deleter_{};
// };

};  // namespace stl

#endif  // MEMORY_H_
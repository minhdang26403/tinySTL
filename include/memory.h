#ifndef MEMORY_H_
#define MEMORY_H_

#include "type_traits.h"
#include "utility.h"

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
  template <typename U,
            typename = stl::enable_if_t<stl::is_convertible_v<U*, T*>>>
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
  template <typename U, typename = stl::enable_if_t<
                            stl::is_convertible_v<U (*)[], T (*)[]>>>
  default_delete(const default_delete<U[]>&) noexcept {}

  /**
   * Calls `delete` on `ptr`
   * @param ptr an array to delete
   */
  void operator()(T* ptr) const noexcept {
    static_assert(sizeof(T) > 0,
                  "default_delete cannot delete incomplete type");
    delete[] ptr;
  }
};

// template <typename T, typename Deleter = default_delete<T>>
// class unique_ptr {
//  public:
//   using pointer = T*;
//   using element_type = T;
//   using deleter_type = Deleter;

//   // There is problem with constructor. Fix later
//   template <typename Del>
//   using enable_if_deleter_default_constructible =
//       stl::enable_if_t<stl::is_default_constructible_v<Del> &&
//                        !stl::is_pointer_v<Del>>;

//   template <typename UPtr, typename U>
//   using enable_if_move_convertible =
//       stl::enable_if_t<stl::is_convertible_v<typename UPtr::pointer, pointer>
//       &&
//                        !stl::is_array_v<U>>;

//   template <typename UDeleter>
//   using enable_if_deleter_convertible =
//       stl::enable_if_t<(stl::is_reference_v<Deleter> &&
//                         stl::is_same_v<Deleter, UDeleter>) ||
//                        (!stl::is_reference_v<Deleter> &&
//                         stl::is_convertible_v<UDeleter, Deleter>)>;

//   /**
//    * Constructs a stl::unique_ptr that owns nothing. Value-initializes the
//    * stored pointer and the stored deleter. Requires that `Deleter` is
//    default
//    * constructible
//    */
//   template <typename = stl::enable_if_t<
//                 stl::is_default_constructible_v<deleter_type> &&
//                 !stl::is_pointer_v<deleter_type>>>
//   constexpr unique_ptr() noexcept {}

//   template <typename = stl::enable_if_t<
//                 stl::is_default_constructible_v<deleter_type> &&
//                 !stl::is_pointer_v<deleter_type>>>
//   constexpr unique_ptr(stl::nullptr_t) noexcept {}

//   /**
//    * Constructs a stl::unique_ptr that owns `p`. Value-initializes the
//    * stored deleter. Requires that `Deleter` is default constructible
//    * @param p a pointer to an object to manage
//    */
//   template <typename = stl::enable_if_t<
//                 stl::is_default_constructible_v<deleter_type> &&
//                 !stl::is_pointer_v<deleter_type>>>
//   explicit unique_ptr(pointer p) noexcept : ptr_(p){}

//   /**
//    * Constructs a stl::unique_ptr that owns `p`, initializing the stored
//    pointer
//    * with `p` and the stored deleter with `d`
//    * @param p a pointer to an object to manage
//    * @param d a deleter to use to destroy the object
//    */
//   template <typename D>
//   unique_ptr(
//       pointer p, D&& d,
//       stl::enable_if_t<stl::is_constructible_v<deleter_type, decltype(d)>>* =
//           nullptr) noexcept
//       : ptr_(p), deleter_(stl::forward<decltype(d)>(d)){}

//   /**
//    * Constructs a unique_ptr by transferring ownership from `u` to `*this`
//    and
//    * stores the null pointer in `u`.
//    * @param u another smart pointer to acquire ownership from
//    */
//   template <
//       typename =
//       stl::enable_if_t<stl::is_move_constructible_v<deleter_type>>>
//   unique_ptr(unique_ptr&& u) noexcept
//       : ptr_(u.release()),
//         deleter_(stl::forward<deleter_type>(u.get_deleter())){}

//   /**
//    * Constructs a unique_ptr by transferring ownership from `u` to `*this`.
//    * @param u another smart pointer to acquire ownership from
//    */
//   template <
//       typename U, typename E,
//       typename = stl::enable_if_t<
//           stl::is_convertible_v<typename unique_ptr<U, E>::pointer, pointer>
//           && !stl::is_array_v<U>>,
//       typename = stl::enable_if_t<(stl::is_reference_v<Deleter> &&
//                                    stl::is_same_v<Deleter, E>) ||
//                                   (!stl::is_reference_v<Deleter> &&
//                                    stl::is_convertible_v<E, Deleter>)>>
//   unique_ptr(unique_ptr<U, E>&& u) noexcept
//       : ptr_(u.release()), deleter_(stl::forward<E>(u.get_deleter())) {}

//   /**
//    * Copy constructor is explicitly deleted
//    */
//   unique_ptr(const unique_ptr&) = delete;

//   /**
//    * Destructor
//    */
//   ~unique_ptr() { get_deleter(get()); }

//   // assignment
//   unique_ptr& operator=(unique_ptr&& u) noexcept {
//     reset(u.release());
//     deleter_ = u.get_deleter();
//   }
//   unique_ptr& operator=(nullptr_t) noexcept;

//   // observers
//   add_lvalue_reference_t<T> operator*() const { return *ptr_; }

//   pointer operator->() const noexcept { return ptr_; }

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
//     auto old_ptr = ptr_;
//     ptr_ = p;
//     if (old_ptr) {
//       get_deleter()(old_ptr);
//     }
//   }

//  private:
//   pointer ptr_{};
//   deleter_type deleter_{};
// };

template <typename T>
class unique_ptr {
 public:
  /*====================Member types====================*/
  using pointer = T*;
  using element_type = T;

  /*====================Member functions====================*/

  /**
   * Constructs a stl::unique_ptr that owns nothing. Value-initializes the
   * stored pointer
   */
  constexpr unique_ptr() noexcept {}
  constexpr unique_ptr(stl::nullptr_t) noexcept {}

  /**
   * Constructs a stl::unique_ptr that owns `p`
   * @param p a pointer to an object to manage
   */
  explicit unique_ptr(pointer p) noexcept : ptr_(p) {}

  /**
   * Constructs a unique_ptr by transferring ownership from `u` to `*this` and
   * stores the null pointer in `u`.
   * @param u another smart pointer to acquire ownership from
   */
  unique_ptr(unique_ptr&& u) noexcept : ptr_(u.release()) {}

  /**
   * Constructs a unique_ptr by transferring ownership from `u` to `*this` and
   * stores the null pointer in `u`.
   * @param u another smart pointer to acquire ownership from
   */
  template <typename U>
  unique_ptr(unique_ptr<U>&& u) noexcept : ptr_(u.release()) {}

  /**
   * Copy constructor is explicitly deleted
   */
  unique_ptr(const unique_ptr&) = delete;

  /**
   * Destructor
   */
  ~unique_ptr() {
    if (get() != nullptr) {
      delete ptr_;
    }
  }

  /**
   * Move assignment operator. Transfer ownership from `r` to `*this`
   * @param r smart pointer from which ownership will be transferred
   * @return reference to this unique_ptr
   */
  unique_ptr& operator=(unique_ptr&& r) noexcept {
    reset(r.release());
    return *this;
  }

  /**
   * Converting assignment operator. Transfer ownership from `r` to `*this`.
   * This overload participates in overload resolution only if `U` is not an
   * array type and `unique_ptr<U>::pointer` is implicitly convertible to
   * `pointer`
   * @param r smart pointer from which ownership will be transferred
   * @return reference to this unique_ptr
   */
  template <
      typename U,
      typename = stl::enable_if_t<
          !stl::is_array_v<U> &&
          stl::is_convertible_v<typename unique_ptr<U>::pointer, pointer>>>
  unique_ptr& operator=(unique_ptr<U>&& r) noexcept {
    reset(r.release());
    return *this;
  }

  /**
   * Frees the owned resource and owns nothing
   * @return reference to this unique_ptr
   */
  unique_ptr& operator=(stl::nullptr_t) noexcept {
    reset();
    return *this;
  }

  /**
   * Copy assignment operator is explicitly deleted
   * @return reference to this unique_ptr
   */
  unique_ptr& operator=(const unique_ptr&) = delete;

  /*==========Modifiers==========*/
  /**
   * Releases the ownership of the owned object, if any. The caller is
   * responsible for cleaning up the object
   * @return pointer to the managed object or `nullptr` if there was no managed
   * object
   */
  pointer release() noexcept {
    pointer p = ptr_;
    ptr_ = pointer();
    return p;
  }

  /**
   * Replaces the managed object
   * @param ptr pointer to a new object to manage
   */
  void reset(pointer ptr = pointer()) noexcept {
    pointer old_ptr = ptr_;
    ptr_ = ptr;
    if (old_ptr) {
      delete old_ptr;
    }
  }

  /**
   * Swaps this unique_ptr with another unique_ptr `other`
   * @param other another unique_ptr to swap with
   */
  void swap(unique_ptr& other) noexcept {
    unique_ptr temp(stl::move(other));
    other = stl::move(*this);
    *this = stl::move(temp);
  }

  /*==========Observers==========*/
  /**
   * Returns a pointer to the managed object or `nullptr` if no object is owned
   * @return pointer to the managed object or `nullptr` if no object is owned
   */
  pointer get() const noexcept { return ptr_; }

  /**
   * Checks whether `*this` owns an object
   * @return true if `*this` owns an object, false otherwise
   */
  explicit operator bool() const noexcept { return get() != nullptr; }

  /**
   * Provides access to the managed object. The behavior is undefined if this
   * unique_ptr owns nothing
   * @return reference to the managed object
   */
  stl::add_lvalue_reference_t<T> operator*() const
      noexcept(noexcept(*std::declval<pointer>())) {
    return *ptr_;
  }

  /**
   * Provides access to the managed object. The behavior is undefined if this
   * unique_ptr owns nothing
   * @return pointer to the managed object
   */
  pointer operator->() const noexcept { return ptr_; }

 private:
  pointer ptr_{};
};

template <class T>
class unique_ptr<T[]> {
 public:
  using pointer = T*;
  using element_type = T;

  template <typename From>
  struct check_array_pointer_conversion : stl::is_same<From, pointer> {};

  template <typename FromElem>
  struct check_array_pointer_conversion<FromElem*>
      : stl::bool_constant<
            stl::is_same_v<FromElem*, pointer> ||
            (stl::is_same_v<pointer, element_type*> &&
             stl::is_convertible_v<FromElem (*)[], element_type (*)[]>)> {
  };

  /**
   * Constructs a stl::unique_ptr that owns nothing. Value-initializes the
   * stored pointer
   */
  constexpr unique_ptr() noexcept {
  }
  constexpr unique_ptr(nullptr_t) noexcept {}

  /**
   * Constructs a stl::unique_ptr that owns `p`
   * @param p a pointer to an object to manage
   */
  template <typename U, typename = stl::enable_if_t<
                            check_array_pointer_conversion<U>::value>>
  explicit unique_ptr(U p) noexcept : ptr_(p) {}

  /**
   * Constructs a unique_ptr by transferring ownership from `u` to `*this` and
   * stores the null pointer in `u`.
   * @param u another smart pointer to acquire ownership from
   */
  unique_ptr(unique_ptr&& u) noexcept : ptr_(u.release()) {}

  /**
   * Constructs a unique_ptr by transferring ownership from `u` to `*this` and
   * stores the null pointer in `u`.
   * @param u another smart pointer to acquire ownership from
   */
  template <typename U>
  unique_ptr(unique_ptr<U>&& u) noexcept : ptr_(u.release()) {}

  /**
   * Copy constructor is explicitly deleted
   */
  unique_ptr(const unique_ptr&) = delete;

  /**
   * Destructor
   */
  ~unique_ptr() {
    if (get() != nullptr) {
      delete [] ptr_;
    }
  }

  /**
   * Move assignment operator. Transfer ownership from `r` to `*this`
   * @param r smart pointer from which ownership will be transferred
   * @return reference to this unique_ptr
   */
  unique_ptr& operator=(unique_ptr&& r) noexcept {
    reset(r.release());
    return *this;
  }

  /**
   * Converting assignment operator. Transfer ownership from `r` to `*this`.
   * This overload participates in overload resolution only if `U` is not an
   * array type and `unique_ptr<U>::pointer` is implicitly convertible to
   * `pointer`
   * @param r smart pointer from which ownership will be transferred
   * @return reference to this unique_ptr
   */
  template <
      typename U,
      typename = stl::enable_if_t<
          stl::is_array_v<U> && stl::is_same_v<pointer, element_type*> &&
          stl::is_same_v<typename unique_ptr<U>::pointer,
                         typename unique_ptr<U>::element_type*> &&
          stl::is_convertible_v<typename unique_ptr<U>::element_type (*)[],
                                element_type (*)[]>>>
  unique_ptr& operator=(unique_ptr<U>&& r) noexcept {
    reset(r.release());
    return *this;
  }

  /**
   * Frees the owned resource and owns nothing
   * @return reference to this unique_ptr
   */
  unique_ptr& operator=(stl::nullptr_t) noexcept {
    reset();
    return *this;
  }

  /**
   * Copy assignment operator is explicitly deleted
   * @return reference to this unique_ptr
   */
  unique_ptr& operator=(const unique_ptr&) = delete;

  /*==========Modifiers==========*/
  /**
   * Releases the ownership of the owned object, if any. The caller is
   * responsible for cleaning up the object
   * @return pointer to the managed object or `nullptr` if there was no managed
   * object
   */
  pointer release() noexcept {
    pointer p = ptr_;
    ptr_ = pointer();
    return p;
  }

  /**
   * Replaces the managed object
   * @param ptr pointer to a new object to manage
   */
  template <typename U, typename = stl::enable_if_t<
                            check_array_pointer_conversion<U>::value>>
  void reset(U ptr) noexcept {
    pointer old_ptr = ptr_;
    ptr_ = ptr;
    if (old_ptr) {
      delete old_ptr;
    }
  }

  /**
   * Replaces the managed object with nullptr
   */
  void reset(stl::nullptr_t = nullptr) noexcept { reset(pointer()); }

  /**
   * Swaps this unique_ptr with another unique_ptr `other`
   * @param other another unique_ptr to swap with
   */
  void swap(unique_ptr& other) noexcept {
    unique_ptr temp(stl::move(other));
    other = stl::move(*this);
    *this = stl::move(temp);
  }

  /*==========Observers==========*/
  /**
   * Returns a pointer to the managed object or `nullptr` if no object is owned
   * @return pointer to the managed object or `nullptr` if no object is owned
   */
  pointer get() const noexcept { return ptr_; }

  /**
   * Checks whether `*this` owns an object
   * @return true if `*this` owns an object, false otherwise
   */
  explicit operator bool() const noexcept { return get() != nullptr; }

  /**
   * Provides access to elements of an array managed by a unique_ptr
   * @param i the index of the element to be returned
   * @return reference to the element at index `i`
   */
  T& operator[](size_t i) const { return get()[i]; }

 private:
  pointer ptr_{};
};

/**
 * Constructs an object of non-array type T and wraps it in a stl::unique_ptr
 * @param args list of arguments to construct the object of type T
 * @return a unique_ptr to the constructed object
 */
template <typename T, typename... Args>
stl::enable_if_t<!stl::is_array_v<T>, stl::unique_ptr<T>> make_unique(
    Args&&... args) {
  return stl::unique_ptr<T>(new T(stl::forward<Args>(args)...));
}

/**
 * Constructs an array of the given dynamic size. The array elements are
 * value-initialized
 * @param size the length of the array to construct
 * @return a unique_ptr to the constructed object
 */
template <typename T>
stl::enable_if_t<stl::is_unbounded_array_v<T>, stl::unique_ptr<T>> make_unique(
    size_t size) {
  return stl::unique_ptr<T>(new stl::remove_extent_t<T>[size]());
}

/**
 * Construction of arrays of known bound is disallowed
 */
template <typename T, typename... Args>
stl::enable_if_t<stl::is_bounded_array_v<T>> make_unique(Args&&...) = delete;

template<typename T>
bool operator==(const unique_ptr<T>& x, stl::nullptr_t) noexcept {
  return !x;
}

template <typename T>
bool operator==(stl::nullptr_t, const unique_ptr<T>& x) noexcept {
  return !x;
}

};  // namespace stl

#endif  // MEMORY_H_
#ifndef EXPRTMPL_H_
#define EXPRTMPL_H_
#include <cassert>
#include <iostream>

/*============================================================
======================Expression Templates====================
==============================================================*/

namespace stl {

// storage array
template <typename T>
class SArray {
 public:
  // create array with initial size
  explicit SArray(size_t s) : storage(new T[s]), storage_size(s) { init(); };

  // copy constructor
  SArray(const SArray<T>& orig)
      : storage(new T[orig.size()]), storage_size(orig.size()) {
    copy(orig);
  };

  // destructor: free memory
  ~SArray() { delete[] storage; }

  // assignment operator
  SArray<T>& operator=(const SArray<T>& orig) {
    if (&orig != this) {
      copy(orig);
    }
    return *this;
  }

  // return size
  size_t size() const { return storage_size; }

  // index operator for constants and variables
  const T& operator[](size_t idx) const { return storage[idx]; }
  T& operator[](size_t idx) { return storage[idx]; }

  void print() const {
    std::cout << "[";
    for (size_t i = 0; i < storage_size - 1; i++) {
      std::cout << storage[i] << ", ";
    }
    std::cout << storage[storage_size - 1] << "]\n";
  }

 protected:
  // init values with default constructor
  void init() {
    for (size_t idx = 0; idx < size(); idx++) {
      storage[idx] = T();
    }
  }

  // copy values of another array
  void copy(const SArray<T>& orig) {
    assert(size() == orig.size());
    for (size_t idx = 0; idx < size(); idx++) {
      storage[idx] = orig.storage[idx];
    }
  }

 private:
  T* storage;
  size_t storage_size;
};

// forward declaration
template <typename T>
class A_Scalar;

// helper class traits template to select whether to refer to
// an expression template node either by value or by reference
template <typename T>
struct A_Traits {
  using ExprRef = const T&;
};

template <typename T>
struct A_Traits<A_Scalar<T>> {
  using ExprRef = A_Scalar<T>;
};

// class for objects that represent the addition of two operands
template <typename T, typename OP1, typename OP2>
class A_Add {
 public:
  // constructor initializes references to operands
  A_Add(const OP1& a, const OP2& b) : op1(a), op2(b){};

  // compute sum when value requested
  T operator[](size_t idx) const { return op1[idx] + op2[idx]; }

  // size is maximum size
  size_t size() const {
    assert(op1.size() == 0 || op2.size() == 0 || op1.size() == op2.size());
    return op1.size() != 0 ? op1.size() : op2.size();
  }

  void print() const {
    std::cout << "[";
    size_t n = size();
    for (size_t i = 0; i < n - 1; i++) {
      std::cout << op1[i] + op2[i] << ", ";
    }
    std::cout << op1[n - 1] + op2[n - 1] << "]\n";
  }

 private:
  typename A_Traits<OP1>::ExprRef op1;
  typename A_Traits<OP2>::ExprRef op2;
};

// class for objects that represent the multiplication of two operands
template <typename T, typename OP1, typename OP2>
class A_Mult {
 public:
  // constructor initializes references to operands
  A_Mult(const OP1& a, const OP2& b) : op1(a), op2(b){};

  // compute product when value requested
  T operator[](size_t idx) const { return op1[idx] * op2[idx]; }

  // size is maximum size
  size_t size() const {
    assert(op1.size() == 0 || op2.size() == 0 || op1.size() == op2.size());
    return op1.size() != 0 ? op1.size() : op2.size();
  }

  void print() const {
    std::cout << "[";
    size_t n = size();
    for (size_t i = 0; i < n - 1; i++) {
      std::cout << op1[i] * op2[i] << ", ";
    }
    std::cout << op1[n - 1] * op2[n - 1] << "]\n";
  }

 private:
  typename A_Traits<OP1>::ExprRef op1;
  typename A_Traits<OP2>::ExprRef op2;
};

// class for objects that represent scalars
template <typename T>
class A_Scalar {
 public:
  // constructor initializes value
  constexpr A_Scalar(const T& v) : s(v){};

  // for index operations, the scalar is the value of each element
  constexpr const T& operator[]([[maybe_unused]] size_t idx) const { return s; }

  // scalars have zero as size
  constexpr size_t size() const { return 0; }

  void print() const {
    std::cout << s << '\n';
  }

 private:
  const T& s;
};

template <typename T, typename Rep = SArray<T>>
class Array {
 public:
  // create array with initial size
  explicit Array(size_t s) : expr_rep(s){};

  // create array from possible implementation
  Array(const Rep& rb) : expr_rep(rb){};

  // assignment operator for same type
  Array& operator=(const Array& b) {
    assert(size() == b.size());
    for (size_t idx = 0; idx < b.size(); idx++) {
      expr_rep[idx] = b[idx];
    }
    return *this;
  }

  // assignment operator for arrays of different type
  template <typename T2, typename Rep2>
  Array& operator=(const Array<T2, Rep2>& b) {
    assert(size() == b.size());
    for (size_t idx = 0; idx < b.size(); idx++) {
      expr_rep[idx] = b[idx];
    }
    return *this;
  }

  // size is size of represented data
  size_t size() const { return expr_rep.size(); }

  // index operator for constants and variables
  decltype(auto) operator[](size_t idx) const {
    assert(idx < size());
    return expr_rep[idx];
  }
  T& operator[](size_t idx) {
    assert(idx < size());
    return expr_rep[idx];
  }

  // return what the array currently represents
  const Rep& rep() const { return expr_rep; }

  Rep& rep() { return expr_rep; }

  void print() const {
    expr_rep.print();
  }

 private:
  Rep expr_rep;  // (access to) the data of the array
};

// addition of two Arrays
template <typename T, typename R1, typename R2>
auto operator+(const Array<T, R1>& a, const Array<T, R2>& b) {
  return Array<T, A_Add<T, R1, R2>>(A_Add<T, R1, R2>(a.rep(), b.rep()));
}

// addition of Array and scalar
template <typename T, typename R1>
auto operator+(const Array<T, R1>& a, const T& s) {
  return Array<T, A_Add<T, R1, A_Scalar<T>>>(
      A_Add<T, R1, A_Scalar<T>>(a.rep(), A_Scalar<T>(s)));
}

// addition of scalar and Array
template <typename T, typename R2>
auto operator+(const T& s, const Array<T, R2>& b) {
  return Array<T, A_Add<T, A_Scalar<T>, R2>>(
      A_Add<T, A_Scalar<T>, R2>(A_Scalar<T>(s), b.rep()));
}

// multiplication of two Arrays
template<typename T, typename R1, typename R2>
auto operator*(const Array<T, R1>& a, const Array<T, R2>& b) {
  return Array<T, A_Mult<T, R1, R2>>(A_Mult<T, R1, R2>(a.rep(), b.rep()));
}

// multiplication of Array and scalar
template<typename T, typename R1>
auto operator*(const Array<T, R1>& a, const T& s) {
  return Array<T, A_Mult<T, R1, A_Scalar<T>>>(A_Mult<T, R1, A_Scalar<T>>(a.rep(), s));
}

// multiplication of scalar and Array
template<typename T, typename R2>
auto operator*(const T& s, const Array<T, R2>& b) {
  return Array<T, A_Mult<T, A_Scalar<T>, R2>>(A_Mult<T, A_Scalar<T>, R2>(s, b.rep()));
}

};  // namespace stl

#endif  // EXPRTMPL_H_
#ifndef CH13_ARRAY_H
#define CH13_ARRAY_H

#include <assert.h>
#include <cstddef>
#include <initializer_list>
#include <iostream>

template <typename T, std::size_t size>
class Array final {
 public:
  Array() {
    if (size) {
      data_ = new T[size];
    } else {
      data_ = nullptr;
    }
  }
  Array(const std::initializer_list<T> list) {
    if (size == 0u || list.size() == 0) {
      data_ = nullptr;
      return;
    }
    unsigned int i = 0u;
    data_ = new T[size];
    for (const auto& v : list) {
      if (i >= size) {
        break;
      }
      data_[i++] = v;
    }
  }

  T& operator[](const std::size_t index) {
    assert(index >= 0u && index < size);
    return data_[index];
  }

  const T& operator[](const std::size_t index) const {
    assert(index >= 0u && index < size);
    return data_[index];
  }

  int length() const;

  ~Array() { delete[] data_; }

 private:
  T* data_;
};

// length is defined outside of the class, template is required
template <typename Z, std::size_t size>
int Array<Z, size>::length() const {
  return size;
}

// template parameters can be used to specify template params for classes
template <typename V, std::size_t size>
void printArray(const Array<V, size>& arr) {
  std::cout << '{';
  if (size != 0u) {
    for (unsigned int i = 0u; i < size; i++) {
      std::cout << arr[i];
      if (i < size - 1u) {
        std::cout << ", ";
      }
    }
  }
  std::cout << "}\n";
}

#endif
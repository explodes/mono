#ifndef NET_EZDB_KEY_H
#define NET_EZDB_KEY_H

#include <vector>

namespace ezdb {

using ::std::vector;

template <typename T>
class Key {
 public:
  // Create a new Key<T> with a value.
  explicit Key(T t) : t_(t) {}

  // No copy/assign
  Key<T>& operator=(const Key<T>&) = delete;
  Key<T>(const Key<T>&) = delete;

  // Equality operator.
  bool operator==(const Key<T>& other) const { return other.t_ == t_; };

  // Get the value of our Key.
  const T& Value() const { return t_; }

  // Convert this key to standard edition: one that can be serialized as binary.
  // TODO(explodes): Better serialization abstraction.
  virtual const Key<vector<char>> ToBinaryKey() const;

 private:
  T t_;
};

template <>
const Key<vector<char>> Key<vector<char>>::ToBinaryKey() const {
  return Key<vector<char>>(t_);
}

}  // namespace ezdb

#endif
#ifndef UTIL_STATS_HISTOGRAM_H
#define UTIL_STATS_HISTOGRAM_H

#include <stdint.h>
#include <algorithm>
#include <cmath>
#include <deque>
#include <iostream>
#include <shared_mutex>
#include <utility>
#include <vector>

namespace util {
namespace stats {

using std::deque;
using std::ostream;
using std::shared_lock;
using std::shared_mutex;
using std::unique_lock;
using std::vector;

template <typename T>
class Histogram;

template <typename T>
class Bucket {
 public:
  Bucket(const T min, const T max, uint64_t count)
      : min_(min), max_(max), count_(count) {}

  T min() const { return min_; };
  T max() const { return max_; };
  uint64_t count() const { return count_; };

  bool operator==(const Bucket& rhs) const {
    return (min_ == rhs.min_) && (max_ == rhs.max_) && (count_ == rhs.count_);
  }

  template <typename U>
  friend ostream& operator<<(ostream&, const Bucket<U>&);

  friend class Histogram<T>;

 private:
  const T min_;
  const T max_;
  uint64_t count_;
};

template <typename T>
ostream& operator<<(ostream& out, const Bucket<T>& b) {
  return out << "Bucket(" << b.min_ << "," << b.max_ << "," << b.count_ << ")";
}

template <typename T>
class Histogram {
 public:
  Histogram() : mutex_(), values_(), max_num_values_(DEFAULT_MAX_VALUES) {}
  Histogram(uint64_t max_num_values)
      : mutex_(), values_(), max_num_values_(max_num_values) {}

  Histogram(const Histogram&) = delete;
  Histogram& operator=(Histogram const&) = delete;

  void Put(T value);
  void MakeHistogram(vector<Bucket<T>>& out) const;

 private:
  static constexpr uint64_t DEFAULT_MAX_VALUES = 256;

  mutable shared_mutex mutex_;
  deque<T> values_;
  uint64_t max_num_values_;
};

template <typename T>
inline void printv(const T& vec) {
  for (const auto val : vec) {
    std::cout << val << " ";
  }
  std::cout << std::endl;
}

template <typename T>
void Histogram<T>::Put(T value) {
  unique_lock lock(mutex_);

  // Remove the oldest element (the element at index 0).
  if (values_.size() > 0 && values_.size() >= max_num_values_) {
    values_.pop_front();
  }
  // Add the new element to the end.
  values_.push_back(value);
}

namespace {

template <typename T>
inline T safeDiv(T a, T b, T def) {
  if (b == 0) {
    return def;
  }
  return a / b;
}

int MedianIndex(int left, int right) {
  int mid = (right - left + 1) / 2 - 1;
  return left + mid + 1;
}

template <typename T>
T InnerQuartileRange(const deque<T>& sorted) {
  int q2_index = MedianIndex(0, sorted.size());
  int q1_index = MedianIndex(0, q2_index);
  int q3_index = MedianIndex(q2_index + 1, sorted.size());

  T q1 = sorted[q1_index];
  T q3 = sorted[q3_index];

  return q3 - q1;
}

}  // namespace

template <typename T>
void Histogram<T>::MakeHistogram(vector<Bucket<T>>& out) const {
  shared_lock lock(mutex_);

  out.clear();

  if (values_.size() == 0) {
    return;
  }

  if (values_.size() == 1) {
    T val = values_.front();
    out.push_back(Bucket<T>(val, val, 1));
    return;
  }

  deque<T> sorted(values_);
  std::sort(sorted.begin(), sorted.end());

  // Freedman-Diaconis: num_buckets = (max-min)/h | h = 2 * IQR * n^(1/3)
  // max - min
  T min_value = sorted.front();
  T max_value = sorted.back();
  T diff = max_value - min_value;
  // n^(1/3)
  auto n_third_root = ::std::pow(static_cast<double>(sorted.size()), 1.0 / 3.0);
  T tn_third_root = static_cast<T>(n_third_root);
  // h = 2 * IQR * n^(1/3)
  auto iqr = InnerQuartileRange(sorted);
  T h = 2 * iqr / tn_third_root;
  // num_buckets
  T num_buckets = safeDiv(diff, h, 1);
  T bucket_width = diff / num_buckets;

  for (const auto val : values_) {
    if (out.size() == 0) {
      out.push_back(Bucket<T>(val, val + bucket_width, 1));
    } else {
      Bucket<T>& last_bucket = out.back();
      if (val > last_bucket.max()) {
        out.push_back(
            Bucket<T>(last_bucket.max(), last_bucket.max() + bucket_width, 1));
      } else {
        ++last_bucket.count_;
      }
    }
  }
}

}  // namespace stats
}  // namespace util

#endif  // UTIL_STATS_HISTOGRAM_H
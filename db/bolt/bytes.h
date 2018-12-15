#include <string>
#include <vector>

namespace bolt {

using ::std::vector;

typedef vector<char> bytes;

inline bool BytesEqual(const bytes& a, const bytes& b) { return a == b; }

}  // namespace bolt

template <>
struct std::hash<::bolt::bytes> {
  std::size_t operator()(const ::bolt::bytes& k) const {
    return hash<string>()(string(k.cbegin(), k.cend()));
  }
};

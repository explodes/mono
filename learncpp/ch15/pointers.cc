#include <iostream>
#include <memory>

class Lifecycle final {
 private:
  int n_;

 public:
  Lifecycle(int n) : n_(n) {
    std::cout << "Lifecycle start " << n_ << std::endl;
  }
  ~Lifecycle() { std::cout << "Lifecycle end " << n_ << std::endl; }

  friend std::ostream& operator<<(std::ostream& out, const Lifecycle& res) {
    return out << "I am a Lifecycle " << res.n_ << std::endl;
  }
};

void takeOwnership(std::unique_ptr<Lifecycle> res) {
  std::cout << "-- taking ownership" << std::endl;
  if (res) std::cout << *res;
}  // the Resource is destroyed here

int main() {
  auto ptr = std::make_unique<Lifecycle>(5);

  std::cout << "main: transferring ownership" << std::endl;
  takeOwnership(std::move(ptr));  // ok: use move semantics

  if (ptr) {
    std::cout << "main: ptr remains: " << *ptr << std::endl;
  } else {
    std::cout << "main: ptr is dead" << std::endl;
  }

  return 0;
}
#include <iostream>
#include <string>

class Base {
 public:
  virtual void stuff() = delete;
  virtual void stuff2() = 0;

  ~Base() { std::cout << "~Base()" << std::endl; }
};

class Derived final : Base {
 public:
  void stuff2() override{};

  ~Derived() { std::cout << "~Derived()" << std::endl; }
};

int main() {
  Derived d;

  d.stuff2();
}
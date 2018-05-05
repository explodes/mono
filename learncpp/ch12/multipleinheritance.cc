#include <iostream>
#include <string>

class BaseA {
 public:
  virtual void stuff() const { std::cout << "BaseA::stuff()" << std::endl; }

  virtual ~BaseA() { std::cout << "~BaseA()" << std::endl; }
};

class BaseB {
 public:
  virtual void stuff() const { std::cout << "BaseB::stuff()" << std::endl; }

  virtual ~BaseB() { std::cout << "~BaseB()" << std::endl; }
};

class Derived final : public BaseA, public BaseB {
 public:
  void stuff() const override {
    BaseA::stuff();
    BaseB::stuff();
    std::cout << "Derived::stuff()" << std::endl;
  };

  ~Derived() { std::cout << "~Derived()" << std::endl; }
};

int main() {
  Derived d;

  d.stuff();
}
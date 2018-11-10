#include <iostream>

void failInt() noexcept(false) { throw 12384; }

void failDouble() noexcept(false) { throw 234.34; }

void failCharPointer() noexcept(false) { throw "~~exception118~~"; }

void failString() noexcept(false) { throw std::string("~~exception42~~"); }

class A {
 public:
  A(int a) {
    if (!a) throw "Wowwwwww";
  }
};

class B : public A {
 public:
  B(int a) try : A(a) {
    std::cout << "Made B";
    std::cout << std::endl;
  } catch (const char* ex) {
    std::cerr << "failed to make A: " << ex << std::endl;
    std::cerr << "rethrowing... " << std::endl;
  }
};

int main() {
  try {
    failInt();
  } catch (const int ex) {
    std::cout << "failed successfully with int " << ex << std::endl;
  }
  try {
    failDouble();
  } catch (const double ex) {
    std::cout << "failed successfully with double " << ex << std::endl;
  }
  try {
    failCharPointer();
  } catch (const char* ex) {
    std::cout << "failed successfully with char* " << ex << std::endl;
  }
  try {
    failString();
  } catch (const std::string& ex) {
    std::cout << "failed successfully with std::string " << ex << std::endl;
  }

  try {
    B b(1);
  } catch (const char* ex) {
    std::cerr << "unexpected failure on B(1): " << ex << std::endl;
  }

  try {
    B b(0);
  } catch (const char* ex) {
    std::cout << "expected failure on B(1): " << ex << std::endl;
  }
}
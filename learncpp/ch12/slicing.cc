#include <functional>  // for std::reference_wrapper
#include <iostream>
#include <vector>

class Base {
 protected:
  int m_value;

 public:
  Base(int value) : m_value(value) {}

  virtual const char* getName() const { return "Base"; }
  int getValue() const { return m_value; }
};

class Derived : public Base {
 public:
  Derived(int value) : Base(value) {}

  virtual const char* getName() const { return "Derived"; }
};

class Derived2 : public Base {
 public:
  Derived2(int value) : Base(value) {}

  virtual const char* getName() const { return "Derived2"; }
};

void slicing() {
  // our vector is a vector of std::reference_wrapper wrapped Base (not Base&)
  std::vector<std::reference_wrapper<Base> > v;
  Base b(5);  // b and d can't be anonymous objects
  Derived d(6);
  v.push_back(b);  // add a Base object to our vector
  v.push_back(d);  // add a Derived object to our vector

  // Print out all of the elements in our vector
  for (unsigned int count = 0u; count < v.size(); ++count)
    std::cout << "I am a " << v[count].get().getName() << " with value "
              << v[count].get().getValue()
              << "\n";  // we use .get() to get our element from the wrapper
}

void frankenObject() {
  Derived d1(5);
  Derived2 d2(6);
  Base& b = d2;

  b = d1;  // this line is problematic

  // But "b" is d2.Derived + d1.Base
  std::cout << "Franken object is a " << b.getName() << " with value "
            << b.getValue() << std::endl;
}

int main() {
  slicing();
  frankenObject();

  return 0;
}
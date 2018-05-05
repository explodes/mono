#include <iostream>

double approx(int num, int den) throw(const char*) {
  if (den == 0) {
    throw "invalid denominator: divide by zero";
  }
  return static_cast<double>(num) / den;
}

int main() {
  int num, den;

  std::cout << "Enter the numerator: ";
  std::cin >> num;

  std::cout << "Enter the denominator: ";
  std::cin >> den;

  double approximated;
  try {
    approximated = approx(num, den);

  } catch (const char* ex) {
    std::cerr << "could not approximate: " << ex << std::endl;
    return 1;
  }

  std::cout << "Approximation: " << approximated << std::endl;
  return 0;
}
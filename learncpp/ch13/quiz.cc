#include <iostream>

template <typename A, typename B>
class Pair {
 private:
  A a_;
  B b_;

 public:
  Pair(const A& a, const B& b) : a_(a), b_(b) {}

  A& first() { return a_; }
  const A& first() const { return a_; }

  B& second() { return b_; }
  const B& second() const { return b_; }
};

template <typename T>
class StringValuePair : public Pair<std::string, T> {
 public:
  StringValuePair(const std::string& a, const T& t)
      : Pair<std::string, T>(a, t) {}
};

int main() {
  Pair<int, double> p1(5, 6.7);
  std::cout << "Pair: " << p1.first() << ' ' << p1.second() << '\n';

  const Pair<double, int> p2(2.3, 4);
  std::cout << "Pair: " << p2.first() << ' ' << p2.second() << '\n';

  StringValuePair<int> svp("Hello", 5);
  std::cout << "Pair: " << svp.first() << ' ' << svp.second() << '\n';

  return 0;
}
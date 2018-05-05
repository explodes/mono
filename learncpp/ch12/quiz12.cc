#include <iostream>
#include <vector>

class Shape {
 public:
  friend std::ostream &operator<<(std::ostream &out, const Shape &shape) {
    return shape.print(out);
  }

  virtual std::ostream &print(std::ostream &out) const = 0;

  virtual ~Shape() {}
};

class Point final {
 public:
  Point() : x_(0), y_(0), z_(0) {}
  Point(double x, double y, double z) : x_(x), y_(y), z_(z) {}

  const double getX() const { return x_; }
  const double getY() const { return y_; }
  const double getZ() const { return z_; }

  friend std::ostream &operator<<(std::ostream &out, const Point &pt) {
    return out << "Point(" << pt.x_ << ", " << pt.y_ << ", " << pt.z_ << ")";
  }

 private:
  const double x_, y_, z_;
};

class Circle final : public Shape {
 public:
  Circle() : Shape(), center_(0, 0, 0), radius_(0) {}
  Circle(const Point center, const double radius)
      : Shape(), center_(center), radius_(radius) {}

  const Point getCenter() const { return center_; }
  const double getRadius() const { return radius_; }

  std::ostream &print(std::ostream &out) const override {
    return out << "Circle(" << center_ << ", radius " << radius_ << ")";
  }

 private:
  const Point center_;
  const double radius_;
};

class Triangle final : public Shape {
 public:
  Triangle() : Shape(), a_(0, 0, 0), b_(0, 0, 0), c_(0, 0, 0) {}
  Triangle(const Point a, const Point b, const Point c)
      : Shape(), a_(a), b_(b), c_(c) {}

  const Point getA() const { return a_; }
  const Point getB() const { return b_; }
  const Point getC() const { return c_; }

  std::ostream &print(std::ostream &out) const override {
    return out << "Triangle(" << a_ << ", " << b_ << ", " << c_ << ")";
  }

 private:
  const Point a_, b_, c_;
};

Circle *getLargestRadius(const std::vector<Shape *> &v) {
  Circle *largest;
  for (const auto &shape : v) {
    Circle *candidate = dynamic_cast<Circle *>(shape);
    if (!candidate) {
      continue;
    }
    if (largest == nullptr || candidate->getRadius() > largest->getRadius()) {
      largest = candidate;
    }
  }
  return largest;
}

int main() {
  std::vector<Shape *> v;
  //   v.push_back(new Circle(Point(1, 2, 3), 7));
  //   v.push_back(new Triangle(Point(1, 2, 3), Point(4, 5, 6), Point(7, 8,
  //   9))); 
  //v.push_back(new Circle(Point(4, 5, 6), 3));

  // print each shape in vector v on its own line here
  for (const auto &shape : v) {
    std::cout << *shape << std::endl;
  }

  Circle *largest = getLargestRadius(v);
  if (largest) {
    std::cout << "The largest radius is: " << *largest
              << '\n';  // write this function
  } else {
    std::cout << "No largest radius found.\n";
  }

  // delete each element in the vector here
  for (const auto &shape : v) {
    delete shape;
  }
}
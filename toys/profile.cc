#include <iostream>
#include "hoist/init.h"
#include "hoist/input.h"
#include "hoist/statusor.h"

int main() {
  Hoist::Init();

  int age = Hoist::prompt<int>("What is your age? ").ValueOrDie();
  std::string name =
      Hoist::prompt<std::string>("What is your name? ").ValueOrDie();

  std::cout << name << " is " << age << " years old\n";

  return 0;
}
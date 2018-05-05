#include <iostream>
#include "hoist/hoist.h"

int main() {
  Hoist::Result<int> age = Hoist::prompt<int>("What is your age? ");
  Hoist::Result<std::string> name =
      Hoist::prompt<std::string>("What is your name? ");

  std::cout << name.value() << " is " << age.value() << " years old\n";

  return 0;
}
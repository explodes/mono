#include "array.h"

int main() {
  Array<int, 5> arr;
  arr[0] = 2;
  arr[1] = 3;
  arr[4] = 6;
  printArray(arr);

  Array<double, 3> arr2{5.4, 34.2, 8.90, 10.2};
  printArray(arr2);
}
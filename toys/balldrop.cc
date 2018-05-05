#include <iostream>
#include "hoist/hoist.h"

const double earthGravity(9.8);

void dropBall(double initialHeight) {
  int seconds{0};
  while (true) {
    double distanceFallen = earthGravity * seconds * seconds / 2;
    double newHeight = initialHeight - distanceFallen;
    if (newHeight <= 0) {
      break;
    }
    std::cout << "At " << seconds
              << " seconds, the ball is at height: " << newHeight << " meters"
              << std::endl;
    seconds++;
  }
  std::cout << "At " << seconds << " seconds, the ball is on the ground."
            << std::endl;
}

int main() {
  Hoist::Init();

  Hoist::Result<double> towerHeight;

  do {
    towerHeight = Hoist::prompt<double>(
        "Enter the initial height of the tower in meters:");
    if (!towerHeight.ok() || towerHeight.value() == 0) {
      std::cerr << "invalid drop height" << std::endl;
    }
  } while (!towerHeight.ok());

  dropBall(towerHeight.value());
  Hoist::waitForInput("Press enter to continue.");
  return 0;
}
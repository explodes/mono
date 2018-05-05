#include <iostream>
#include "hoist/hoist.h"

static constexpr int minimumNumber{0};
static constexpr int maximumNumber{500000};

int obtainGuess() {
  Hoist::Result<int> result;
  do {
    result = Hoist::prompt<int>("Enter your guess:");
    if (!result.ok() || result.value() < minimumNumber ||
        result.value() > maximumNumber) {
      std::cerr << "invalid guess, try again: ";
    }
  } while (!result.ok());
  return result.value();
}

void playGame() {
  int number = Hoist::RNG::rand(minimumNumber, maximumNumber + 1);
  int numGuesses{0};

  std::cout << "I'm thinking of a number between " << minimumNumber << " and "
            << maximumNumber << "..." << std::endl;

  while (true) {
    int guess = obtainGuess();
    numGuesses++;
    if (guess < number) {
      std::cout << "Higher!" << std::endl;
    } else if (guess > number) {
      std::cout << "Lower!" << std::endl;
    } else {
      std::cout << "Correct! It took " << numGuesses
                << " to get the corrected answer." << std::endl;
      break;
    }
  }
}

bool playAgain() {
  Hoist::Result<std::string> result;
  std::string answer;
  while (true) {
    result = Hoist::prompt<std::string>("Would you like to play again (y/n)?");
    if (!result.ok()) {
      continue;
    }
    answer = result.value();
    if (answer.compare("y") == 0 || answer.compare("n") == 0) {
      break;
    }
  }
  return answer.compare("y") == 0;
}

int main() {
  Hoist::Init();
  do {
    playGame();
  } while (playAgain());
}
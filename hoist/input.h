#ifndef HOIST_INPUT_H
#define HOIST_INPUT_H

#include <iostream>
#include <limits>
#include <string>
#include "errors.h"

namespace Hoist {

static bool hasInput_(false);

inline void clearInput() {
  if (hasInput_) {
    std::cin.clear();
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
  }
}

template <typename T>
Result<T> prompt(const std::string &message) {
  std::cout << message << " ";
  T input;
  std::cin >> input;
  hasInput_ = true;

  if (std::cin.fail()) {
    clearInput();
    return Result<T>(StatusCode::Invalid);
  }
  return Result<T>(input);
}

template <typename T>
T mustPrompt(const std::string &message, const std::string &retryMessage) {
  Result<T> result;
  bool first{true};
  do {
    result = prompt<T>(first ? message : retryMessage);
    first = false;
  } while (!result.ok());
  return result.value();
}

template <>
inline Result<std::string> prompt<std::string>(const std::string &message) {
  std::cout << message << " ";
  std::string input;
  clearInput();
  std::getline(std::cin, input);
  hasInput_ = true;

  if (std::cin.fail()) {
    clearInput();
    return Result<std::string>(StatusCode::Invalid);
  }
  return Result<std::string>(input);
}

// waitForInput pauses execution until the user presses enter
void waitForInput(const std::string &message);

}  // namespace Hoist
#endif
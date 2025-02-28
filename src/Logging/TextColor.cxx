#include <Logging/TextColor.hxx>

#pragma once
#include <string>
namespace color {

std::string text(Color code) {
  switch (code) {
  case RESET:
    return "\033[0m";
  case RED:
    return "\033[31m";
  case GREEN:
    return "\033[32m";
  case YELLOW:
    return "\033[33m";
  case BLUE:
    return "\033[34m";
  case MAGENTA:
    return "\033[35m";
  case CYAN:
    return "\033[36m";
  case WHITE:
    return "\033[37m";
  default:
    return "\033[0m"; // Default to reset
  }
}

std::string reset() { return text(Color::RESET); }

}; // namespace color
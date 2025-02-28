#pragma once
#include <string>
namespace color {

enum Color { RED = 0, GREEN, BLUE, YELLOW, MAGENTA, CYAN, WHITE, RESET };

std::string text(Color code);
std::string reset();

} // namespace color
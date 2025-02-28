#pragma once
#include <functional> // for function
#include <memory>     // for shared_ptr, allocator, __shared_ptr_access
#include <string>     // for string, basic_string
#include <vector>     // for vector

#include "ftxui/component/component.hpp"
namespace ui {
namespace tools {
std::vector<std::string> get_last_n_lines(const std::string &text, size_t n);
std::vector<std::string> split_string_by_newline(const std::string &str);
ftxui::Component Wrap(std::string name, ftxui::Component component);
} // namespace tools
} // namespace ui
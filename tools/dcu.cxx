// Copyright 2020 Arthur Sonzogni. All rights reserved.
// Use of this source code is governed by the MIT license that can be found in
// the LICENSE file.
#include <functional>  // for function
#include <memory>      // for shared_ptr, allocator, __shared_ptr_access
#include <string>      // for string, basic_string
#include <vector>      // for vector
 
#include "ftxui/component/captured_mouse.hpp"  // for ftxui
#include "ftxui/component/component.hpp"  // for Slider, Checkbox, Vertical, Renderer, Button, Input, Menu, Radiobox, Toggle
#include "ftxui/component/component_base.hpp"  // for ComponentBase
#include "ftxui/component/screen_interactive.hpp"  // for Component, ScreenInteractive
#include "ftxui/dom/elements.hpp"  // for separator, operator|, Element, size, xflex, text, WIDTH, hbox, vbox, EQUAL, border, GREATER_THAN
 
#include "Logging/Logger.hxx"
#include "DimHandlers/DimManager.hxx"
#include "Configuration/XmlParser.hxx"

#include<UI/UIManager.hxx>

int main(int argc, const char** argv) {
  using namespace ftxui;
  Logger::Get().queit(true);
  config::XmlParser parser;
  if(!parser.load(argv[1])){
    return -1;
  }
  ui::Manager ui;
  ui.runUI();
  
  return 0;
}
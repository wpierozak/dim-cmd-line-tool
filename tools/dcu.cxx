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
using namespace ftxui;
 
// Display a component nicely with a title on the left.
Component Wrap(std::string name, Component component) {
  return Renderer(component, [name, component] {
    return hbox({
               text(name) | size(WIDTH, EQUAL, 8),
               separator(),
               component->Render() | xflex,
           }) |
           xflex;
  });
}

int main(int argc, const char** argv) {
  Logger::Get().queit(true);
  config::XmlParser parser;
  if(!parser.load(argv[1])){
    return -1;
  }

  auto screen = ScreenInteractive::FitComponent();
 
  // -- Menu
  // ----------------------------------------------------------------------
  std::vector<std::string> main_menu_entries = {
      "Send command",
      "Print latest data"
  };
  int main_menu_selected = 0;
  auto main_menu = Menu(&main_menu_entries, &main_menu_selected);
  //command_menu = Wrap("Menu", command_menu);

  std::vector<std::string> service_menu_entries = {
    "COMMAND A",
    "COMMAND B"
  };
  int service_menu_selected = 0;
  auto service_menu = Menu(&service_menu_entries, &service_menu_selected);

  std::vector<std::string> command_menu_entries = {
    
    };
  int command_menu_selected = 0;
  auto command_menu = Menu(&command_menu_entries, &command_menu_selected);

  // -- Input ------------------------------------------------------------------
  std::string input_label;
  auto input = Input(&input_label, "placeholder");
  input = Wrap("Input", input);
  
  // -- Button -----------------------------------------------------------------
  std::string button_label = "Quit";
  std::function<void()> on_button_clicked_;
  auto button = Button(&button_label, screen.ExitLoopClosure());
  //button = Wrap("Button", button);
 
  // Layout
  // -----------------------------------------------------------------
  auto layout = Container::Vertical({
        Container::Horizontal({
      main_menu,
      service_menu,
      command_menu
    }),
    input
});
 
  auto component = Renderer(layout, [&] {
    
    service_menu_entries.clear();
    command_menu_entries.clear();

    std::list<std::string> services;

    if(main_menu_selected == 0){
        services = DIM_MANAGER.getCommandSenders();
    } else{
        services = DIM_MANAGER.getSubscribers();
    }

    for(auto& service: services){
        service_menu_entries.push_back(service);
    }

    if(service_menu_selected == 0){
       auto commands = DIM_MANAGER.getCommands(service_menu_entries[service_menu_selected]);
       for(auto& cmd: commands){
        command_menu_entries.push_back(cmd);
       }
    } 
    
    return vbox( {
        text("DIM command-line tool") | border,
        hbox({
               main_menu->Render(),
               separator(),
               service_menu->Render(),
               separator(),
               command_menu->Render()
           }) | border,
        input->Render() | size(WIDTH,GREATER_THAN,100) | border
        
        });
        
  });
 
  screen.Loop(component);
 
  return 0;
}
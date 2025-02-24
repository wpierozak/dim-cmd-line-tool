// filepath: /home/wiktor/dim-cmd-line-tool/inc/UI/UI.hxx
#pragma once
#include <thread>
#include <atomic>
#include <chrono>
#include <functional>

 
#include "ftxui/component/captured_mouse.hpp"  // for ftxui
#include "ftxui/component/component.hpp"  // for Slider, Checkbox, Vertical, Renderer, Button, Input, Menu, Radiobox, Toggle
#include "ftxui/component/component_base.hpp"  // for ComponentBase
#include "ftxui/component/screen_interactive.hpp"  // for Component, ScreenInteractive
#include "ftxui/dom/elements.hpp"  // for separator, operator|, Element, size, xflex, text, WIDTH, hbox, vbox, EQUAL, border, GREATER_THAN

class UI
{
private:
void handleKeyboardEvent(char c)
{
    
}
struct CommandOutput
{
    std::vector<std::string> lines;
    uint32_t currentPosition;
} m_curretnCommandOutput;

};
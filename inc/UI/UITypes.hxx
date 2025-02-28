#pragma once
#include <DimHandlers/DimManager.hxx>
#include <functional> // for function
#include <memory>     // for shared_ptr, allocator, __shared_ptr_access
#include <string>     // for string, basic_string
#include <vector>     // for vector

#include "UINotification.hxx"
#include "UITools.hxx"
#include "ftxui/component/captured_mouse.hpp" // for ftxui
#include "ftxui/component/component.hpp" // for Slider, Checkbox, Vertical, Renderer, Button, Input, Menu, Radiobox, Toggle
#include "ftxui/component/component_base.hpp" // for ComponentBase
#include "ftxui/component/screen_interactive.hpp" // for Component, ScreenInteractive
#include "ftxui/dom/elements.hpp" // for separator, operator|, Element, size, xflex, text, WIDTH, hbox, vbox, EQUAL, border, GREATER_THAN

namespace ui {
namespace types {
class Root : public notify::Publisher {
public:
  Root() : Node("ROOT"), Publisher("ROOT") {}
  void evaluateState() override {
    m_counter++;
    updateState(std::optional<std::string>(std::to_string(m_counter)));
  }

private:
  uint64_t m_counter{0};
};

class Menu : public notify::Publisher, public notify::Subscriber {
public:
  Menu(const std::string &ID,
       std::function<std::vector<std::string>(const std::string &)>
           entriesSources_)
      : Node(ID), Publisher(ID), Subscriber(ID),
        m_entriesSource(entriesSources_) {
    m_component = ftxui::Menu(&m_entries, &m_selected);
  }

  void evaluateState() override;
  void notify(std::string publisher,
              std::optional<std::string> context) override;
  ftxui::Component &component() { return m_component; }

  int selected() const { return m_selected; }
  std::string option() const { return m_entries[m_selected]; }

private:
  void updateEntries(std::string context);

  std::vector<std::string> m_entries;
  int m_selected;

  std::function<std::vector<std::string>(const std::string &)> m_entriesSource;
  ftxui::Component m_component;
};

struct Input {
  Input() { component = tools::Wrap("Input", ftxui::Input(&content, "")); }
  std::string content;
  ftxui::Component component;
};

class MultiLineText {
public:
  MultiLineText(const std::string &text, size_t lines = std::string::npos);
  ftxui::Element Render();

private:
  std::vector<std::string> m_lines;
};

class Command {
public:
  enum class State { Active, Ready, Waiting, Finished, Failure };
  enum class Type { Known, Input };

  Command(const std::string &cmdSender, Type type_)
      : type(type_), m_state(State::Ready), m_commandSender(cmdSender) {}

  State state() {
    std::lock_guard lock(m_mutex);
    return m_state;
  }

  void moveReady(const std::string &command);
  void moveWaiting();
  void moveFinished();

  std::optional<std::string> response() { return m_response; }

  std::optional<std::string> error() { return m_errorMessage; }

  const Type type;

private:
  std::mutex m_mutex;
  State m_state;
  std::string m_commandSender;
  std::string m_command;

  std::optional<std::string> m_response;
  std::optional<std::string> m_errorMessage;
};
} // namespace types
} // namespace ui
#pragma once
#include <DimHandlers/DimManager.hxx>
#include <functional> // for function
#include <memory>     // for shared_ptr, allocator, __shared_ptr_access
#include <string>     // for string, basic_string
#include <vector>     // for vector

#include "UITools.hxx"
#include "ftxui/component/captured_mouse.hpp" // for ftxui
#include "ftxui/component/component.hpp" // for Slider, Checkbox, Vertical, Renderer, Button, Input, Menu, Radiobox, Toggle
#include "ftxui/component/component_base.hpp" // for ComponentBase
#include "ftxui/component/screen_interactive.hpp" // for Component, ScreenInteractive
#include "ftxui/dom/elements.hpp" // for separator, operator|, Element, size, xflex, text, WIDTH, hbox, vbox, EQUAL, border, GREATER_THAN
#include <Notify/Notification.hxx>

namespace ui {
namespace types {
class Root : public notify::Publisher {
public:
  Root() : Node(0x0), Publisher(0x0) {}
  void evaluateState() override {
    m_counter++;
    updateState(std::optional<std::string>(std::to_string(m_counter)));
  }

private:
  uint64_t m_counter{0};
};

class Menu {
public:
  Menu() { m_component = ftxui::Menu(&m_entries, &m_selected); }

  ftxui::Component &component() { return m_component; }

  bool isNull() const { return m_selected == -1; }
  int selected() const { return m_selected; }
  const std::string &option() const { return m_entries[m_selected]; }
  opt_str nullableOption() const {
    return (m_selected != -1) ? opt_str(m_entries[m_selected]) : std::nullopt;
  }

protected:
  std::vector<std::string> m_entries;
  int m_selected;
  ftxui::Component m_component;
};

struct Input {
  Input() { component = tools::Wrap("Input", ftxui::Input(&content, "")); }
  std::string content;
  ftxui::Component component;
};

class MultiLineText {
public:
  MultiLineText() = default;
  MultiLineText(const std::string &text, size_t lines = std::string::npos);
  void operator()(const std::string &text, size_t lines = std::string::npos);
  ftxui::Element Render();
  void firstLine(size_t first) { m_first = first; }
  void lineNumber(size_t lineNumber) { m_last = lineNumber; }

private:
  std::vector<std::string> m_lines;
  size_t m_first{0};
  size_t m_last{10};
};

class MessageBox : public notify::Subscriber {
public:
  MessageBox(uint64_t ID) : Node(ID), Subscriber(ID) {}

  void notify(uint64_t publisher) override;
  void evaluateState();
  ftxui::Element Render();
  void moveTextLineUp() {
    m_first++;
    m_content.firstLine(m_first);
  }

  void moveTextLineDown() {
    if (m_first > 0) {
      m_first--;
    }
    m_content.firstLine(m_first);
  }

  void increasePrintedLinesNum(){
    m_lineNumber++;
    m_content.lineNumber(m_lineNumber);
  }

  void decreasePrintedLineNum(){
    if(m_lineNumber > 5){
      m_lineNumber--;
    }
    m_content.lineNumber(m_lineNumber);
  }

private:
  void printLogs();
  void printLatestData();
  void printCommand();

  enum class Mode { Logs, LatestData, Command } m_mode;

  MultiLineText m_content;
  uint32_t m_currentServiceData;
  std::string m_currentService;
  uint64_t m_serviceState;

  uint32_t m_first{0};
  uint32_t m_lineNumber{10};
};

class Command : public notify::Subscriber, public notify::Publisher {
public:
  enum class State { Invalid, Active, Ready, Waiting, Finished, Failure };

  static std::string stateToString(State state) {
    switch (state) {
    case State::Invalid:
      return "Invalid";
    case State::Active:
      return "Active";
    case State::Ready:
      return "Ready";
    case State::Waiting:
      return "Waiting";
    case State::Finished:
      return "Finished";
    case State::Failure:
      return "Failure";
    default:
      return "Unknown";
    }
  }
  enum class Type { Known, Input };

  Command(uint64_t ID) : Node(ID), Subscriber(ID), Publisher(ID) {}
  void notify(uint64_t publisher) override;

  State state() { return m_state; }

  void executeAndWait();
  void execute();
  void setCmd(const std::string &cmd) {
    m_command = cmd;
    m_state = State::Ready;
  }

  std::optional<std::string> response() { return m_response; }
  std::optional<std::string> error() { return m_errorMessage; }

private:
  State m_state;
  Type m_type;
  std::string m_commandSender;
  std::string m_command;

  std::optional<std::string> m_response;
  std::optional<std::string> m_errorMessage;
};
} // namespace types
} // namespace ui
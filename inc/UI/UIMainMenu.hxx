#pragma once
#include "UITypes.hxx"
#include <Notify/Notification.hxx>

namespace ui {
namespace menu {
class MainMenu : public notify::Publisher,
                 public notify::Subscriber,
                 public types::Menu {
public:
  MainMenu() : Node(ID), Publisher(ID), Subscriber(ID) {
    m_entries = {SEND_COMMAND.data(), SEND_COMMAND_WAIT.data(),
                 PRINT_LATEST_DATA.data(), LOGS.data()};
  }
  void evaluateState() override { updateState(m_entries[m_selected]); }
  void notify(uint64_t publisher) override { evaluateState(); }

  static constexpr uint64_t ID = 0x1;
  static constexpr std::string_view SEND_COMMAND = "Send command";
  static constexpr std::string_view SEND_COMMAND_WAIT =
      "Send command and wait for data";
  static constexpr std::string_view PRINT_LATEST_DATA = "Print latest data";
  static constexpr std::string_view LOGS = "Print logs";
  static constexpr std::string_view BROWSE_BUFFERED_DATA =
      "Browse buffered data";
};
} // namespace menu
} // namespace ui
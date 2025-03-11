#pragma once
#include "UITypes.hxx"
#include <Notify/Notification.hxx>

namespace ui {
namespace menu {
class CommandMenu : public notify::Publisher,
                    public notify::Subscriber,
                    public types::Menu {
public:
  CommandMenu() : Node(ID), Publisher(ID), Subscriber(ID) {}
  void evaluateState() override {
    if (m_entries.empty()) {
      m_selected = -1;
      updateState(std::nullopt);
    } else {
      updateState(m_entries[m_selected]);
    }
  }
  void notify(uint64_t publisher) override;

  static constexpr uint64_t ID = 0x3;
  static constexpr std::string_view SEND_CMD_INPUT = "Send command from input";

private:
  void updateEntries();
};
} // namespace menu
} // namespace ui

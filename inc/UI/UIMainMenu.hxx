#pragma once
#include"UINotification.hxx"
#include"UITypes.hxx"

namespace ui
{
namespace menu
{
class MainMenu: public notify::Publisher, public notify::Subscriber, public types::Menu
{
public:
    MainMenu(): Node(ID.data()), Publisher(ID.data()), Subscriber(ID.data())
    {
        m_entries = {SEND_COMMAND.data(), SEND_COMMAND_WAIT.data(), PRINT_LATEST_DATA.data(), LOGS.data()};
    }
    void evaluateState() override
    {
        updateState(m_entries[m_selected]);
    }
    void notify(const std::string& publisher, opt_str_ref context) override
    {
        evaluateState();
    }

    static constexpr std::string_view ID = "MAIN_MENU";
    static constexpr std::string_view SEND_COMMAND = "Send command";
    static constexpr std::string_view SEND_COMMAND_WAIT = "Send command and wait for data";
    static constexpr std::string_view PRINT_LATEST_DATA = "Print latest data";
    static constexpr std::string_view LOGS = "Print logs";
    static constexpr std::string_view BROWSE_BUFFERED_DATA = "Browse buffered data";
};
}
}
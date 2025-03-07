#pragma once
#include"UINotification.hxx"
#include"UITypes.hxx"

namespace ui
{
namespace menu
{
class CommandMenu: public notify::Publisher, public notify::Subscriber, public types::Menu
{
    public:
    CommandMenu(): Node(ID.data()), Publisher(ID.data()), Subscriber(ID.data())
    {
    }
    void evaluateState() override
    {
        if(m_entries.empty()){
            m_selected = -1;
            updateState(std::nullopt);
        } else{
            updateState(m_entries[m_selected]);
        }

    }
    void notify(const std::string& publisher, opt_str_ref context) override
    {
        updateEntries(context);
        evaluateState();
    }

    static constexpr std::string_view ID = "COMMAND_MENU";
    static constexpr std::string_view SEND_CMD_INPUT = "Send command from input";
private:
    void updateEntries(opt_str_ref context);
};
}
} // namespace ui

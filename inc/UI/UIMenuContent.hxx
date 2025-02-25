#pragma once
#include<string_view>

namespace ui
{
namespace menu
{
namespace main
{
    constexpr std::string_view SEND_COMMAND = "Send command";
    constexpr std::string_view SEND_COMMAND_WAIT = "Send command and wait for data";
    constexpr std::string_view PRINT_LATEST_DATA = "Print latest data";
    constexpr std::string_view LOGS = "Print logs";
}
namespace commands
{
    constexpr std::string_view SEND_CMD_INPUT = "Send command from input";
}
}
}
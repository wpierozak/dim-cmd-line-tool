#include <UI/UIMenuContent.hxx>
#include <UI/UIObjects.hxx>
namespace ui {
namespace objects {
std::shared_ptr<ui::types::Menu> mainMenu =
    std::make_shared<ui::types::Menu>("MAIN_MENU", getMainMenuEntries);
std::shared_ptr<ui::types::Menu> serviceMenu =
    std::make_shared<ui::types::Menu>("SERVICE_MENU", getServiceMenuEntries);
std::shared_ptr<ui::types::Menu> commandsMenu =
    std::make_shared<ui::types::Menu>("COMMANDS_MENU", getCommandsMenuEntries);

ui::types::Input input = ui::types::Input();

std::vector<std::string> getMainMenuEntries(const std::string &context) {
  return {ui::menu::main::SEND_COMMAND.data(),
          ui::menu::main::SEND_COMMAND_WAIT.data(),
          ui::menu::main::PRINT_LATEST_DATA.data(),
          ui::menu::main::LOGS.data()};
}

std::optional<ui::types::Command> command = std::nullopt;

std::vector<std::string> getServiceMenuEntries(const std::string &context) {
  
  std::vector<std::string> entries;
  std::list<std::string> source;
  if (context == ui::menu::main::SEND_COMMAND ||
      context == ui::menu::main::SEND_COMMAND_WAIT) {
    source = DIM_MANAGER.getCommandSenders();
  } else if (context == ui::menu::main::PRINT_LATEST_DATA) {
    source = DIM_MANAGER.getSubscribers();
  }
  if (source.empty()) {
    return {};
  }
  std::transform(source.begin(), source.end(), std::back_inserter(entries),
                 [](const std::string &entry) { return entry; });
  return entries;
}

std::vector<std::string> getCommandsMenuEntries(const std::string &context) {
  std::vector<std::string> entries;
  auto source = DIM_MANAGER.getCommands(context);
  if(source.empty()){
    {};
  }
  std::transform(source.begin(), source.end(), std::back_inserter(entries),
                 [](const std::string &entry) { return entry; });
  entries.push_back(menu::commands::SEND_CMD_INPUT.data());
  return entries;
}

} // namespace objects

} // namespace ui
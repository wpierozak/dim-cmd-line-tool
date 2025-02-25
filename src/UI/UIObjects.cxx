#include<UI/UIObjects.hxx>
#include<UI/UIMenuContent.hxx>
namespace ui
{
namespace objects
{
ui::types::Menu mainMenu = ui::types::Menu();
ui::types::Menu serviceMenu = ui::types::Menu();
ui::types::Menu commandsMenu = ui::types::Menu();

ui::types::Input input = ui::types::Input();

std::optional<ui::types::Command> command = std::nullopt;

void updateMainMenu()
{
    mainMenu.entries.clear();
    mainMenu.entries.push_back(ui::menu::main::SEND_COMMAND.data());
    mainMenu.entries.push_back(ui::menu::main::SEND_COMMAND_WAIT.data());
    mainMenu.entries.push_back(ui::menu::main::PRINT_LATEST_DATA.data());
    mainMenu.entries.push_back(ui::menu::main::LOGS.data());
}

void updateServiceMenu()
{
    const auto& mainMenuOption = mainMenu.entries[mainMenu.selected];
    serviceMenu.entries.clear();
    std::list<std::string> entries;
    if(mainMenuOption == ui::menu::main::SEND_COMMAND || mainMenuOption == ui::menu::main::SEND_COMMAND_WAIT){
      entries = DIM_MANAGER.getCommandSenders();
    } else{
     entries = DIM_MANAGER.getSubscribers();
    }
    std::transform(entries.begin(), entries.end(), std::back_inserter(serviceMenu.entries), [](const std::string& entry){return entry;});
}

void updateCommandsMenu()
{
    const auto& mainMenuOption = mainMenu.entries[mainMenu.selected];
    auto& commandMenuContent = commandsMenu.entries;
    commandMenuContent.clear();
    if(serviceMenu.entries.size() > 0){
      const auto& serviceMenuOption = serviceMenu.entries[serviceMenu.selected];
      if(mainMenuOption == ui::menu::main::SEND_COMMAND || mainMenuOption == ui::menu::main::SEND_COMMAND_WAIT){
        auto commands = DIM_MANAGER.getCommands(serviceMenuOption);
        std::transform(commands.begin(), commands.end(), 
          std::back_inserter(commandMenuContent),[&](const std::string& command){return command;});
        commandMenuContent.push_back(ui::menu::commands::SEND_CMD_INPUT.data());
      }
    }   
}

}


}
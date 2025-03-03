#include "UITypes.hxx"

namespace ui {
namespace objects {
extern std::shared_ptr<ui::types::Menu> mainMenu;
std::vector<std::string> getMainMenuEntries(const std::string &context);
void updateMainMenu();

extern std::shared_ptr<ui::types::Menu> serviceMenu;
std::vector<std::string> getServiceMenuEntries(const std::string &context);
void updateServiceMenu();

extern std::shared_ptr<ui::types::Menu> commandsMenu;
std::vector<std::string> getCommandsMenuEntries(const std::string &context);
void updateCommandsMenu();

extern std::shared_ptr<ui::types::MessageBox> messageBox;

extern ui::types::Input input;

extern std::shared_ptr<ui::types::Command> command;
} // namespace objects
} // namespace ui
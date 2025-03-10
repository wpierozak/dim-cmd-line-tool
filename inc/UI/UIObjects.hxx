#include "UICommandMenu.hxx"
#include "UIMainMenu.hxx"
#include "UIServiceMenu.hxx"
#include "UITypes.hxx"

namespace ui {
namespace objects {
extern std::shared_ptr<ui::menu::MainMenu> mainMenu;
extern std::shared_ptr<ui::menu::ServiceMenu> serviceMenu;
extern std::shared_ptr<ui::menu::CommandMenu> commandsMenu;

extern std::shared_ptr<ui::types::MessageBox> messageBox;

extern ui::types::Input input;

extern std::shared_ptr<ui::types::Command> command;
} // namespace objects
} // namespace ui
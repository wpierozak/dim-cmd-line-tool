#include"UITypes.hxx"

namespace ui
{
namespace objects
{
extern ui::types::Menu mainMenu;
void updateMainMenu();

extern ui::types::Menu serviceMenu;
void updateServiceMenu();

extern ui::types::Menu commandsMenu;
void updateCommandsMenu();

extern ui::types::Input input;


extern std::optional<ui::types::Command> command; 
}
}
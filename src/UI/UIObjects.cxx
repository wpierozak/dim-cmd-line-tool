#include <UI/UIMenuContent.hxx>
#include <UI/UIObjects.hxx>
namespace ui {
namespace objects {
std::shared_ptr<ui::menu::MainMenu> mainMenu =
    std::make_shared<ui::menu::MainMenu>();
std::shared_ptr<ui::menu::ServiceMenu> serviceMenu =
    std::make_shared<ui::menu::ServiceMenu>();
std::shared_ptr<ui::menu::CommandMenu> commandsMenu =
    std::make_shared<ui::menu::CommandMenu>();

std::shared_ptr<ui::types::MessageBox> messageBox =
    std::make_shared<ui::types::MessageBox>(0x4);

ui::types::Input input = ui::types::Input();

std::shared_ptr<ui::types::Command> command =
    std::make_shared<types::Command>(0x5);
} // namespace objects

} // namespace ui
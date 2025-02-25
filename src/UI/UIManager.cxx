#include<UI/UIManager.hxx>
#include<UI/UIMenuContent.hxx>
namespace ui
{
void Manager::runUI()
{
    auto screen = ftxui::ScreenInteractive::FitComponent();
    auto layout = 
    ftxui::Container::Vertical({
        ftxui::Container::Horizontal({
        ui::objects::mainMenu.component,
        ui::objects::serviceMenu.component,
        ui::objects::commandsMenu.component
      }),
      ui::objects::input.component
    });

  auto component = ftxui::Renderer(layout, 
  [&] {
    ui::objects::updateMainMenu();
    ui::objects::updateServiceMenu();
    ui::objects::updateCommandsMenu();

    auto output = getOutput();
    
    return
      ftxui::vbox( {
        ftxui::text("DIM command-line tool") | ftxui::border,
        ftxui::hbox({
                ui::objects::mainMenu.component->Render(),
               ftxui::separator(),
               ui::objects::serviceMenu.component->Render(),
               ftxui::separator(),
               ui::objects::commandsMenu.component->Render()
           }) | ftxui::border,
           ui::objects::input.component->Render() | ftxui::size(ftxui::WIDTH,ftxui::EQUAL,100) | ftxui::border,
           output.Render() | ftxui::border
        
        });
      }
    );
 
    std::atomic<bool> refresh_ui_continue = true;
    std::thread refresh_ui([&] {
      while (refresh_ui_continue) {
        using namespace std::chrono_literals;
        std::this_thread::sleep_for(0.1s);
        
        // After updating the state, request a new frame to be drawn. This is done
        // by simulating a new "custom" event to be handled.
        screen.Post(ftxui::Event::Custom);
      }
    });
   
  screen.Loop(component);
  refresh_ui_continue = false;
  refresh_ui.join();
}

ui::types::MultiLineText Manager::getOutput()
{
    using namespace ui::objects;
    if(mainMenu.entries[mainMenu.selected] == ui::menu::main::PRINT_LATEST_DATA){
        if(serviceMenu.entries.size() > 0){
            ui::types::MultiLineText text(getServiceInfo(serviceMenu.entries[serviceMenu.selected]));
            return text;
        } else return ui::types::MultiLineText("");
    }
    return ui::types::MultiLineText("");
}

std::string Manager::getServiceInfo(const std::string& name)
{
    auto res = DIM_MANAGER.getServiceData(name,true);
    if(res.isError() == false){
        return res.result.value();
    } 
    return res.error.value_or("");
}

std::string Manager::getLogs()
{

}
}
#include<UI/UIManager.hxx>
#include<UI/UIMenuContent.hxx>
namespace ui
{
void Manager::runUI()
{
    auto button = ftxui::Button("Enter",[&]{enterClicked();});
    auto screen = ftxui::ScreenInteractive::FitComponent();
    auto layout = 
    ftxui::Container::Vertical({
        ftxui::Container::Horizontal({
        ui::objects::mainMenu.component,
        ui::objects::serviceMenu.component,
        ui::objects::commandsMenu.component
      }),
      ftxui::Container::Horizontal({
        ui::objects::input.component,
        button
      })
    });

  auto component = ftxui::Renderer(layout, 
  [&] {
    ui::objects::updateMainMenu();
    if(ui::objects::mainMenu.selected != m_lastMainSelected){
        ui::objects::updateServiceMenu();
    }
    if(ui::objects::mainMenu.selected != m_lastMainSelected || 
        ui::objects::serviceMenu.selected != m_lastServiceSelected){
        ui::objects::updateCommandsMenu();
        updateCommand();
    }
    auto output = getOutput();
    

    m_lastMainSelected = ui::objects::mainMenu.selected;
    m_lastServiceSelected = ui::objects::serviceMenu.selected;
    m_lastCommandSelected = ui::objects::commandsMenu.selected;

    return
      ftxui::vbox( {
        ftxui::hbox({
                ui::objects::mainMenu.component->Render(),
               ftxui::separator(),
               ui::objects::serviceMenu.component->Render(),
               ftxui::separator(),
               ui::objects::commandsMenu.component->Render()
           }) | ftxui::border,
           ftxui::hbox({
            ui::objects::input.component->Render(),
            ftxui::separator(),
            button->Render()
            }) | ftxui::size(ftxui::WIDTH,ftxui::EQUAL,100) | ftxui::border,
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
    } else if(mainMenu.entries[mainMenu.selected] == ui::menu::main::LOGS){
        ui::types::MultiLineText text(getLogs(), 3);
        return text;
    }
    
    if(ui::objects::command.has_value()){
        if(ui::objects::command->state() == ui::types::Command::State::Finished){
            return ui::types::MultiLineText(command->response().value_or("Executed"));
        } else if(ui::objects::command->state() == ui::types::Command::State::Waiting){
            return ui::types::MultiLineText("Waiting...");
        } else if(ui::objects::command->state() == ui::types::Command::State::Failure){
            return ui::types::MultiLineText("Failure!");
        }
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
    return Logger::Get().getQuietLogs();
}

void Manager::updateCommand()
{
    using namespace ui::objects;
    const auto& mainMenuOption = mainMenu.entries[mainMenu.selected];
    if(mainMenuOption != ui::menu::main::SEND_COMMAND && mainMenuOption != ui::menu::main::SEND_COMMAND_WAIT){
        command = std::nullopt;
        return;
    }
    if(serviceMenu.entries.size() == 0){
        return;
    }

    const auto& serviceMenuOption =  serviceMenu.entries[serviceMenu.selected];
    auto type = (mainMenuOption == ui::menu::main::SEND_COMMAND) ? ui::types::Command::Type::Input : ui::types::Command::Type::Known;
    command.emplace(serviceMenuOption, type);
    
    if(commandsMenu.entries.size() == 0){
        return;
    }
    if(commandsMenu.entries[commandsMenu.selected] == ui::menu::commands::SEND_CMD_INPUT){
        return;
    }
    command->moveReady(commandsMenu.entries[commandsMenu.selected]);
}

void Manager::enterClicked()
{
    using namespace ui::objects;
    if(command == std::nullopt){
        return;
    }
    const auto& mainMenuOption = mainMenu.entries[mainMenu.selected];

    if(mainMenuOption == ui::menu::main::SEND_COMMAND){
        if(commandsMenu.entries[commandsMenu.selected] != ui::menu::commands::SEND_CMD_INPUT){
            command->moveFinished();
        } else {
            command->moveReady(input.content);
            command->moveFinished();
        }
    } else if(mainMenuOption == ui::menu::main::SEND_COMMAND_WAIT){
        if(commandsMenu.entries[commandsMenu.selected] != ui::menu::commands::SEND_CMD_INPUT){
            command->moveWaiting();
        }else{
            command->moveReady(input.content);
            command->moveWaiting();
        }
    }
}

}
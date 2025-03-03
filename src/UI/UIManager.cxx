#include <UI/UIManager.hxx>
#include <UI/UIMenuContent.hxx>
namespace ui {
void Manager::runUI() {
  ui::types::Root root;
  root.subscribe(ui::objects::mainMenu);
  root.subscribe(ui::objects::messageBox);

  ui::objects::mainMenu->subscribe(ui::objects::serviceMenu);
  ui::objects::mainMenu->subscribe(ui::objects::command);
  ui::objects::serviceMenu->subscribe(ui::objects::commandsMenu);
  ui::objects::serviceMenu->subscribe(ui::objects::command);
  ui::objects::commandsMenu->subscribe(ui::objects::command);
  
  // ui::objects::mainMenu->updateEntries();
  auto button = ftxui::Button("Enter", [&] { enterClicked(); });
  auto screen = ftxui::ScreenInteractive::FitComponent();
  auto layout = ftxui::Container::Vertical(
      {ftxui::Container::Horizontal({ui::objects::mainMenu->component(),
                                     ui::objects::serviceMenu->component(),
                                     ui::objects::commandsMenu->component()}),
       ftxui::Container::Horizontal({ui::objects::input.component, button})});

  auto component = ftxui::Renderer(layout, [&] {
    root.evaluateState();
    ui::objects::serviceMenu->evaluateState();
    ui::objects::commandsMenu->evaluateState();

    return ftxui::vbox(
        {ftxui::hbox({ui::objects::mainMenu->component()->Render(),
                      ftxui::separator(),
                      ui::objects::serviceMenu->component()->Render(),
                      ftxui::separator(),
                      ui::objects::commandsMenu->component()->Render()}) |
             ftxui::border,
         ftxui::hbox({ui::objects::input.component->Render(),
                      ftxui::separator(), button->Render()}) |
             ftxui::size(ftxui::WIDTH, ftxui::EQUAL, 100) | ftxui::border,
         ui::objects::messageBox->Render() | ftxui::border

        });
  });

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
  try{
  screen.Loop(component);
  }
  catch(std::exception& e)
  {
   std::cout << e.what(); 
  }
  refresh_ui_continue = false;
  refresh_ui.join();
}


std::string Manager::getServiceInfo(const std::string &name) {
  auto res = DIM_MANAGER.getServiceData(name, true);
  if (res.isError() == false) {
    return res.result.value();
  }
  return res.error.value_or("");
}

std::string Manager::getLogs() { return Logger::Get().getQuietLogs(); }

void Manager::enterClicked() 
{
  auto cmdOption = ui::objects::commandsMenu->nullableOption();
  if(cmdOption == std::nullopt){
    return;
  } else if(cmdOption == ui::menu::commands::SEND_CMD_INPUT){
    ui::objects::command->setCmd(ui::objects::input.content);
  }
  if(ui::objects::mainMenu->option() == ui::menu::main::SEND_COMMAND_WAIT){
    ui::objects::command->executeAndWait();
  } else if(ui::objects::mainMenu->option() == ui::menu::main::SEND_COMMAND){
    ui::objects::command->execute();

  }
}

} // namespace ui
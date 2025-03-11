#include <UI/UIMainMenu.hxx>
#include <UI/UIObjects.hxx>
#include <UI/UIServiceMenu.hxx>
namespace ui {
namespace menu {

void ServiceMenu::notify(uint64_t publisher) {
  if (publisher == ui::objects::mainMenu->identity()) {
    updateEntries();
  }
  evaluateState();
}

void ServiceMenu::updateEntries() {
  m_entries.clear();
  auto &context = objects::mainMenu->option();
  std::list<std::string> source;

  if (context == ui::menu::MainMenu::SEND_COMMAND ||
      context == ui::menu::MainMenu::SEND_COMMAND_WAIT) {
    source = DIM_MANAGER.getCommandSenders();
  } else if (context == ui::menu::MainMenu::PRINT_LATEST_DATA) {
    source = DIM_MANAGER.getSubscribers();
  }

  if (source.empty()) {
    return;
  }

  m_entries.insert(m_entries.end(), std::make_move_iterator(source.begin()),
                   std::make_move_iterator(source.end()));
}
} // namespace menu
} // namespace ui
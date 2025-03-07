#include <UI/UIMainMenu.hxx>
#include <UI/UIServiceMenu.hxx>
namespace ui {
namespace menu {
void ServiceMenu::updateEntries(opt_str_ref contextOpt) {
  m_entries.clear();
  auto context = *contextOpt;

  std::list<std::string> source;
  if (context.get() == ui::menu::MainMenu::SEND_COMMAND ||
      context.get() == ui::menu::MainMenu::SEND_COMMAND_WAIT) {
    source = DIM_MANAGER.getCommandSenders();
  } else if (context.get() == ui::menu::MainMenu::PRINT_LATEST_DATA) {
    source = DIM_MANAGER.getSubscribers();
  }

  if (source.empty()) {
    return;
  }
  std::transform(source.begin(), source.end(), std::back_inserter(m_entries),
                 [](const std::string &entry) { return entry; });
}
} // namespace menu
} // namespace ui
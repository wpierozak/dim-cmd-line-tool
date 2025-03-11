#include <UI/UICommandMenu.hxx>
#include <UI/UIMainMenu.hxx>
#include <UI/UIObjects.hxx>
namespace ui {
namespace menu {

void CommandMenu::notify(uint64_t publisher) {
  updateEntries();
  evaluateState();
}
void CommandMenu::updateEntries() {
  m_entries.clear();
  if (objects::serviceMenu->isNull()) {
    return;
  }

  auto &context = objects::serviceMenu->option();

  auto source = DIM_MANAGER.getCommands(context);
  if (source.empty()) {
    return;
  }
  m_entries.insert(m_entries.end(), std::make_move_iterator(source.begin()),
                   std::make_move_iterator(source.end()));
  m_entries.push_back(SEND_CMD_INPUT.data());
}
} // namespace menu
} // namespace ui
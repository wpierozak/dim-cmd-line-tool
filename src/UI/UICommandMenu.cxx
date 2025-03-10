#include <UI/UICommandMenu.hxx>
#include <UI/UIMainMenu.hxx>
namespace ui {
namespace menu {
void CommandMenu::updateEntries(opt_str_ref contextOpt) {
  m_entries.clear();
  if (contextOpt.has_value() == false) {
    return;
  }
  auto context = *contextOpt;

  auto source = DIM_MANAGER.getCommands(context);
  if (source.empty()) {
    return;
  }

  std::transform(source.begin(), source.end(), std::back_inserter(m_entries),
                 [](const std::string &entry) { return entry; });
  m_entries.push_back(SEND_CMD_INPUT.data());
}
} // namespace menu
} // namespace ui
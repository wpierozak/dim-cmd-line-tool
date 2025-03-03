#include <UI/UITypes.hxx>
#include<UI/UIObjects.hxx>
#include<UI/UIMenuContent.hxx>

namespace ui {
namespace types {
void Menu::evaluateState() {
  if (m_entries.empty()) {
    updateState(std::nullopt);
    return;
  }
  if(m_selected == -1){
    m_selected  = 0;
  }
  updateState(m_entries[m_selected]);
}

void Menu::notify(std::string publisher, std::optional<std::string> state) {
  if (state.has_value()) {
    updateEntries(state.value());
  } else if (m_entries.empty() == false) {
    m_entries.clear();
    m_selected = -1;
  }
  evaluateState();
}

void Menu::updateEntries(std::string context) {
  m_entries = m_entriesSource(context);
}

void MessageBox::notify(std::string publisher, std::optional<std::string> context)
{
  evaluateState();
}

void MessageBox::evaluateState()
{
  if(ui::objects::mainMenu->nullableOption() == menu::main::LOGS){
    printLogs();
  } else if(ui::objects::mainMenu->nullableOption() == menu::main::PRINT_LATEST_DATA){
    printLatestData();
  }
}

void MessageBox::printLogs(){
  m_content = MultiLineText(Logger::Get().getQuietLogs());
}

void MessageBox::printLatestData()
{
  opt_str service = ui::objects::serviceMenu->nullableOption();
  if(service == std::nullopt){
    m_content = MultiLineText("");
    return;
  }
  auto res = DIM_MANAGER.getServiceData(service.value(),true);
  if(res.isError()){
    m_content = MultiLineText(res.error.value());
    return;
  }
  m_content = MultiLineText(res.result.value_or(""));
}

ftxui::Element MessageBox::Render()
{
  return m_content.Render();
}

MultiLineText::MultiLineText(const std::string &text, size_t lines) {
  if (lines == std::string::npos) {
    m_lines = tools::split_string_by_newline(text);
  } else {
    m_lines = tools::get_last_n_lines(text, lines);
  }
}

ftxui::Element MultiLineText::Render() {
  ftxui::Elements elements;
  for (const auto &line : m_lines) {
    elements.push_back(ftxui::paragraph(line));
  }
  return ftxui::vbox(elements);
}
void Command::moveReady(const std::string &command) {
  std::lock_guard lock(m_mutex);
  m_command = command;
}

void Command::moveWaiting() {
  m_state = State::Waiting;

  utils::Result<std::string, std::string> res;
  if (type == Type::Input) {
    res = DIM_MANAGER.executeCommand(m_commandSender, m_command, true);
  } else {
    res = DIM_MANAGER.executeKnownCommand(m_commandSender, m_command, true);
  }
  if (res.isError()) {
    std::lock_guard lock(m_mutex);
    m_state = State::Failure;
    m_errorMessage = res.error.value_or("");
  } else {
    std::lock_guard lock(m_mutex);
    m_state = State::Finished;
    m_response = res.result.value_or("");
  }
}

void Command::moveFinished() {
  utils::Result<std::string, std::string> res;
  LOG(INFO) << "EXECUTING";
  if (type == Type::Input) {
    res = DIM_MANAGER.executeCommand(m_commandSender, m_command, false);
  } else {
    res = DIM_MANAGER.executeKnownCommand(m_commandSender, m_command, false);
  }
  if (res.isError()) {
    std::lock_guard lock(m_mutex);
    m_state = State::Failure;
    m_errorMessage = res.error.value_or("Failed to executed command");
  } else {
    std::lock_guard lock(m_mutex);
    m_state = State::Finished;
    m_response = res.result;
  }
}
} // namespace types
} // namespace ui
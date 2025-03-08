#include <UI/UIMenuContent.hxx>
#include <UI/UIObjects.hxx>
#include <UI/UITypes.hxx>

namespace ui {
namespace types {

void MessageBox::notify(const std::string &publisher, opt_str_ref context) {
  evaluateState();
}

void MessageBox::evaluateState() {
  if (ui::objects::mainMenu->option() == menu::main::PRINT_LATEST_DATA) {
    printLatestData();
  } else if (ui::objects::mainMenu->option() == menu::main::LOGS) {
    printLogs();
  } else if (ui::objects::mainMenu->option() == menu::main::SEND_COMMAND_WAIT) {
    printCommand();
  } else {
    m_content = MultiLineText("");
  }
}

void MessageBox::printCommand() {
  auto resp = objects::command->response();
  auto err = objects::command->error();
  if (resp.has_value()) {
    m_content = MultiLineText(resp.value());
  } else if (err.has_value()) {
    m_content = MultiLineText(err.value());
  }
}

void MessageBox::printLogs() {
  m_content = MultiLineText(Logger::Get().getQuietLogs(), 3);
}

void MessageBox::printLatestData() {
  opt_str service = ui::objects::serviceMenu->nullableOption();
  if (service == std::nullopt) {
    m_content = MultiLineText("");
    return;
  }
  auto res = DIM_MANAGER.getServiceData(service.value(), true);
  if (res.isError()) {
    m_content = MultiLineText(res.error.value());
    return;
  }
  m_content = MultiLineText(res.result.value_or(""));
}

ftxui::Element MessageBox::Render() { return m_content.Render(); }

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

void Command::notify(const std::string &publisher, opt_str_ref context) {
  if (publisher == ui::objects::mainMenu->identity()) {
    if (context->get() != menu::main::SEND_COMMAND &&
        context->get() != menu::main::SEND_COMMAND_WAIT) {
      m_state = State::Invalid;
    } else {
      m_state = State::Active;
    }
  } else if (publisher == ui::objects::serviceMenu->identity() &&
             m_state != State::Invalid) {
    if (ui::objects::serviceMenu->nullableOption().has_value()) {
      m_commandSender = ui::objects::serviceMenu->option();
    }
  } else if (publisher == ui::objects::commandsMenu->identity() &&
             m_state != State::Invalid) {
    if (context->get() == menu::commands::SEND_CMD_INPUT) {
      m_type = Type::Input;
    } else {
      m_command = context.value();
      m_type = Type::Known;
    }
    m_state = State::Ready;
  }
}

void Command::executeAndWait() {
  if (m_state == State::Invalid) {
    LOG(ERROR) << "Command is not ready! State: " << stateToString(m_state);
    return;
  }
  utils::Result<std::string, std::string> res;
  if (m_type == Type::Input) {
    res = DIM_MANAGER.executeCommand(m_commandSender, m_command, true);
  } else {
    res = DIM_MANAGER.executeKnownCommand(m_commandSender, m_command, true);
  }
  if (res.isError()) {
    m_errorMessage = res.error.value_or("");
  } else {
    m_response = res.result.value_or("");
  }
}

void Command::execute() {
  if (m_state == State::Invalid) {
    LOG(ERROR) << "Command is not ready! State: " << stateToString(m_state);
    return;
  }

  utils::Result<std::string, std::string> res;
  LOG(INFO) << "Executing command on '" << m_commandSender + "'";
  if (m_type == Type::Input) {
    res = DIM_MANAGER.executeCommand(m_commandSender, m_command, false);
  } else {
    res = DIM_MANAGER.executeKnownCommand(m_commandSender, m_command, false);
  }
  if (res.isError()) {
    m_errorMessage = res.error.value_or("Failed to executed command");
  } else {
    m_response = res.result;
  }
}
} // namespace types
} // namespace ui
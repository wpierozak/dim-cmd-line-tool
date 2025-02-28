#include <DimHandlers/CommandSender.hxx>
namespace dim_handlers {
utils::Result<bool, std::string>
CommandSender::addCommandFromFile(const std::string &commandName,
                                  const std::string &fileName) {
  LOG_SERVICE(DEBUG) << "Adding new command " << commandName << " from file "
                     << fileName;

  if (m_commands.find(commandName) != m_commands.end()) {
    return {.result = false, .error = "Command already exists."};
  }

  std::ifstream file;
  file.open(fileName);
  if (file.is_open() == false) {
    return {.result = false, .error = "Failed to open file."};
  }

  std::stringstream buffer;
  buffer << file.rdbuf();
  m_commands[commandName] = buffer.str();
  file.close();
  LOG_SERVICE(DATA) << "Command:\n" << m_commands[commandName];
  return {.result = true};
}

utils::Result<bool, std::string>
CommandSender::addCommand(const std::string &commandName,
                          const std::string &commandText) {
  if (m_commands.find(commandName) != m_commands.end()) {
    return {.result = false, .error = "Command already exists."};
  }

  m_commands[commandName] = commandText;
  return {.result = true};
}
} // namespace dim_handlers
#pragma once

#include <Utils/Result.hxx>
#include <dim/dic.hxx>
#include <fstream>
#include <map>
#include <set>
#include <sstream>

#include "Service.hxx"

namespace dim_handlers {
class CommandSender : public Service {
public:
  CommandSender(std::string name, std::optional<std::string> alias)
      : Service(name, alias, Type::Command) {}

  utils::Result<bool, std::string>
  addCommandFromFile(const std::string &commandName,
                     const std::string &fileName);
  utils::Result<bool, std::string> addCommand(const std::string &commandName,
                                              const std::string &commandText);

  bool sendKnownCommand(const std::string &commandName) {
    LOG_SERVICE(DEBUG) << "Executing " << commandName << "...";
    if (m_commands.find(commandName) == m_commands.end()) {
      return false;
    }

    return DimClient::sendCommand(getName().c_str(),
                                  m_commands[commandName].c_str());
  }

  bool sendCommand(const std::string &command) {
    return DimClient::sendCommand(getName().c_str(), command.c_str());
  }

  void addResponseService(const std::string &serviceName) {
    m_responseServices.emplace(serviceName);
  }

  const std::set<std::string> &getResponseServices() const {
    return m_responseServices;
  }

  std::list<std::string> getCommands() {
    std::list<std::string> commands;
    for (auto [name, cmd] : m_commands) {
      commands.push_back(name);
    }
    return commands;
  }

private:
  std::map<std::string, std::string> m_commands;
  std::set<std::string> m_responseServices;
};

} // namespace dim_handlers
#pragma once

#include <Utils/Result.hxx>
#include <memory>

#include "CommandSender.hxx"
#include "RpcInfo.hxx"
#include "ServiceInfo.hxx"

#define DIM_MANAGER dim_handlers::DimManager::Instance()

namespace dim_handlers {
class DimManager {
public:
  static DimManager &Instance() { return s_Instance; }

  utils::Result<bool, std::string>
  createSubscriber(const std::string &service, std::optional<std::string> alias,
                   Subscriber::Type type,
                   std::optional<uint32_t> timeout = 1000);
  utils::Result<bool, std::string>
  createSubscriber(const std::string &service, std::optional<std::string> alias,
                   const std::string &file, Subscriber::Type type,
                   std::optional<uint32_t> timeout = 1000);

  utils::Result<bool, std::string>
  createCommandSender(const std::string &service, std::optional<std::string>);
  utils::Result<bool, std::string> addCommand(const std::string &commandSender,
                                              const std::string &commandName,
                                              const std::string &commandText);
  utils::Result<bool, std::string>
  addCommandFromFile(const std::string &commandSender,
                     const std::string &commandName,
                     const std::string &fileName);

  utils::Result<bool, std::string>
  addResponseServiceToCommandSender(const std::string &service,
                                    const std::string &responseService);

  utils::Result<std::string, std::string>
  executeKnownCommand(const std::string &service, const std::string &command,
                      bool waitForResponse = false);
  utils::Result<std::string, std::string>
  executeCommand(const std::string &service, const std::string &command,
                 bool waitForResponse = false);
  utils::Result<std::string, std::string>
  getServiceData(const std::string &service, bool getImmediateData = false);

  std::list<std::string> getCommandSenders();
  std::list<std::string> getSubscribers();
  std::list<std::string> getCommands(const std::string &service);

  bool isSubscriber(const std::string &name) {
    return (m_subscribersByName.find(name) != m_subscribersByName.end()) ||
           (m_subscribersByAlias.find(name) != m_subscribersByAlias.end());
  }

  uint64_t getServiceState(const std::string &name) {
    if (m_subscribersByName.find(name) != m_subscribersByName.end()) {
      return m_subscribersByName[name]->state();
    } else if (m_subscribersByAlias.find(name) != m_subscribersByAlias.end()) {
      return m_subscribersByAlias[name]->state();
    }
    return 0;
  }

private:
  DimManager() = default;
  utils::Result<std::string, std::string>
  waitForData(const std::string &commandService);

  static DimManager s_Instance;
  std::map<std::string, std::shared_ptr<Subscriber>> m_subscribersByName;
  std::map<std::string, std::shared_ptr<Subscriber>> m_subscribersByAlias;

  std::map<std::string, std::shared_ptr<CommandSender>> m_commandSendersByName;
  std::map<std::string, std::shared_ptr<CommandSender>> m_commandSendersByAlias;
};
} // namespace dim_handlers
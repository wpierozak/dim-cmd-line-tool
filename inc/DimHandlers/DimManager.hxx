#pragma once

#include"ServiceInfo.hxx"
#include"RpcInfo.hxx"
#include"CommandSender.hxx"

#include<memory>
#include<Utils/Result.hxx>

#define DIM_MANAGER dim_handlers::DimManager::Instance()

namespace dim_handlers
{
class DimManager
{
public:
    static DimManager& Instance()
    {
        return s_Instance;
    }

    utils::Result<bool,std::string> createSubscriber(const std::string& service, const std::string& alias, Subscriber::Type type);
    utils::Result<bool,std::string> createSubscriber(const std::string& service, const std::string& alias, const std::string& file, Subscriber::Type type);
    
    utils::Result<bool,std::string> createCommandSender(const std::string& service, const std::string& alias);
    utils::Result<bool,std::string> addCommand(const std::string& commandSender, const std::string& commandName, const std::string& commandText);
    utils::Result<bool,std::string> addCommandFromFile(const std::string& commandSender, const std::string& commandName, const std::string& fileName);

    utils::Result<bool,std::string> addResponseServiceToCommandSender(const std::string& service, const std::string& responseService);

    utils::Result<std::string,std::string> executeKnownCommand(const std::string& service, const std::string& command, bool waitForResponse=false);
    utils::Result<std::string,std::string> executeCommand(const std::string& service, const std::string& command, bool waitForResponse=false);
    utils::Result<std::string,std::string> getServiceData(const std::string& service, bool getImmediateData = false);

private:
    DimManager() = default;
    utils::Result<std::string,std::string> waitForData(const std::string& commandService);

    static DimManager s_Instance;
    std::map<std::string, std::shared_ptr<Subscriber>> m_subscribersByName;
    std::map<std::string, std::shared_ptr<Subscriber>> m_subscribersByAlias;

    std::map<std::string, std::shared_ptr<CommandSender>> m_commandSendersByName;
    std::map<std::string, std::shared_ptr<CommandSender>> m_commandSendersByAlias;
};
}
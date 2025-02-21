#include<DimHandlers/DimManager.hxx>
#include<thread>
#include<mutex>
#include<Logging/Logger.hxx>

namespace dim_handlers
{

DimManager DimManager::s_Instance = DimManager();

utils::Result<bool,std::string> DimManager::createSubscriber(const std::string& name, const std::string& alias, Subscriber::Type type)
{
    if(m_subscribersByName.find(name) != m_subscribersByName.end()){
        LOG(ERROR) << "Subscriber to " << name << " already exists.";
        return {.result=false,.error="Subscriber of that name already exists."};
    }
    if(m_subscribersByAlias.find(alias) != m_subscribersByAlias.end()){
        LOG(ERROR) << "Subscriber of alias: '" << alias << "' already exists";
        return {.result=false,.error="Subscriber of that alias already exists."};
    }

    std::shared_ptr<Subscriber> subscriber;
    if(type == Subscriber::Type::ServiceInfo){
        LOG(DEBUG) << "Creating ServiceInfo subscriber with name: " << name << " and alias: " << alias;
        subscriber = std::make_shared<ServiceInfo>(name, alias);
    }else if(type == Subscriber::Type::RpcInfo){
        LOG(DEBUG) << "Creating RpcInfo subscriber with name: " << name << " and alias: " << alias;
        subscriber = std::make_shared<RpcInfo>(name, alias);
    }else{
        return {.error="Unknown subscriber type."};
    }

    m_subscribersByName[name] = subscriber;
    m_subscribersByAlias[alias] = subscriber;
    LOG(DEBUG) << "Subscriber '" << name << "' with alias '" << alias << "' successfully initialized";
    return {.result=true};
}

utils::Result<bool,std::string> DimManager::createSubscriber(const std::string& service, const std::string& alias, const std::string& file, Subscriber::Type type)
{
    auto creationRes = createSubscriber(service,alias,type);
    if(creationRes.isError()){
        return creationRes;
    }
    bool success = m_subscribersByName[service]->setFile(file);
    if(!success){
        return {.error="Failed to open a file"};
    }
    return {.result=true};
}

utils::Result<bool,std::string> DimManager::createCommandSender(const std::string& service, const std::string& alias)
{
    if(m_commandSendersByName.find(service) != m_commandSendersByName.end()){
        return {.result=false,.error="Command sender of that name already exists."};
    }
    if(m_commandSendersByAlias.find(alias) != m_commandSendersByAlias.end()){
        return {.result=false,.error="Command sender of that alias already exists."};
    }

    std::shared_ptr<CommandSender> commandSender = std::make_shared<CommandSender>(service, alias);
    m_commandSendersByName[service] = commandSender;
    m_commandSendersByAlias[alias] = commandSender;
    return {.result=true};
}

utils::Result<bool,std::string> DimManager::addCommand(const std::string& commandSender, const std::string& commandName, const std::string& commandText)
{
    if(m_commandSendersByName.find(commandSender) == m_commandSendersByName.end()){
        return {.result=false,.error="Command sender does not exist."};
    }
    return m_commandSendersByName[commandSender]->addCommand(commandName, commandText);
}

utils::Result<bool,std::string> DimManager::addCommandFromFile(const std::string& commandSender, const std::string& commandName, const std::string& fileName)
{
    if(m_commandSendersByName.find(commandSender) == m_commandSendersByName.end()){
        return {.result=false,.error="Command sender does not exist."};
    }
    return m_commandSendersByName[commandSender]->addCommandFromFile(commandName, fileName);
}

utils::Result<bool,std::string> DimManager::addResponseServiceToCommandSender(const std::string& service, const std::string& responseService)
{
    if(m_commandSendersByName.find(service) == m_commandSendersByName.end()){
        return {.result=false,.error="Command sender does not exist."};
    }

    m_commandSendersByName[service]->addResponseService(responseService);
    return {.result=true};
}

utils::Result<std::string,std::string> DimManager::executeKnownCommand(const std::string& service, const std::string& command, bool waitForResponse)
{
    if(m_commandSendersByName.find(service) == m_commandSendersByName.end()){
        return {.error="Command sender does not exist."};
    }

    if(waitForResponse){
        m_commandSendersByName[service]->sendKnownCommand(command);
        return waitForData(service);
    }
    m_commandSendersByName[service]->sendKnownCommand(command);
    return {.result="Command sent."};
}

utils::Result<std::string,std::string> DimManager::executeCommand(const std::string& service, const std::string& command, bool waitForResponse)
{
    if(m_commandSendersByName.find(service) == m_commandSendersByName.end()){
        return {.error="Command sender does not exist."};
    }

    if(waitForResponse){
        m_commandSendersByName[service]->sendCommand(command);
        return waitForData(service);
    }
    m_commandSendersByName[service]->sendCommand(command);
    return {.result="Command sent."};
}

utils::Result<std::string,std::string> DimManager::waitForData(const std::string& commandService)
{
    std::mutex listMutex;
    std::map<std::string, std::optional<std::string>> dataMap;
    uint32_t threadsNum =  m_commandSendersByName[commandService]->getResponseServices().size() + 1;

    #pragma omp parallel num_threads(threadsNum)
    {
        for(const auto& responseService: m_commandSendersByName[commandService]->getResponseServices()){
            if(m_subscribersByName.find(responseService) != m_subscribersByName.end()){
                #pragma omp task
                {
                    auto result = m_subscribersByName[responseService]->waitForData();
                    if(result.has_value()){
                        std::lock_guard<std::mutex> lock(listMutex);
                        dataMap[responseService] = result;
                    }
                }
            } else if(m_subscribersByAlias.find(responseService) != m_subscribersByAlias.end()){
                #pragma omp task
                {
                    auto result = m_subscribersByAlias[responseService]->waitForData();
                    if(result.has_value()){
                        std::lock_guard<std::mutex> lock(listMutex);
                        dataMap[responseService] = result;
                    }
                }
            }
        }
    }

    if(dataMap.empty()){
        return {.error="No data received."};
    }

    if(dataMap.size() == 1){
        return {.result=dataMap.begin()->second.value()};
    }

    return {.error = "Unexpected behaviour. Multiple responses received."};
}

utils::Result<std::string,std::string> DimManager::getServiceData(const std::string& service, bool getImmediateData)
{
    if(m_subscribersByName.find(service) == m_subscribersByName.end()){
        return {.error="Subscriber does not exist."};
    }

    if(getImmediateData){
        auto data = m_subscribersByName[service]->getData();
        if(data.has_value()){
            m_subscribersByName[service]->popFront();
            return {.result=data.value()};
        }
        return {.error="No data available."};
    }

    auto data = m_subscribersByName[service]->waitForData();
    if(data == std::nullopt){
        return {.error="No data received."};
    }
    m_subscribersByName[service]->popFront();
    return {.result = data};
}

}
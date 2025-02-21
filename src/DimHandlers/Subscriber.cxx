#include<DimHandlers/Subscriber.hxx>
#include<Logging/Logger.hxx>
#include<iostream>

namespace dim_handlers
{

void Subscriber::print(TerminalOutput output)
{
    if(m_serviceData.empty()){
        return;
    }
    LOG_DATA(Service::getName(), m_serviceData.front());
}

bool Subscriber::setFile(const std::string &fileName)
{
    if(m_fileName.has_value()){
        CLOSE_LOG_FILE(m_fileName.value());
    }
    m_fileName = fileName;
    return OPEN_LOG_FILE(m_fileName.value());
}

bool Subscriber::saveToFile(const std::string& output)
{
    if(m_fileName.has_value() == false){
        return true;
    }
    LOG_SERVICE(DEBUG) << "Logging to file...";
    return LOG_TO_FILE(m_fileName.value(), output);
}

bool Subscriber::handleNewData(const std::string &data)
{
    m_serviceData.emplace_front(data);
    if(!saveToFile(data)){
        LOG_SERVICE(ERROR) << "File update failed!";
    }
    if(m_hideTerminal==false){
        print(TerminalOutput::Latest);
    }
    return true;
}

} // namespace dim_handlers
#include<DimHandlers/Subscriber.hxx>
#include<Logging/Logger.hxx>
#include<iostream>

namespace dim_handlers
{

void Subscriber::print(TerminalOutput output)
{
    LOG(INFO) << "Service: " << getName() << " Alias: " << getAlias();
    LOG(INFO) << "Data: " << getData().value();
}

bool Subscriber::setFile(const std::string &fileName)
{
    if(m_fileName.has_value()){
        LOGGER.closeFile(m_fileName.value());
    }
    return LOGGER.openFile(fileName);
}

bool Subscriber::saveToFile(const std::string& output)
{
    if(m_fileName.has_value() == false){
        return true;
    }
    return LOGGER.writeToFile(m_fileName.value(), output);
}

bool Subscriber::handleNewData(const std::string &data)
{
    m_serviceData.emplace_front(data);
    saveToFile(data);
    if(m_hideTerminal==false){
        print(TerminalOutput::Latest);
    }
    return true;
}

} // namespace dim_handlers
#include<dic.hxx>
#include"Service.hxx"
#include<Utils/Result.hxx>

#include<map>
#include<fstream>
#include<sstream>
#include<set>

namespace dim_handlers
{
class CommandSender: public Service
{
public:
    CommandSender(std::string name, std::string alias)
        : Service(name, alias, Type::Command)
    {}

    utils::Result<bool,std::string> addCommandFromFile(const std::string &commandName, const std::string &fileName);
    utils::Result<bool,std::string> addCommand(const std::string &commandName, const std::string &commandText);

    bool sendCommand(const std::string &commandName)
    {
        if(m_commands.find(commandName) == m_commands.end()){
            return false;
        }

        return DimClient::sendCommand(getName().c_str(), m_commands[commandName].c_str());
    }

    void addResponseService(const std::string &serviceName)
    {
        m_responseServices.emplace(serviceName);
    }
   
private:
    std::map<std::string, std::string> m_commands;
    std::set<std::string> m_responseServices;
};

}// namespace dim_handlers
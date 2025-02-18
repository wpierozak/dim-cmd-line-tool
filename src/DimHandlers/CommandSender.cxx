#include<DimHandlers/CommandSender.hxx>
namespace dim_handlers
{
utils::Result<bool,std::string> CommandSender::addCommandFromFile(const std::string &commandName, const std::string &fileName)
{
    if(m_commands.find(commandName) != m_commands.end()){
        return {.result=false,.error="Command already exists."};
    }

    std::ofstream file;
    file.open(fileName);
    if(file.is_open() == false){
        return {.result=false,.error="Failed to open file."};
    }

    std::stringstream buffer;
    buffer << file.rdbuf();
    m_commands[commandName] = buffer.str();
    file.close();

    return {.result=true};
}
utils::Result<bool,std::string> CommandSender::addCommand(const std::string &commandName, const std::string &commandText)
{
    if(m_commands.find(commandName) != m_commands.end()){
        return {.result=false,.error="Command already exists."};
    }

    m_commands[commandName] = commandText;
    return {.result=true};
}
}
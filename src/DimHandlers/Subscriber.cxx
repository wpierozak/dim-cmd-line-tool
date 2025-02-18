#include<DimHandlers/Subscriber.hxx>
#include<iostream>

namespace dim_handlers
{

void Subscriber::print(TerminalOutput output)
{
    if (output == TerminalOutput::Latest)
        std::cout << getData() << std::endl;
    else
        for (const auto &data : m_serviceData)
            std::cout << data << std::endl;
}

bool Subscriber::setFile(const std::string &fileName)
{
    if(m_file.has_value()){
        m_file->close();
        m_file.reset();
    }
    m_fileName = fileName;
    m_file = std::ifstream(fileName);
    if(m_file->is_open()==false){
        return false;
    }
    return true;
}

bool Subscriber::saveToFile(const std::string& output)
{
    if(m_file.has_value()==false){
        return false;
    }
    (*m_file) << output;
    return true;
}

bool Subscriber::handleNewData(const std::string &data)
{
    m_serviceData.emplace_front(data);
    if(m_file.has_value()){
        (*m_file) << data;
    }
    if(m_hideTerminal==false){
        print(TerminalOutput::Latest);
    }
    return true;
}

} // namespace dim_handlers
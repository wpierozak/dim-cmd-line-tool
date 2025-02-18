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

void Subscriber::setFileName(const std::string &fileName)
{
    if(m_file.has_value()){
        m_file->close();
        m_file.reset();
    }
    m_fileName = fileName;
}


} // namespace dim_handlers
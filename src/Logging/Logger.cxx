#define OOF_IMPL
#include<Logging/Logger.hxx>

Logger Logger::Instance = Logger();

Logger& Logger::operator()(Mode mode)
{
    m_mode = mode;
    switch(mode){
        case Mode::Debug:
            std::cout << std::endl << oof::fg_color({0,0,255}) << "[DEBUG] " << oof::reset_formatting();
            break;
        case Mode::Info:
            std::cout << std::endl << oof::fg_color({0,255,0}) << "[INFO] " << oof::reset_formatting();
            break;
        case Mode::Warning:
            std::cout << std::endl << oof::fg_color({255,255,0}) << "[WARNING] " << oof::reset_formatting();
            break;
        case Mode::Error:
            std::cout << std::endl << oof::fg_color({255,0,0}) << "[ERROR] " << oof::reset_formatting();
            break;
    }
    return *this;
}

template<typename T>
Logger& Logger::operator<<(const T& message){
    std::cout <<  message;
    return *this;
}


Logger& Logger::operator<<(const char* message)
{
    std::cout <<  message;
    return *this;
}

bool Logger::isFileOpen(const std::string& name)
{
    if(m_files.find(name) != m_files.end())
    {
        return m_files[name].file.is_open();
    }
    return false;
}

bool Logger::openFile(const std::string& name)
{
    if(m_files.find(name) != m_files.end())
    {
        LOG(ERROR) << "File " << name << " is already open";
        return false;
    }
    m_files[name].file.open(name);
    if(m_files[name].file.is_open())
    {
        LOG(INFO) << "File " << name << " opened successfully";
        return true;
    }
    LOG(ERROR) << "Failed to open file " << name;
    return false;
}

void Logger::closeFile(const std::string& name)
{
    if(m_files.find(name) != m_files.end())
    {
        m_files[name].file.close();
        LOG(INFO) << "File " << name << " closed successfully";
    }
    else
    {
        LOG(ERROR) << "File " << name << " is not open";
    }
}

bool Logger::writeToFile(const std::string& fileName, const std::string& content)
{
    if(m_files.find(fileName) != m_files.end())
    {
        std::lock_guard<std::mutex> lock(m_files[fileName].mutex);
        m_files[fileName].file << content;
        return true;
    }
    return false;
}

void Logger::printServiceData(const std::string& serviceName, const std::string& data)
{
    LOG(INFO) << "Service: " << serviceName;
    LOG(INFO) << " Data: " << data;
}
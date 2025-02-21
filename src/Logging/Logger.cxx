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
        case Mode::Data:
            std::cout << std::endl << oof::fg_color({247,150,250}) << "[DATA] " << oof::reset_formatting();
            break;
    }
    return *this;
}

Logger& Logger::operator()(Mode mode, std::string_view service)
{
    switch(mode){
        case Mode::Debug:
            std::cout << std::endl << oof::fg_color({0,0,255}) << "[DEBUG] " << "[" << service << "] " << oof::reset_formatting();
            break;
        case Mode::Info:
            std::cout << std::endl << oof::fg_color({0,255,0}) << "[INFO] " << "[" << service << "] " << oof::reset_formatting();
            break;
        case Mode::Warning:
            std::cout << std::endl << oof::fg_color({255,255,0}) << "[WARNING] " << "[" << service << "] " << oof::reset_formatting();
            break;
        case Mode::Error:
            std::cout << std::endl << oof::fg_color({255,0,0}) << "[ERROR] " << "[" << service << "] " << oof::reset_formatting();
            break;
            case Mode::Data:
            std::cout << std::endl << oof::fg_color({247,150,250}) << "[DATA] " << "[" << service << "] " << oof::reset_formatting();
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

Logger& Logger::operator<<(uint32_t value)
{
    std::cout << value;
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
        if(m_files[fileName].file.is_open() == false){
            LOG(ERROR) << "File " << fileName << " is not open!";
        }
        m_files[fileName].file << std::endl << content << std::endl;
        m_files[fileName].file.flush();
        return true;
    }
    return false;
}

void Logger::printServiceData(std::string_view service,  std::string_view data)
{
    (*this)(DATA,service) << data;
}
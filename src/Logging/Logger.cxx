#define OOF_IMPL
#include<Logging/Logger.hxx>

Logger Logger::Instance = Logger();

Log Logger::operator()(Mode mode, std::string_view service)
{
   if(m_quietLogs.has_value()){
    return (*this)(m_quietLogs->monitor(),mode,service);
   } else if(m_logFile.has_value()){
    return (*this)(m_logFile->monitor(),mode,service);
   } 
   return(*this)(m_stdStream,mode,service);
}

Log Logger::operator()(StreamMonitor& stream, Mode mode, std::string_view service)
{
    Log log(stream);
    log << "\n" << oof::fg_color(modeColor(mode)) << "[" << modeName(mode) << "] ";
    if(service.empty() == false){
        log << "[" << service << "] ";
    }
    log.format(oof::reset_formatting());
    return log;   
}

bool Logger::isFileOpen(const std::string& name)
{
    if(m_files.find(name) != m_files.end())
    {
        return m_files.at(name)->isOpen();
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
    m_files.emplace(name,std::make_unique<FileStream>(name));
    if(m_files.at(name)->isOpen())
    {
        LOG(INFO) << "File " << name << " opened successfully";
        return true;
    }
    LOG(ERROR) << "Failed to open file " << name;
    return false;
}

void Logger::closeFile(const std::string& name)
{
   m_files.erase(name);
}

bool Logger::writeToFile(const std::string& fileName, const std::string& content)
{
    if(m_files.find(fileName) != m_files.end())
    {
        m_files.at(fileName)->monitor() << "\n" << content;
        m_files.at(fileName)->monitor().flush();
        return true;
    }
    return false;
}

void Logger::printServiceData(std::string_view service, std::string_view data)
{
    (*this)(DATA,service) << data.data();
}

oof::color Logger::modeColor(Mode mode)
{
    switch (mode) {
        case Mode::Info:
            return {0,255,0};
        case Mode::Warning:
            return {255,255,0};
        case Mode::Error:
            return {255,0,0};
        case Mode::Debug:
            return {0,0,255};
        default:
        return {200, 162, 200};
    }
}

std::string Logger::modeName(Mode mode)
{
    switch (mode) {
        case Mode::Info:
            return "INFO";
        case Mode::Warning:
            return "WARNING";
        case Mode::Error:
            return "ERROR";
        case Mode::Debug:
            return "DEBUG";
        case Mode::Data:
            return "DATA";
    }
}
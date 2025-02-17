#define OOF_IMPL
#include<Logging/Logger.hxx>

Logger Logger::Instance = Logger();

template<typename T>
Logger& Logger::operator<<(const T& message){
    switch(m_mode){
        case Mode::Debug:
            std::cout << oof::fg_color({0,255,0}) << "[DEBUG] " << oof::reset_formatting() <<  message << std::endl;
            break;
        case Mode::Info:
            std::cout << oof::fg_color({0,0,255}) << "[INFO] " << oof::reset_formatting() << message << std::endl;
            break;
        case Mode::Warning:
            std::cout << oof::fg_color({255,255,0}) << "[WARNING] " << oof::reset_formatting() << message << std::endl;
            break;
        case Mode::Error:
            std::cout << oof::fg_color({255,0,0}) << "[ERROR] " << oof::reset_formatting() << message << std::endl;
            break;
    }
    return *this;
}


Logger& Logger::operator<<(const char* message)
{
    switch(m_mode){
        case Mode::Debug:
            std::cout << oof::fg_color({0,255,0}) << "[DEBUG] " << oof::reset_formatting() << message << std::endl;
            break;
        case Mode::Info:
            std::cout << oof::fg_color({0,0,255}) << "[INFO] " << oof::reset_formatting() << message << std::endl;
            break;
        case Mode::Warning:
            std::cout << oof::fg_color({255,255,0}) << "[WARNING] " << oof::reset_formatting() << message << std::endl;
            break;
        case Mode::Error:
            std::cout << oof::fg_color({255,0,0}) << "[ERROR] " << oof::reset_formatting() << message << std::endl;
            break;
    }
    return *this;
}
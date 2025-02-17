#include<oof.h>
#include<iostream>

#define LOG(mode) Logger::Get()(mode)
#define ERROR Logger::Mode::Error
#define WARNING Logger::Mode::Warning
#define INFO Logger::Mode::Info
#define DEBUG Logger::Mode::Debug

class Logger
{
public:
enum class Mode{Debug, Info, Warning, Error};

static Logger& Get()
{
    return Instance;
}

Logger& operator()(Mode mode){
    m_mode = mode;
    return *this;
}

template<typename T>
Logger& operator<<(const T& message);

Logger& operator<<(const char* message);

private:

static Logger Instance;
Logger() = default;

Mode m_mode;
};
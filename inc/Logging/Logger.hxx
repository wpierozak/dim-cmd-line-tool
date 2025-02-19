#include<oof.h>
#include<iostream>
#include<fstream>
#include<mutex>

#define LOG(mode) Logger::Get()(mode)
#define ERROR Logger::Mode::Error
#define WARNING Logger::Mode::Warning
#define INFO Logger::Mode::Info
#define DEBUG Logger::Mode::Debug
#define LOGGER Logger::Get()

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

bool isFileOpen(const std::string& name);
bool doesFileExist(const std::string& name);
bool openFile(const std::string& name);
void closeFile(const std::string& name);
bool writeToFile(const std::string& fileName, const std::string& content);

private:
struct File
{
    const std::string name;
    std::ofstream file;
    std::mutex mutex;
};

static Logger Instance;
Logger() = default;

Mode m_mode;
std::unordered_map<std::string,File> m_files;
};
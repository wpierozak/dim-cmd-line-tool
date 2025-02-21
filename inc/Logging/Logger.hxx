#pragma once

#include<oof.h>
#include<iostream>
#include<fstream>
#include<mutex>

#define LOG(mode) Logger::Get()(mode)

#define ERROR Logger::Mode::Error
#define WARNING Logger::Mode::Warning
#define INFO Logger::Mode::Info
#define DEBUG Logger::Mode::Debug
#define DATA Logger::Mode::Data

#define LOG_DATA(name, data) Logger::Get().printServiceData(name, data)

#define LOG_TO_FILE(file,content) Logger::Get().writeToFile(file,content)
#define OPEN_LOG_FILE(file) Logger::Get().openFile(file)
#define CLOSE_LOG_FILE(file) Logger::Get().closeFile(file)

class Logger
{
public:
    enum class Mode{Debug, Info, Warning, Error, Data};

    static Logger& Get()
    {
        return Instance;
    }

    Logger& operator()(Mode mode);
    Logger& operator()(Mode mode, std::string_view service);

    template<typename T>
    Logger& operator<<(const T& message);
    Logger& operator<<(const char* message);
    Logger& operator<<(uint32_t value);

    void printServiceData(std::string_view service,  std::string_view data);

    bool isFileOpen(const std::string& name);
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
    std::mutex m_loggerMutex;
    std::unordered_map<std::string,File> m_files;

};
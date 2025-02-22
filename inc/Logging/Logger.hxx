#pragma once
#include"Stream.hxx"
#include"Log.hxx"
#include<oof.h>
#include<iostream>
#include<fstream>
#include<mutex>
#include<map>
#include<memory>

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

    Log operator()(Mode mode, std::string_view service = std::string_view());
    
    void printServiceData(std::string_view service,  std::string_view data);

    bool isFileOpen(const std::string& name);
    bool openFile(const std::string& name);
    void closeFile(const std::string& name);
    bool writeToFile(const std::string& fileName, const std::string& content);

private:
    Log operator()(StreamMonitor& stream, Mode mode, std::string_view service);
    oof::color modeColor(Mode mode);
    std::string modeName(Mode mode);

    static Logger Instance;
    Logger() = default;

    std::map<std::string,std::unique_ptr<FileStream>> m_files;
    
    bool m_quietMode{false};

    std::optional<FileStream> m_logFile;
    std::optional<StringStream> m_quietLogs;
    StreamMonitor m_stdStream{std::cout};
};
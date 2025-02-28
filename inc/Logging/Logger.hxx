#pragma once
#include <fstream>
#include <iostream>
#include <map>
#include <memory>
#include <mutex>

#include "Log.hxx"
#include "Stream.hxx"
#include "TextColor.hxx"

#define LOG(mode) Logger::Get()(mode)

#define ERROR Logger::Mode::Error
#define WARNING Logger::Mode::Warning
#define INFO Logger::Mode::Info
#define DEBUG Logger::Mode::Debug
#define DATA Logger::Mode::Data

#define LOG_DATA(name, data) Logger::Get().printServiceData(name, data)

#define LOG_TO_FILE(file, content) Logger::Get().writeToFile(file, content)
#define OPEN_LOG_FILE(file) Logger::Get().openFile(file)
#define CLOSE_LOG_FILE(file) Logger::Get().closeFile(file)

class Logger {
public:
  enum class Mode { Debug, Info, Warning, Error, Data };

  static Logger &Get() { return Instance; }

  Log operator()(Mode mode, std::string_view service = std::string_view());

  void printServiceData(std::string_view service, std::string_view data);

  bool isFileOpen(const std::string &name);
  bool openFile(const std::string &name);
  void closeFile(const std::string &name);
  bool writeToFile(const std::string &fileName, const std::string &content);

  std::string getQuietLogs() {
    if (isQuiet()) {
      return m_quietLogs->monitor().str();
    }
    return "";
  }

  bool isQuiet() { return m_quietLogs != nullptr; }

  void queit(bool on) {
    if (on && !isQuiet()) {
      m_stdStream.flush();
      m_quietLogs = std::make_unique<StringStream>();
    } else if (!on && isQuiet()) {
      m_quietLogs.reset();
    }
  }

  bool logToFile(const std::string &fileName) {
    m_logFile = std::make_unique<FileStream>(fileName);
    return m_logFile->isOpen();
  }
  ~Logger() { (*this)(WARNING) << "Destroying Logger instance\n"; }

private:
  Log operator()(StreamMonitor &stream, Mode mode, std::string_view service);
  color::Color modeColor(Mode mode);
  std::string modeName(Mode mode);

  static Logger Instance;
  Logger() = default;

  std::map<std::string, std::unique_ptr<FileStream>> m_files;

  bool m_quietMode{false};

  std::unique_ptr<FileStream> m_logFile;
  std::unique_ptr<StringStream> m_quietLogs;
  StreamMonitor m_stdStream{std::cout};
};
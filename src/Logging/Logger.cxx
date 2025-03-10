#include <Logging/Logger.hxx>

Logger Logger::Instance = Logger();

Log Logger::operator()(Mode mode, std::string_view service) {
  if (m_quietLogs != nullptr) {
    return (*this)(m_quietLogs->monitor(), mode, service);
  } else if (m_logFile != nullptr) {
    return (*this)(m_logFile->monitor(), mode, service);
  }
  return (*this)(m_stdStream, mode, service);
}

Log Logger::operator()(StreamMonitor &stream, Mode mode,
                       std::string_view service) {
  Log log(stream);
  log << "\n" << color::text(modeColor(mode)) << "[" << modeName(mode) << "] ";
  if (service.empty() == false) {
    log << "[" << service << "] ";
  }
  log << color::reset();
  return log;
}

bool Logger::isFileOpen(const std::string &name) {
  if (m_files.find(name) != m_files.end()) {
    return m_files.at(name)->isOpen();
  }
  return false;
}

bool Logger::openFile(const std::string &name) {
  if (m_files.find(name) != m_files.end()) {
    LOG(ERROR) << "File " << name << " is already open";
    return false;
  }
  m_files.emplace(name, std::make_unique<FileStream>(name));
  if (m_files.at(name)->isOpen()) {
    LOG(INFO) << "File " << name << " opened successfully";
    return true;
  }
  LOG(ERROR) << "Failed to open file " << name;
  return false;
}

void Logger::closeFile(const std::string &name) { m_files.erase(name); }

bool Logger::writeToFile(const std::string &fileName,
                         const std::string &content, int timeStamp,
                         int miliseconds) {
  auto data_format = [](int timestamp, int miliseconds) {
    time_t t = timestamp;
    struct tm *localTime = localtime(&t);
    char buffer[64];
    strftime(buffer, sizeof(buffer), "%d/%m/%Y %H:%M:%S", localTime);
    char result[80];
    snprintf(result, sizeof(result), "%s.%03d", buffer, miliseconds);
    return std::string(result);
  };

  if (m_files.find(fileName) != m_files.end()) {
    m_files.at(fileName)->monitor()
        << "\n\n[" << data_format(timeStamp, miliseconds) << "]\n"
        << content;
    m_files.at(fileName)->monitor().flush();
    return true;
  }
  return false;
}

bool Logger::writeToFile(const std::string &fileName,
                         const std::string &content) {
  if (m_files.find(fileName) != m_files.end()) {
    m_files.at(fileName)->monitor() << "\n" << content;
    m_files.at(fileName)->monitor().flush();
    return true;
  }
  return false;
}

void Logger::printServiceData(std::string_view service, std::string_view data) {
  (*this)(DATA, service) << data.data();
}

color::Color Logger::modeColor(Mode mode) {
  switch (mode) {
  case Mode::Info:
    return color::Color::GREEN;
  case Mode::Warning:
    return color::Color::YELLOW;
  case Mode::Error:
    return color::Color::RED;
  case Mode::Debug:
    return color::Color::BLUE;
  case Mode::Data:
    return color::Color::MAGENTA;
  }
}

std::string Logger::modeName(Mode mode) {
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
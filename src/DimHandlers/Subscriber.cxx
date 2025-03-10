#include <DimHandlers/Subscriber.hxx>
#include <Logging/Logger.hxx>
#include <iostream>

namespace dim_handlers {

void Subscriber::print(TerminalOutput output) {
  if (m_serviceData.empty()) {
    return;
  }
  LOG_DATA(Service::getName(), m_serviceData.front().data);
}

bool Subscriber::setFile(const std::string &fileName) {
  if (m_fileName.has_value()) {
    CLOSE_LOG_FILE(m_fileName.value());
  }
  m_fileName = fileName;
  return OPEN_LOG_FILE(m_fileName.value());
}

bool Subscriber::saveToFile(const std::string &output, int timeStamp,
                            int miliseconds) {
  if (m_fileName.has_value() == false) {
    return true;
  }
  LOG_SERVICE(DEBUG) << "Logging to file...";
  return LOG_TO_FILE_T(m_fileName.value(), output, timeStamp, miliseconds);
}

bool Subscriber::saveToFile(const std::string &output) {
  if (m_fileName.has_value() == false) {
    return true;
  }
  LOG_SERVICE(DEBUG) << "Logging to file...";
  return LOG_TO_FILE(m_fileName.value(), output);
}

bool Subscriber::handleNewData(const std::string &data, int timeStamp,
                               int miliseconds) {
  markStateChange();
  m_serviceData.emplace_front(data, timeStamp, miliseconds);
  if (m_serviceData.size() > m_bufferedDataLimit) {
    m_serviceData.pop_back();
  }

  if (!saveToFile(data, timeStamp, miliseconds)) {
    LOG_SERVICE(ERROR) << "File update failed!";
  }
  if (m_hideTerminal == false) {
    print(TerminalOutput::Latest);
  }
  return true;
}

bool Subscriber::handleNewData(const std::string &data) {
  markStateChange();
  m_serviceData.emplace_front(data);
  if (m_serviceData.size() > m_bufferedDataLimit) {
    m_serviceData.pop_back();
  }

  if (!saveToFile(data)) {
    LOG_SERVICE(ERROR) << "File update failed!";
  }
  if (m_hideTerminal == false) {
    print(TerminalOutput::Latest);
  }
  return true;
}

} // namespace dim_handlers
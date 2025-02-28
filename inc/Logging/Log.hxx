#pragma once
#include <functional>

#include "Stream.hxx"
class Log {
public:
  Log(StreamMonitor &logStream) : m_logStream(logStream) {}
  Log(Log &) = delete;
  Log(Log &&) = default;
  ~Log() { m_logStream.get() << m_localStream.rdbuf(); }

  template <typename T> std::stringstream &operator<<(const T &message) {
    m_localStream << message;
    return m_localStream;
  }

  std::stringstream &operator<<(const char *message) {
    m_localStream << message;
    return m_localStream;
  }

  std::stringstream &operator<<(uint32_t value) {
    m_localStream << value;
    return m_localStream;
  }

  template <typename T> std::stringstream &format(const T &formating) {
    return m_localStream << formating;
  }

private:
  std::stringstream m_localStream;
  std::reference_wrapper<StreamMonitor> m_logStream;
};
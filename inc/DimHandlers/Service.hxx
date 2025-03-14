#pragma once

#include <Logging/Logger.hxx>
#include <list>
#include <string>

#define LOG_SERVICE(mode) Logger::Get()(mode, Service::getName())
#define LOG_SERVICE_DATA(data)                                                 \
  Logger::Get().printServiceData(Service::getName(), data);

namespace dim_handlers {
class Service {
public:
  enum class Type { Command, ServiceInfo, RpcInfo };

  Service(std::string name, std::optional<std::string> alias, Type type)
      : m_name(name), m_alias(alias), m_type(type) {}

  std::string getName() const { return m_name; }
  std::optional<std::string> getAlias() const { return m_alias; }
  Type getType() const { return m_type; }
  uint64_t state() { return m_state; }
  void markStateChange() { m_state++; }

private:
  std::string m_name;
  std::optional<std::string> m_alias;
  uint64_t m_state;
  Type m_type;
};

} // namespace dim_handlers
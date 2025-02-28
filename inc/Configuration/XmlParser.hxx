#pragma once

#include <fstream>
#include <iostream>
#include <optional>
#include <sstream>
#include <string>
#include <vector>

#include "ServiceInfo.hxx"

namespace config {

class XmlParser {
public:
  bool load(std::string configFile);

private:
  void populateDimManager(std::string configPath);
  void createCommandSender(const config::ServiceInfo &service);
  void createSubscriber(const config::ServiceInfo &service);
};

} // namespace config
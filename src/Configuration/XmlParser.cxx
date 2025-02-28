#include <Configuration/XmlParser.hxx>
#include <DimHandlers/DimManager.hxx>

namespace config {
bool XmlParser::load(std::string configFile) {
  bool success = true;
  try {
    populateDimManager(configFile);
  } catch (std::exception &e) {
    LOG(ERROR) << "Parsing error: " << e.what();
    success = false;
  }
  if (success) {
    LOG(INFO) << "Configuration succeded";
  }
  return success;
}

void XmlParser::populateDimManager(std::string configPath) {
  std::ifstream file(configPath);
  if (!file) {
    throw std::runtime_error("Cannot open config file: " + configPath);
  }

  std::stringstream buffer;
  buffer << file.rdbuf();
  std::string content = buffer.str();
  std::vector<char> xmlData(content.begin(), content.end());
  xmlData.push_back('\0');

  rapidxml::xml_document<> doc;
  doc.parse<0>(xmlData.data());

  auto *root = doc.first_node("Configuration");
  if (!root) {
    throw std::runtime_error(
        "Missing <Configuration> root node in config file.");
  }

  for (auto *serviceNode = root->first_node("Service"); serviceNode;
       serviceNode = serviceNode->next_sibling("Service")) {
    config::ServiceInfo serviceInfo(serviceNode);
    std::string alias = serviceInfo.alias.value_or(serviceInfo.name);
    if (serviceInfo.type == "command") {
      createCommandSender(serviceInfo);
    } else if (serviceInfo.type == "service_info" ||
               serviceInfo.type == "rpc_info") {
      createSubscriber(serviceInfo);
    } else {
      throw std::runtime_error("Invalid service type: " + serviceInfo.type);
    }
  }
}

void XmlParser::createCommandSender(const ServiceInfo &serviceInfo) {
  auto senderRes = dim_handlers::DimManager::Instance().createCommandSender(
      serviceInfo.name, serviceInfo.alias);
  if (senderRes.isOk()) {
    for (auto &cmd : serviceInfo.commands) {
      if (cmd.command) {
        auto res = dim_handlers::DimManager::Instance().addCommand(
            serviceInfo.name, cmd.name, *cmd.command);
        if (res.isError()) {
          throw std::runtime_error(res.error.value());
        }
      } else if (cmd.file) {
        auto res = dim_handlers::DimManager::Instance().addCommandFromFile(
            serviceInfo.name, cmd.name, *cmd.file);
        if (res.isError()) {
          throw std::runtime_error(res.error.value());
        }
      }
    }
    if (serviceInfo.responseOn) {
      for (const auto &respService : serviceInfo.responseOn->services) {
        dim_handlers::DimManager::Instance().addResponseServiceToCommandSender(
            serviceInfo.name, respService);
      }
    }
  }
}

void XmlParser::createSubscriber(const ServiceInfo &serviceInfo) {
  auto type = serviceInfo.type == "service_info"
                  ? dim_handlers::Subscriber::Type::ServiceInfo
                  : dim_handlers::Subscriber::Type::RpcInfo;

  if (serviceInfo.output.has_value()) {
    dim_handlers::DimManager::Instance().createSubscriber(
        serviceInfo.name, serviceInfo.alias, serviceInfo.output->filename, type,
        serviceInfo.timeout.value_or(1000));
  } else {
    dim_handlers::DimManager::Instance().createSubscriber(
        serviceInfo.name, serviceInfo.alias, type,
        serviceInfo.timeout.value_or(1000));
  }
}
}; // namespace config
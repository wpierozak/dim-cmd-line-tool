#pragma once
#include <list>
#include <optional>
#include <rapidxml.hpp>
#include <string>
#include <string_view>

namespace config {
struct ServiceInfo {
  struct Output {
    Output(rapidxml::xml_node<> *node);
    std::string filename;
  };

  struct Command {
    Command(rapidxml::xml_node<> *node);

    std::string name;
    std::optional<std::string> command;
    std::optional<std::string> file;
  };

  struct Commands {
    Commands(rapidxml::xml_node<> *node);
    std::list<Command> commands;
  };

  struct ResponseOn {
    std::list<std::string> services;
  };

  ServiceInfo(rapidxml::xml_node<> *node);

  std::string name;
  std::optional<std::string> alias;
  std::string type;
  std::optional<uint32_t> timeout;

  std::optional<Output> output;
  std::list<Command> commands;
  std::optional<ResponseOn> responseOn;
};
} // namespace config
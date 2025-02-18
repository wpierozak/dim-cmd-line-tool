#pragma once

#include <string>
#include <vector>
#include <rapidxml.hpp>

namespace config {

namespace xml
{

struct Command {
    std::string name;
    std::string file;  // optional attribute (e.g., for file-based commands)
    std::string text;  // inline text if provided
};

struct ResponseOnService {
    std::string alias;
    std::string name;
};

struct ServiceCommand {
    std::vector<Command> commands;
};

struct CommandService {
    ServiceCommand commands;
    std::vector<ResponseOnService> responseOn;
};

struct Output {
    std::string filePath;
    bool hideTerminal = false;
};

struct ServiceInfo {
    Output output;
};

struct RpcInfo {
    Output output;
};

struct Service {
    std::string type;
    std::string name;
    std::string alias;
    // Only one of the following will be used based on type:
    CommandService commandService;  // for type="command"
    ServiceInfo serviceInfo;          // for type="service_info"
    RpcInfo rpcInfo;                  // for type="rpc_info"
};

struct Send {
    std::string serviceAlias;
    std::string commandName;
    bool responsePrint = false;
};

struct Task {
    std::string name;
    std::string type; // empty for a normal task, or "indefinite"/"iterative"
    int period = 0;      // for periodic tasks
    int iterations = 0;  // for iterative tasks
    std::vector<Send> sends;
    int waitTime = 0;    // optional wait element
};

class XmlParser {
public:
    XmlParser();
    ~XmlParser();

    // Parse the XML file given its filename.
    // Returns true if parsing succeeds.
    bool parseFile(const std::string &filename);

    // Getters for the parsed data.
    const std::vector<Service>& getServices() const;
    const std::vector<Task>& getTasks() const;

private:
    // Helper functions to extract attributes and parse nodes.
    std::string getAttribute(rapidxml::xml_node<>* node, const char* attrName, std::string defaultValue = "");
    Service parseService(rapidxml::xml_node<>* node);
    Task parseTask(rapidxml::xml_node<>* node);

    // Storage for the parsed data.
    std::vector<Service> services;
    std::vector<Task> tasks;
};
}
} // namespace config

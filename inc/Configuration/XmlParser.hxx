#pragma once

#include <string>
#include <vector>
#include <rapidxml.hpp>

namespace config {

namespace xml
{

/**
 * @brief Represents a command to be executed.
 */
struct Command {
    std::string name;   /**< The name of the command. */
    std::string file;   /**< Optional attribute: file path for file-based commands. */
    std::string text;   /**< Inline text for the command, if provided. */
};

/**
 * @brief Represents a response on a service.
 */
struct ResponseOnService {
    std::string alias;  /**< Alias of the service. */
    std::string name;   /**< Name of the service. */
};

/**
 * @brief Represents a service command, containing a list of commands.
 */
struct ServiceCommand {
    std::vector<Command> commands;  /**< Vector of commands for this service. */
};

/**
 * @brief Represents a command service, including commands and responses.
 */
struct CommandService {
    ServiceCommand commands;                  /**< Service command containing the list of commands. */
    std::vector<ResponseOnService> responseOn; /**< Vector of responses on this service. */
};

/**
 * @brief Represents output configuration.
 */
struct Output {
    std::string filePath;    /**< File path for output. */
    bool hideTerminal = false; /**< Flag to hide terminal output (default: false). */
};

/**
 * @brief Represents service information.
 */
struct ServiceInfo {
    Output output; /**< Output configuration for the service. */
};

/**
 * @brief Represents RPC information.
 */
struct RpcInfo {
    Output output; /**< Output configuration for the RPC. */
};

/**
 * @brief Represents a service with different types and configurations.
 */
struct Service {
    std::string type;   /**< Type of the service (e.g., "command", "service_info", "rpc_info"). */
    std::string name;   /**< Name of the service. */
    std::string alias;  /**< Alias of the service. */
    // Only one of the following will be used based on type:
    CommandService commandService;  /**< Command service configuration for type="command". */
    ServiceInfo serviceInfo;          /**< Service information configuration for type="service_info". */
    RpcInfo rpcInfo;                  /**< RPC information configuration for type="rpc_info". */
};

/**
 * @brief Represents a send action to a service command.
 */
struct Send {
    std::string serviceAlias;    /**< Alias of the service to send the command to. */
    std::string commandName;     /**< Name of the command to send. */
    bool responsePrint = false;  /**< Flag to print the response (default: false). */
};

/**
 * @brief Represents a task to be executed.
 */
struct Task {
    std::string name;           /**< Name of the task. */
    std::string type;           /**< Type of the task ("" for normal, "indefinite", "iterative"). */
    int period = 0;                /**< Period for periodic tasks. */
    int iterations = 0;            /**< Number of iterations for iterative tasks. */
    std::vector<Send> sends;       /**< Vector of send actions for this task. */
    int waitTime = 0;            /**< Optional wait time after the task. */
};

/**
 * @brief Class to parse XML configuration files.
 */
class XmlParser {
public:
    /**
     * @brief Constructor for the XmlParser class.
     */
    XmlParser();

    /**
     * @brief Destructor for the XmlParser class.
     */
    ~XmlParser();

    /**
     * @brief Parses the XML file given its filename.
     * @param filename The name of the XML file to parse.
     * @return True if parsing succeeds, false otherwise.
     */
    bool parseFile(const std::string &filename);

    /**
     * @brief Gets the list of parsed services.
     * @return A constant reference to the vector of services.
     */
    const std::vector<Service>& getServices() const;

    /**
     * @brief Gets the list of parsed tasks.
     * @return A constant reference to the vector of tasks.
     */
    const std::vector<Task>& getTasks() const;

private:
    /**
     * @brief Helper function to extract an attribute from an XML node.
     * @param node The XML node to extract the attribute from.
     * @param attrName The name of the attribute to extract.
     * @param defaultValue The default value to return if the attribute is not found.
     * @return The value of the attribute, or the default value if not found.
     */
    std::string getAttribute(rapidxml::xml_node<>* node, const char* attrName, std::string defaultValue = "");

    /**
     * @brief Helper function to parse a service node.
     * @param node The XML node representing the service.
     * @return The parsed Service object.
     */
    Service parseService(rapidxml::xml_node<>* node);

    /**
     * @brief Helper function to parse a task node.
     * @param node The XML node representing the task.
     * @return The parsed Task object.
     */
    Task parseTask(rapidxml::xml_node<>* node);

    /**
     * @brief Storage for the parsed services.
     */
    std::vector<Service> services;

    /**
     * @brief Storage for the parsed tasks.
     */
    std::vector<Task> tasks;
};
}
} // namespace config
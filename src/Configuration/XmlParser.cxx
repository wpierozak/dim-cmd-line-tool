// #include "Configuration/XmlParser.hxx"

// #include "rapidxml_utils.hpp"
// #include <cstdlib>
// #include <iostream>
// #include <exception>

// namespace config {
// namespace xml
// {
// XmlParser::XmlParser() {
//     // Constructor (initialize if needed)
// }

// XmlParser::~XmlParser() {
//     // Destructor (cleanup if necessary)
// }

// // Helper function to get an attribute value from a node.
// std::string XmlParser::getAttribute(rapidxml::xml_node<>* node, const char* attrName,  std::string defaultValue) {
//     if (rapidxml::xml_attribute<>* attr = node->first_attribute(attrName)){
//         return attr->value();
//     }
//     return defaultValue;
// }

// // Parses a <Service> node.
// Service XmlParser::parseService(rapidxml::xml_node<>* node) {
//     Service service;
//     service.type  = getAttribute(node, "type");
//     service.name  = getAttribute(node, "name");
//     service.alias = getAttribute(node, "alias");

//     // Process child nodes of <Service>
//     for (rapidxml::xml_node<>* child = node->first_node(); child; child = child->next_sibling()) {
//         std::string childName = child->name();

//         if (childName == "commands") {
//             // Process the <commands> node.
//             for (rapidxml::xml_node<>* cmdNode = child->first_node("command"); cmdNode; cmdNode = cmdNode->next_sibling("command")) {
//                 Command cmd;
//                 cmd.name = getAttribute(cmdNode, "name");
//                 cmd.file = getAttribute(cmdNode, "file");
//                 // Capture inline text if present.
//                 if (cmdNode->value())
//                     cmd.text = cmdNode->value();
//                 service.commandService.commands.push_back(cmd);
//             }
//         }
//         else if (childName == "response_on") {
//             // Process the <response_on> node.
//             for (rapidxml::xml_node<>* respNode = child->first_node("service"); respNode; respNode = respNode->next_sibling("service")) {
//                 ResponseOnService resp;
//                 resp.alias = getAttribute(respNode, "alias");
//                 resp.name  = getAttribute(respNode, "name");
//                 service.commandService.responseOn.push_back(resp);
//             }
//         }
//         else if (childName == "output") {
//             // Process the <output> node for service_info or rpc_info.
//             Output out;
//             if (rapidxml::xml_node<>* fileNode = child->first_node("file"))
//                 out.filePath = getAttribute(fileNode, "path");
//             if (rapidxml::xml_node<>* termNode = child->first_node("terminal")) {
//                 std::string hide = getAttribute(termNode, "hide");
//                 out.hideTerminal = (hide == "true" || hide == "1");
//             }
//             if (service.type == "service_info")
//                 service.serviceInfo.output = out;
//             else if (service.type == "rpc_info")
//                 service.rpcInfo.output = out;
//         }
//     }
//     return service;
// }

// // Parses a <Task> node.
// Task XmlParser::parseTask(rapidxml::xml_node<>* node) {
//     Task task;
//     task.name = getAttribute(node, "name");
//     task.type = getAttribute(node, "type");

//     std::string periodStr = getAttribute(node, "period");
//     std::string iterStr   = getAttribute(node, "iterations");
//     if (!periodStr.empty())
//         task.period = std::atoi(periodStr.c_str());
//     if (!iterStr.empty())
//         task.iterations = std::atoi(iterStr.c_str());

//     // Process child nodes of <Task>.
//     for (rapidxml::xml_node<>* child = node->first_node(); child; child = child->next_sibling()) {
//         std::string childName = child->name();
//         if (childName == "send") {
//             Send send;
//             if (rapidxml::xml_node<>* svcNode = child->first_node("service"))
//                 send.serviceAlias = getAttribute(svcNode, "alias");
//             if (rapidxml::xml_node<>* cmdNode = child->first_node("command"))
//                 send.commandName = getAttribute(cmdNode, "name");
//             if (rapidxml::xml_node<>* respNode = child->first_node("response"))
//                 send.responsePrint = (getAttribute(respNode, "print") == "true" ||
//                                       getAttribute(respNode, "print") == "1");
//             task.sends.push_back(send);
//         }
//         else if (childName == "wait") {
//             std::string waitTimeStr = getAttribute(child, "time");
//             if (!waitTimeStr.empty())
//                 task.waitTime = std::atoi(waitTimeStr.c_str());
//         }
//     }
//     return task;
// }

// // Parses the XML file specified by filename.
// bool XmlParser::parseFile(const std::string &filename) {
//     try {
//         // Read the file into a RapidXML file buffer.
//         rapidxml::file<> xmlFile(filename.c_str());
//         rapidxml::xml_document<> doc;
//         doc.parse<0>(xmlFile.data());

//         rapidxml::xml_node<>* root = doc.first_node();
//         if (!root) {
//             std::cerr << "No root node found!" << std::endl;
//             return false;
//         }

//         // Clear previous parsed data.
//         services.clear();
//         tasks.clear();

//         // Iterate over the children of the root node.
//         for (rapidxml::xml_node<>* node = root->first_node(); node; node = node->next_sibling()) {
//             std::string nodeName = node->name();
//             if (nodeName == "Service") {
//                 Service srv = parseService(node);
//                 services.push_back(srv);
//             }
//             else if (nodeName == "Task") {
//                 Task task = parseTask(node);
//                 tasks.push_back(task);
//             }
//         }
//     }
//     catch (std::exception &e) {
//         std::cerr << "Exception during XML parsing: " << e.what() << std::endl;
//         return false;
//     }
//     return true;
// }

// // Returns the parsed Service nodes.
// const std::vector<Service>& XmlParser::getServices() const {
//     return services;
// }

// // Returns the parsed Task nodes.
// const std::vector<Task>& XmlParser::getTasks() const {
//     return tasks;
// }
// }
// } // namespace config

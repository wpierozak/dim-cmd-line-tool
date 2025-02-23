#include <Configuration/ServiceInfo.hxx>
#include <cstring>
#include <stdexcept>

// ServiceInfo::Output constructor
config::ServiceInfo::Output::Output(rapidxml::xml_node<>* node) {
    // Look for a child <file> node and read its "path" attribute
    rapidxml::xml_node<>* file_node = node->first_node("file");
    if (file_node) {
        rapidxml::xml_attribute<>* path_attr = file_node->first_attribute("path");
        if (path_attr)
            filename = path_attr->value();
    }
}

config::ServiceInfo::Command::Command(rapidxml::xml_node<>* node) {
    // Read required "name" attribute
    if (auto name_attr = node->first_attribute("name")){
        name = name_attr->value();
    }else{
        throw std::runtime_error("Missing required attribute 'name' in command");
    }

    // Read optional "file" attribute
    if (auto file_attr = node->first_attribute("file")){
        file = file_attr->value();
    }

    // If no file attribute, check if the node has text content to use as command
    const char* node_text = node->value();
    if (node_text && *node_text != '\0'){
        command = node_text;
    }
}

// ServiceInfo::Commands constructor
config::ServiceInfo::Commands::Commands(rapidxml::xml_node<>* node) {
    for (rapidxml::xml_node<>* cmd = node->first_node("command"); cmd; cmd = cmd->next_sibling("command")){
        commands.emplace_back(cmd);
    }
}

config::ServiceInfo::ServiceInfo(rapidxml::xml_node<>* node) {
    if (auto name_attr = node->first_attribute("name"))
        name = name_attr->value();
    else{
        throw std::runtime_error("Missing required attribute 'name' in service");
    }

    if (auto type_attr = node->first_attribute("type"))
        type = type_attr->value();
    else{
        throw std::runtime_error("Missing required attribute 'type' in service");
    }

    if (auto alias_attr = node->first_attribute("alias")){
        alias = alias_attr->value();
    }

    if(auto timeout_ptr = node->first_attribute("timeout")){
        timeout = std::stoi(timeout_ptr->value());
    }

    // Process child nodes
    for (rapidxml::xml_node<>* child = node->first_node(); child; child = child->next_sibling()) {
        if (std::strcmp(child->name(), "output") == 0) {
            output = Output(child);
        }
        else if (std::strcmp(child->name(), "commands") == 0) {
            // Append all commands from the <commands> section
            Commands cmds(child);
            commands.insert(commands.end(), cmds.commands.begin(), cmds.commands.end());
        }
        else if (std::strcmp(child->name(), "response_on") == 0) {
            ResponseOn resp;
            for (rapidxml::xml_node<>* srv = child->first_node("service"); srv; srv = srv->next_sibling("service")) {
                // Trim whitespace if necessary
                const char* text = srv->value();
                if (text && *text != '\0')
                    resp.services.push_back(text);
            }
            responseOn = resp;
        }
    }
}

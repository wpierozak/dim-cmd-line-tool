#include<string>

namespace dim_handlers
{
class Service
{
public:
    enum class Type{Command, ServiceInfo, RpcInfo};

    Service(std::string name, std::string alias, Type type)
        : m_name(name), m_alias(alias), m_type(type) {}

    std::string getName() const { return m_name; }
    std::string getAlias() const { return m_alias; }
    Type getType() const { return m_type; }

private:
    std::string m_name;
    std::string m_alias;
    Type m_type;
};

}
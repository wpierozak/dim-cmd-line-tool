#include"Service.hxx"
#include<dic.hxx>

namespace dim_handlers
{
class ServiceInfo: public Service, public DimInfo
{
public:
void infoHandler() override {
    m_serviceData.emplace_back(getString());
}

private:
    std::list<std::string> m_serviceData;
    std::optional<std::string> m_fileName;
};

}
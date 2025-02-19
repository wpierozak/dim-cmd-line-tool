#pragma once

#include"Subscriber.hxx"
#include<dim/dic.hxx>
#include<thread>

namespace dim_handlers
{
class ServiceInfo: public Subscriber, public DimInfo
{
public:
    ServiceInfo(std::string name, std::string alias)
        : Subscriber(name, alias, Type::ServiceInfo), DimInfo(const_cast<char*>(name.c_str()), const_cast<char*>(SERVICE_NO_LINK.data())){}

    void infoHandler() override {
        std::string newData = getString();
        if(newData != SERVICE_NO_LINK){
            handleNewData(newData);
        }
    }

    std::optional<std::string> waitForData() override {
        auto startTime = std::chrono::high_resolution_clock::now();
        while(availableData() == 0){
            std::this_thread::sleep_for(std::chrono::milliseconds(1));
            if(checkTimeout(startTime, std::chrono::high_resolution_clock::now())){
                return std::nullopt;
            }
        }
        return {Subscriber::getData()};
    }
private:
    static constexpr std::string_view SERVICE_NO_LINK = "<!>NO_LINK<!>";
};

}
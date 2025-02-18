#include"Subscriber.hxx"
#include<dic.hxx>
#include<thread>

namespace dim_handlers
{
class ServiceInfo: public Subscriber, public DimInfo
{
public:
    ServiceInfo(std::string name, std::string alias)
        : Subscriber(name, alias, Type::ServiceInfo) {}

    void infoHandler() override {
        std::string newData = getString();
        handleNewData(newData);
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
};

}
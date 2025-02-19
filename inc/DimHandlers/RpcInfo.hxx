#pragma once

#include<dim/dic.hxx>
#include"Subscriber.hxx"

namespace dim_handlers
{
class RpcInfo: public Subscriber, public DimRpcInfo
{
public:
    RpcInfo(std::string name, std::string alias)
        : Subscriber(name, alias, Type::RpcInfo), 
        DimRpcInfo(const_cast<char*>(name.c_str()), const_cast<char*>(RpcInfo::RPC_NO_LINK.data())) 
        {}

    RpcInfo(std::string name, std::string alias, uint32_t timeout)
        : Subscriber(name, alias, Type::RpcInfo, timeout), 
        DimRpcInfo(const_cast<char*>(name.c_str()), timeout, const_cast<char*>(RpcInfo::RPC_NO_LINK.data())) 
        {}

    std::optional<std::string> waitForData() override {
        auto startTime = std::chrono::high_resolution_clock::now();
        std::string newData = getString();
        if(newData == RpcInfo::RPC_NO_LINK){
            return std::nullopt;
        }else{
            handleNewData(newData);
        }
        return {Subscriber::getData()};
    }
private:
    static constexpr std::string_view RPC_NO_LINK = "<!>NO_LINK<!>";
};
} // namespace dim_handlers
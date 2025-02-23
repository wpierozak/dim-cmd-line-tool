#include<DimHandlers/DimManager.hxx>
#include<iostream>
#include<Logging/Logger.hxx>
#include<Configuration/XmlParser.hxx>
#include<chrono>
void test_sub();
void test_cmd();

using std::chrono_literals::operator""ms;

int main()
{
    config::XmlParser parser;
    bool success = parser.load("/home/wiktor/dim-cmd-line-tool/config/example.xml");
    if(!success){
        LOG(ERROR) << "FAILURE";
        return 0;
    }
    while(true){
        std::this_thread::sleep_for(1000ms);
        dim_handlers::DimManager::Instance().executeKnownCommand("ALF/SERIAL_0/ENDPOINT_0/LINK_0/SWT_SEQUENCE/RpcIn", "read");
    }
    return 0;
}

void test_sub()
{
    int numServices;
    std::cout << "Enter number of services to subscribe: ";
    std::cin >> numServices;

    std::vector<std::string> services(numServices);
    std::vector<std::string> aliases(numServices);
    std::vector<std::string> files(numServices);

    for (int i = 0; i < numServices; ++i) {
        std::cout << "Service " << i + 1 << ": ";
        std::cin >> services[i];
        std::cout << "Alias " << i + 1 << ": ";
        std::cin >> aliases[i];
        std::cout << "File " << i + 1 << ": ";
        std::cin >> files[i];
    }
    for (int i = 0; i < numServices; ++i) {
        dim_handlers::DimManager::Instance().createSubscriber(services[i], aliases[i], files[i], dim_handlers::Subscriber::Type::ServiceInfo);
    }

    LOG(INFO) << "Waiting for data";

    for (int i = 0; i < 100; ++i) {
       std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    }
    std::cout << std::endl;
}

void test_cmd()
{
//std::cout << "Service IN: ";
std::string sIN = "ALF/SERIAL_0/ENDPOINT_0/LINK_0/SWT_SEQUENCE/RpcIn";
std::string sIN_B = "ALF/SERIAL_0/ENDPOINT_0/LINK_1/SWT_SEQUENCE/RpcIn";
//std::cin >> sIN;

//std::cout << "Service OUT: ";
std::string sOUT = "ALF/SERIAL_0/ENDPOINT_0/LINK_0/SWT_SEQUENCE/RpcOut";
std::string sOUT_B = "ALF/SERIAL_0/ENDPOINT_0/LINK_1/SWT_SEQUENCE/RpcOut";
//std::cin >> sOUT;
LOG(INFO) << "LOG DEBUG";
Logger::Get().logToFile("logs");

DIM_MANAGER.createSubscriber(sOUT, "LINK_0", "link_0", dim_handlers::Service::Type::ServiceInfo);
DIM_MANAGER.createCommandSender(sIN, sIN);
DIM_MANAGER.addResponseServiceToCommandSender(sIN,"LINK_0");
DIM_MANAGER.addResponseServiceToCommandSender(sIN,sOUT_B);
auto res = DIM_MANAGER.addCommandFromFile(sIN, "FILE", "/home/wiktor/dim-cmd-line-tool/build/cmd");

DIM_MANAGER.createSubscriber(sOUT_B, "LINK_1", "link_1", dim_handlers::Service::Type::ServiceInfo);
DIM_MANAGER.createCommandSender(sIN_B, sIN_B);
DIM_MANAGER.addResponseServiceToCommandSender(sIN_B,"LINK_1");
DIM_MANAGER.addCommandFromFile(sIN, "FILE", "/home/wiktor/dim-cmd-line-tool/build/cmd");

if(res.isError()){
    LOG(ERROR) << res.error.value();
}
for(int i = 0; i < 1; i++)
{
    auto res = DIM_MANAGER.executeKnownCommand(sIN, "FILE",true);
    if(res.isError()){
        LOG(ERROR) << res.error.value();
    }else{
        LOG(INFO) << "Command send successfully";
    } if(res.result.has_value()){
        LOG(INFO) << "Result: " << res.result.value();
    }
}
std::cout << Logger::Get().getQuietLogs();
std::cout << std::endl;
}
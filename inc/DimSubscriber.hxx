#include <dim/dic.hxx>
#include <string>
#include <list>
#include <iostream>
#include <iomanip>
#include<fstream>
#include <unistd.h>
#include <sstream>
#include <chrono>
#include <ctime>
#include<algorithm>
#include <csignal>



class DimSubscriber : virtual public DimInfo {
public:

    static inline std::string Empty = "Empty";

    DimSubscriber(std::string name)
        : DimInfo(name.c_str(), const_cast<char*>(Empty.data())), m_serviceName(name)
    {
        
    }

    void infoHandler() override {
        m_serviceData.emplace_back(getString());
    }

    std::string getServiceName() {
        return m_serviceName;
    }

    bool isData() {
        return !m_serviceData.empty();
    }

    std::string getData() {
        std::string data = m_serviceData.front();
        m_serviceData.pop_front();
        return data;
    }

private:
    std::string m_serviceName;
    std::list<std::string> m_serviceData;
};
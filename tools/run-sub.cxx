#include<DimHandlers/DimManager.hxx>
#include<iostream>
#include<Logging/Logger.hxx>
#include<Configuration/XmlParser.hxx>
#include<chrono>

using std::chrono_literals::operator""ms;

int main(int argc, const char** argv)
{
    config::XmlParser parser;
    bool success = parser.load(argv[1]);
    if(!success){
        LOG(ERROR) << "Aborting";
        return 0;
    }
    while(true){
        
    }
   return 0;
}
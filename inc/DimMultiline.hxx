#include<dic.hxx>
#include<string>
#include<optional>
#include<cstdint>
#include<fstream>
#include<sstream>
#include<Logging/Logger.h>
#include<Object.hxx>

class DimMultiline: public Object
{
public:
enum class Mode{Indefinite, Iterative, Single};
    DimMultiline(const std::string& serviceName, const std::string& fileName, Mode mode, std::optional<uint32_t> loopParam = std::nullopt):
    m_serviceName(serviceName), m_sourceFileName(fileName), m_mode(mode), m_loopParam(loopParam)
    {
        std::ifstream file;
        file.open(m_sourceFileName.c_str());
        if(file.is_open() == false){
            LOG(ERROR) << "Failure: cannot open file: " << m_sourceFileName << std::endl;
            changeState(State::Error);
        }

        if(m_mode == Mode::Iterative){
            if(loopParam.has_value() == false){
                LOG(ERROR) << "Failure: Iterative mode requires number of iterations" << std::endl;
            }
            changeState(State::Error);
        }

        if(m_mode == Mode::Indefinite){
            if(loopParam.has_value() == false){
                LOG(ERROR) << "Failure: Indefinite mode requires sleep period" << std::endl;
            }
            changeState(State::Error);
        }

        std::stringstream sstr;
        sstr << file.rdbuf();
        m_request = sstr.str();
        
        file.close();
    }

    bool execute()
    {
        do{
            DimClient::sendCommand(argv[1],request.c_str());
            printTime("Sent command");
            usleep(interval);
        }while(periodic);
    }

private:
    std::string m_serviceName;

    std::string m_sourceFileName;
    std::string m_request;
    
    Mode m_mode;
    std::optional<uint32_t> m_loopParam;
};
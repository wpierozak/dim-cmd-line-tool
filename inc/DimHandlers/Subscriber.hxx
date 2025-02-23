#pragma once

#include<vector>
#include<functional>
#include<list>
#include<fstream>
#include<optional>
#include<cstdint>
#include<chrono>
#include"Service.hxx"

namespace dim_handlers
{
/**
 * @class Subscriber
 * @brief Represents service subscribing to either a DIM service or DIM RPC.
 */
class Subscriber: public Service
{
public:
/**
 * @brief Constructs a new Subscriber object.
 * 
 * @param name The name of the subscriber.
 * @param alias The alias of the subscriber.
 * @param type The type of the subscriber.
 * @param timeout Optional timeout value in milliseconds. Default is std::nullopt.
 */
    Subscriber(std::string name, std::optional<std::string> alias, Type type, std::optional<uint32_t> timeout = std::nullopt)
        : Service(name, alias, type), m_timeout(timeout) {
            
        }

    virtual ~Subscriber() 
    { 
        closeFile(); 
    }
/**
 * @brief Returns the number of available values fetched from the service since last clearData() call.
 * @return Number of available values.
 */
    uint32_t availableData() const { return m_serviceData.size(); }
/**
 * @brief Clears the data fetched from the service.
 * @return Number of cleared values.
 */
    uint32_t clearData() { 
        uint32_t size = m_serviceData.size(); 
        m_serviceData.clear(); 
        return size; 
    }
/**
 * @brief Returns the latest data fetched from the service.
 * @return Latest data as a string.
 */
    std::optional<std::string> getData() { 
        if(m_serviceData.empty()){
            return std::nullopt;
        }
        return m_serviceData.front(); 
    }

    virtual std::optional<std::string> waitForData() = 0;
/**
 * @brief Removes the latest data fetched from the service.
 */
    void popFront() { m_serviceData.pop_front(); }


/**
 * @enum TerminalOutput
 * @brief Specifies the terminal output to be printed.
 * @var TerminalOutput::Latest
 * Print the latest data.
 * @var TerminalOutput::All
 * Print all data.
 */
    enum class TerminalOutput{Latest, All};
/**
 * @brief Returns whether the terminal output is hidden by default.
 * @return True if terminal output is hidden, false otherwise.
 */
    bool hideTerminal() const { return m_hideTerminal; }
/**
 * @brief Changes default terminal output visibility.
 * @param hide True to hide terminal output, false to show.
 */
    void hideTerminal(bool hide) { m_hideTerminal = hide; }

/**
 * @brief Prints the terminal output.
 * @param output Specifies the terminal output to be printed. Default is TerminalOutput::Latest.
 */
    void print(TerminalOutput output = TerminalOutput::Latest);



/**
 * @brief Returns the file name to which the output is saved.
 * @return Optional file name as a string.
 */
    std::optional<std::string> getFileName() const { return m_fileName; }
/**
 * @brief Returns whether the output is saved to a file.
 * @return True if output is saved to a file, false otherwise.
 */
    bool isFile() const { return m_file.has_value(); }
/**
 * @brief Sets the file name to which the output is saved.
 * @param fileName The file name as a string.
 */
    bool setFile(const std::string &fileName);
/**
 * @brief Saves the output to a file.
 * @param output The output to be saved as a string.
 * @return True if the output is successfully saved, false otherwise.
 */
    bool saveToFile(const std::string& output);


/**
 * @brief Handles new data fetched from the service.
 * @param data The new data as a string.
 * @return True if the data is successfully handled, false otherwise.
 */
    virtual bool handleNewData(const std::string &data);

protected:
    bool checkTimeout(const std::chrono::high_resolution_clock::time_point& startTime, std::chrono::high_resolution_clock::time_point currentTime)
    {
        if(m_timeout.has_value() == false){
            return false;
        }
        return std::chrono::duration_cast<std::chrono::milliseconds>(currentTime - startTime).count() > m_timeout.value();
    }
    uint32_t timeout()
    {
        return m_timeout.value_or(0);
    }

private:
    bool openFile();
    void closeFile()
    {
        if(m_file.has_value()){
            m_file->close();
            m_file.reset();
        }
    }
    
    std::list<std::string> m_serviceData;
    std::optional<std::string> m_fileName;
    std::optional<std::ofstream> m_file;
    std::optional<uint32_t> m_timeout;
    bool m_hideTerminal = false;
};
} // namespace dim_handlers
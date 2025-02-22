#pragma once
#include<iostream>
#include<fstream>
#include<sstream>
#include<optional>
#include<mutex>

class StreamMonitor
{
public:

StreamMonitor(std::ostream& stream): m_current(stream)
{
}

template<typename T>
StreamMonitor& operator<<(const T& message)
{
    const std::lock_guard<std::mutex> guard(m_lock);
    m_current << message;
    return *this;
}

StreamMonitor& operator<<(const char* message)
{
    const std::lock_guard<std::mutex> guard(m_lock);
    m_current << message;
    return *this;
}

StreamMonitor& operator<<(uint32_t value)
{
    const std::lock_guard<std::mutex> guard(m_lock);
    m_current << value;
    return *this;
}

std::string str() const
{
    const std::lock_guard<std::mutex> guard(m_lock);
    std::stringstream tmp;
    tmp << m_current.rdbuf();
    return tmp.str();
}

void flush()
{
    m_current.flush();
}

private:
    std::ostream& m_current;
    mutable std::mutex m_lock;
};

template<typename StreamType>
class StreamOwner
{
public:
    StreamMonitor& monitor()
    {
        return m_monitor;
    }

    StreamMonitor& operator()()
    {
        return m_monitor;
    }

protected:
    StreamType m_fileStream;
    StreamMonitor m_monitor{m_fileStream};
};

class FileStream: public StreamOwner<std::ofstream>
{
public:
    FileStream(const std::string& fileName)
    {
        m_fileStream.open(fileName);
    }

    bool isOpen()
    {
        return m_fileStream.is_open();
    }
};

typedef StreamOwner<std::stringstream> StringStream;
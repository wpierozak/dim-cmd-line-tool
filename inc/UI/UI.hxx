// filepath: /home/wiktor/dim-cmd-line-tool/inc/UI/UI.hxx
#pragma once
#include <thread>
#include <atomic>
#include <chrono>
#include <functional>

class UI
{
private:
void handleKeyboardEvent(char c)
{
    
}
struct CommandOutput
{
    std::vector<std::string> lines;
    uint32_t currentPosition;
} m_curretnCommandOutput;

};
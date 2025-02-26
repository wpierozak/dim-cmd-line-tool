// filepath: /home/wiktor/dim-cmd-line-tool/inc/UI/UI.hxx
#pragma once
#include"UIObjects.hxx"
#include"UITools.hxx"

namespace ui
{
class Manager
{
public:
    void runUI();
private:

    ui::types::MultiLineText getOutput();
    std::string getServiceInfo(const std::string& name);
    std::string getLogs();
    void updateCommand();
    void enterClicked();

    int m_lastMainSelected;
    int m_lastServiceSelected;
    int m_lastCommandSelected;
};
}
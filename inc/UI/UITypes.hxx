#pragma once
#include <functional>  // for function
#include <memory>      // for shared_ptr, allocator, __shared_ptr_access
#include <string>      // for string, basic_string
#include <vector>      // for vector
 
#include "ftxui/component/captured_mouse.hpp"  // for ftxui
#include "ftxui/component/component.hpp"  // for Slider, Checkbox, Vertical, Renderer, Button, Input, Menu, Radiobox, Toggle
#include "ftxui/component/component_base.hpp"  // for ComponentBase
#include "ftxui/component/screen_interactive.hpp"  // for Component, ScreenInteractive
#include "ftxui/dom/elements.hpp"  // for separator, operator|, Element, size, xflex, text, WIDTH, hbox, vbox, EQUAL, border, GREATER_THAN
 
#include"UITools.hxx"
#include<DimHandlers/DimManager.hxx>

namespace ui
{
namespace types
{
    struct Menu
    {
      Menu()
      {
        component = ftxui::Menu(&entries, &selected);
      }
  
      std::vector<std::string> entries;
      int selected;
      ftxui::Component component;
    };

    struct Input
    {
        Input()
        {
            component = tools::Wrap("Input", ftxui::Input(&content,""));
        }
        std::string content;
        ftxui::Component component;
    };
    
    class MultiLineText
    {
        public:
        MultiLineText(const std::string& text, size_t lines = std::string::npos)
        {
            if(lines == std::string::npos){
                m_lines = tools::split_string_by_newline(text);
            }else{
                m_lines = tools::get_last_n_lines(text, lines);
            }
        }

        ftxui::Element Render()
        {
            ftxui::Elements elements;
            for (const auto& line : m_lines) {
                elements.push_back(ftxui::paragraph(line));
              }
            return ftxui::vbox(elements);
        }

        private:
        std::vector<std::string> m_lines;

    };

    class Command
    {
        public:
        enum class State{Active,Ready,Waiting,Finished,Failure};
        enum class Type{Known, Input};

        Command(const std::string& cmdSender, Type type_): 
            type(type_), m_state(State::Ready),
            m_commandSender(cmdSender)
        {

        }

        State state(){ return m_state;}

        void moveReady(const std::string& command)
        {
            m_command = command;
        }

        void moveWaiting()
        {
            m_state = State::Waiting;
            std::thread task([&]{
                utils::Result<std::string,std::string> res;
                if(type == Type::Input){
                    res = DIM_MANAGER.executeCommand(m_commandSender,m_commandSender,true);
                } else{
                    res = DIM_MANAGER.executeKnownCommand(m_commandSender,m_commandSender,true);
                }
                if(res.isError()){
                    m_state = State::Failure;
                    m_errorMessage = res.error.value_or("");
                } else{
                    m_state = State::Finished;
                    m_response = res.result.value_or("");
                }
            });
        }

        void moveFinished()
        {
            utils::Result<std::string,std::string> res;
            if(type == Type::Input){
                res = DIM_MANAGER.executeCommand(m_commandSender,m_commandSender,false);
            } else{
                res = DIM_MANAGER.executeKnownCommand(m_commandSender,m_commandSender,false);
            }
            if(res.isError()){
                m_state = State::Failure;
                m_errorMessage = res.error.value_or("Failed to executed command");
            } else{
                m_state = State::Finished;
            }
        }

        std::optional<std::string> response()
        {
            return m_response;
        }

        std::optional<std::string> error()
        {
            return m_errorMessage;
        }

        const Type type;
        private:
        
        State m_state;
        const std::string m_commandSender;
        std::string m_command;

        std::optional<std::string> m_response;
        std::optional<std::string> m_errorMessage;
    };
}
}
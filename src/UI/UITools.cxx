#include<UI/UITools.hxx>
#include<sstream>

namespace ui
{
namespace tools
{
    std::vector<std::string> get_last_n_lines(const std::string &text, size_t n) 
    {
      std::vector<std::string> lines;
      size_t start = 0;
      size_t pos = 0;
      while ((pos = text.find('\n', start)) != std::string::npos) {
        lines.push_back(text.substr(start, pos - start));
        start = pos + 1;
      }
      if (start < text.size())
        lines.push_back(text.substr(start));
    
      size_t begin = lines.size() > n ? lines.size() - n : 0;
      std::vector<std::string> result(lines.begin() + begin, lines.end());
      return result;
    }

    std::vector<std::string> split_string_by_newline(const std::string& str)
    {
        auto result = std::vector<std::string>{};
        auto ss = std::stringstream{str};

        for (std::string line; std::getline(ss, line, '\n');)
        result.push_back(line);

        return result;
    }

    ftxui::Component Wrap(std::string name, ftxui::Component component) {
        return ftxui::Renderer(component, [name, component] {
          return ftxui::hbox({
            ftxui::text(name) | size(ftxui::WIDTH, ftxui::EQUAL, 8),
            ftxui::separator(),
                     component->Render() | ftxui::xflex,
                 }) |
                 ftxui::xflex;
        });
      }
}
}
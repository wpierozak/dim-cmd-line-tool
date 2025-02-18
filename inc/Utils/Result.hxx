#include<optional>
namespace utils
{
template<typename ResultType>
struct Result
{
    std::optional<ResultType> result;
    std::optional<std::string> error;
};
} // namespace utils

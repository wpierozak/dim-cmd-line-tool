#include<optional>
#include<cstdint>
namespace utils
{
template<typename ResultType, typename ErrorType>
struct Result
{
    std::optional<ResultType> result = std::nullopt;
    std::optional<ErrorType> error = std::nullopt;
};

} // namespace utils

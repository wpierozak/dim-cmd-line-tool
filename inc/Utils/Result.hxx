#include<optional>
#include<cstdint>
namespace utils
{
template<typename ResultType, typename ErrorType>
struct Result
{
    std::optional<ResultType> result = std::nullopt;
    std::optional<ErrorType> error = std::nullopt;

    operator ResultType()
    {
        if(result.has_value() == false){
            throw std::runtime_error("Result is empty.");
        }
        return result.value();
    }
};

} // namespace utils

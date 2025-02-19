#pragma once

#include<optional>
#include<cstdint>
namespace utils
{
template<typename ResultType, typename ErrorType>
struct Result
{
    std::optional<ResultType> result = std::nullopt;
    std::optional<ErrorType> error = std::nullopt;

    operator bool()
    {
        return result.has_value();
    }
};

} // namespace utils

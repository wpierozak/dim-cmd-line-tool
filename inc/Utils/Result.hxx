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

    bool isError() const
    {
        return error.has_value();
    }
};

} // namespace utils

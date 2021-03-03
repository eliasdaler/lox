#pragma once

#include <any>
#include <stdexcept>

namespace Lox
{
class ReturnException : public std::runtime_error {
public:
    ReturnException(std::any value) : std::runtime_error(""), value(value) {}

    const std::any& getValue() const { return value; }

private:
    std::any value;
};

} // end of namespace Lox

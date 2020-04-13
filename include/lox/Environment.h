#pragma once

#include <any>
#include <string>
#include <unordered_map>

namespace Lox
{
class Token;

class Environment {
public:
    void define(const std::string& name, const std::any& value);

    const std::any& get(const Token& name) const;

private:
    std::unordered_map<std::string, std::any> values;
};
} // namespace Lox

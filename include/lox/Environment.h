#pragma once

#include <any>
#include <string>
#include <unordered_map>

namespace Lox
{
class Token;

class Environment {
public:
    Environment();
    Environment(Environment* enclosing);
    void define(const std::string& name, const std::any& value);
    void assign(const Token& name, const std::any& value);

    const std::any& get(const Token& name) const;

private:
    Environment* enclosing;
    std::unordered_map<std::string, std::any> values;
};
} // namespace Lox

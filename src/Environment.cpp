#include "lox/Environment.h"

#include "lox/RuntimeError.h"
#include "lox/Token.h"

namespace Lox
{
void Environment::define(const std::string& name, const std::any& value)
{
    values.emplace(name, value);
}

void Environment::assign(const Token& name, const std::any& value)
{
    auto it = values.find(name.getText());
    if (it == values.end()) {
        throw RuntimeError(name, "Undefined variable '" + name.getText() + "'.");
    }

    it->second = value;
}

const std::any& Environment::get(const Token& name) const
{
    auto it = values.find(name.getText());
    if (it == values.end()) {
        throw RuntimeError(name, "Undefined variable '" + name.getText() + "'.");
    }

    return it->second;
}

} // namespace Lox


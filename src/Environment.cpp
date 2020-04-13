#include "lox/Environment.h"

#include "lox/RuntimeError.h"
#include "lox/Token.h"

#include <cassert>

namespace Lox
{
Environment::Environment() : enclosing(nullptr)
{}

Environment::Environment(Environment* enclosing) : enclosing(enclosing)
{
    assert(enclosing != nullptr);
}

void Environment::define(const std::string& name, const std::any& value)
{
    values.emplace(name, value);
}

void Environment::assign(const Token& name, const std::any& value)
{
    auto it = values.find(name.getText());
    if (it != values.end()) {
        it->second = value;
    }

    if (enclosing) {
        enclosing->assign(name, value);
    }

    throw RuntimeError(name, "Undefined variable '" + name.getText() + "'.");
}

const std::any& Environment::get(const Token& name) const
{
    auto it = values.find(name.getText());
    if (it != values.end()) {
        return it->second;
    }

    if (enclosing) {
        return enclosing->get(name);
    }

    throw RuntimeError(name, "Undefined variable '" + name.getText() + "'.");
}

} // namespace Lox


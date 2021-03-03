#include "lox/Callable.h"

#include "lox/Environment.h"
#include "lox/Interpreter.h"
#include "lox/ReturnException.h"
#include "lox/Stmt/FunctionStmt.h"

#include <cassert>

namespace Lox
{
Callable::Callable(int arity, FuncType f) : arity(arity), f(f), declaration(nullptr)
{}

Callable::Callable(const FunctionStmt* declaration) : declaration(declaration)
{
    assert(declaration);
    arity = static_cast<int>(declaration->getParams().size());
}

std::any Callable::call(Interpreter& intepreter, const std::vector<std::any>& arguments) const
{
    if (!declaration) {
        return f(intepreter, arguments);
    }

    // LoxCallable
    const auto& params = declaration->getParams();
    assert(params.size() == arguments.size()); // before we call, we check for arity, but let's be
                                               // extra-safe

    // define params as local variables for env
    auto env = std::make_unique<Environment>(&intepreter.getGlobalsEnvironment());
    for (std::size_t i = 0u; i < params.size(); ++i) {
        env->define(params.at(i).getText(), arguments.at(i));
    }

    try {
        intepreter.executeBlock(declaration->getBody(), std::move(env));
    } catch (const ReturnException& v) {
        return v.getValue();
    }

    return std::any{};
}
} // end of namespace Lox

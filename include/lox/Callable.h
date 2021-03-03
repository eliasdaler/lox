#pragma once

#include <any>
#include <functional>
#include <memory>
#include <vector>

namespace Lox
{
class Interpreter;
class FunctionStmt;

using FuncType = std::function<std::any(Interpreter&, const std::vector<std::any>&)>;

class Callable {
public:
    Callable(int arity, FuncType f); // Callbale
    Callable(const FunctionStmt* declaration); // LoxFunction

    std::any call(Interpreter& intepreter, const std::vector<std::any>& arguments) const;

    int getArity() const { return arity; }

private:
    int arity;
    FuncType f;

    const FunctionStmt* declaration; // TODO: make LoxFunction?
};
} // end of namespace Lox

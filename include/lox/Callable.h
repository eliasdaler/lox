#pragma once

#include <any>
#include <functional>
#include <vector>

namespace Lox
{
class Interpreter;

using FuncType = std::function<std::any(Interpreter&, const std::vector<std::any>&)>;

class Callable {
public:
    Callable(int arity, FuncType f);
    int getArity() const;
    std::any call(Interpreter& intepreter, const std::vector<std::any>& arguments) const;

private:
    int arity;
    FuncType f;
};
} // end of namespace Lox

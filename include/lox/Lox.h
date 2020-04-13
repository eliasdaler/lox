#pragma once

#include <string>

namespace Lox
{
class Token;
class RuntimeError;

class Lox {
public:
    static void Report(int line, const std::string& where, const std::string& message);
    static void Error(int line, const std::string& message);
    static void Error(Token token, const std::string& message);
    static void ReportRuntimeError(const RuntimeError& error);

    static bool HadError;
    static bool HadRuntimeError;
};

} // end of namespace Lox

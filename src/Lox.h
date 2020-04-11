#pragma once

#include <string>

class Lox {
   public:
    static void Report(int line, const std::string& where,
                       const std::string& message);
    static void Error(int line, const std::string& message);
    static bool HadError;
};


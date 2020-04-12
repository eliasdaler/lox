#include <cmath>
#include <fstream>
#include <iostream>
#include <string>

#include "lox/Interpreter.h"
#include "lox/Lox.h"
#include "lox/Parser.h"
#include "lox/Scanner.h"

namespace
{
static Lox::Interpreter Interpreter;

std::string stringify(const std::any& object)
{
    if (!object.has_value()) {
        return "nil";
    }

    if (object.type() == typeid(bool)) {
        return std::any_cast<bool>(object) ? "true" : "false";
    }

    if (object.type() == typeid(double)) {
        double n = std::any_cast<double>(object);
        if (std::trunc(n) == n) { // is int
            return std::to_string((int)n);
        } else {
            return std::to_string(n); // TODO: don't print trailing zeros
        }
    }

    if (object.type() == typeid(std::string)) {
        return std::any_cast<std::string>(object);
    }

    return "";
}
}

void run(const std::string& source)
{
    Lox::Scanner scanner{source};
    Lox::Parser parser{scanner.scanTokens()};
    auto expr = parser.parse();
    if (expr) {
        auto value = Interpreter.intepret(*expr);
        std::cout << ::stringify(value) << std::endl;
    }
}

void runFile(const std::string& filename)
{
    std::cout << "Running file '" << filename << "'..." << std::endl;

    std::ifstream file{filename};
    std::string line;
    std::string source;
    while (std::getline(file, line)) {
        source += line + "\n";
    }

    run(source);
    if (Lox::Lox::HadError) {
        exit(2);
    }
    if (Lox::Lox::HadRuntimeError) {
        exit(3);
    }
}

void runPrompt()
{
    std::cout << "lox v0.0.1" << std::endl;

    std::string code;
    while (true) {
        std::cout << "> ";
        if (std::getline(std::cin, code)) {
            run(code);
            Lox::Lox::HadError = false;
        } else {
            std::cout << std::endl;
            break;
        }
    }
}

int main(int args, char* argv[])
{
    if (args > 2) {
        std::cout << "usage: lox [script]" << std::endl;
        exit(1);
    } else if (args == 2) {
        runFile(argv[1]);
    } else {
        runPrompt();
    }
}

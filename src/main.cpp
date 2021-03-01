#include <cmath>
#include <fstream>
#include <iostream>
#include <string>

#include <fmt/core.h>

#include "lox/Interpreter.h"
#include "lox/Lox.h"
#include "lox/Parser.h"
#include "lox/Scanner.h"

// TODO: add CMake generated header for version
#define LOX_VERSION "0.0.1"

namespace
{
static Lox::Interpreter Interpreter(std::cout);
}

void run(const std::string& source)
{
    Lox::Scanner scanner{source};
    Lox::Parser parser{scanner.scanTokens()};
    const auto statements = parser.parse();

    if (Lox::Lox::HadError) {
        return;
    }

    Interpreter.intepret(statements);
}

void runFile(const std::string& filename)
{
    std::ifstream file{filename};
    if (!file.good()) {
        // TODO: write proper error message
        fmt::print("Failed to open {}: No such file or directory\n", filename);
        return;
    }

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
    fmt::print("lox v{}\n", LOX_VERSION);

    std::string code;
    while (true) {
        fmt::print("> ");
        if (std::getline(std::cin, code)) {
            run(code);
            Lox::Lox::HadError = false;
        } else {
            fmt::print("\n");
            break;
        }
    }
}

int main(int args, char* argv[])
{
    if (args > 2) {
        fmt::print("usage: lox [script]\n");
        exit(1);
    } else if (args == 2) {
        runFile(argv[1]);
    } else {
        runPrompt();
    }
}

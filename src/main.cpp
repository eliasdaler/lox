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
        std::cout << "Failed to open '" << filename << "': No such file or directory" << std::endl;
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

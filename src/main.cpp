#include <fstream>
#include <iostream>
#include <string>

#include "lox/Lox.h"
#include "lox/Scanner.h"

void run(const std::string& source)
{
    Lox::Scanner scanner{source};
    const auto tokens = scanner.scanTokens();
    for (const auto& token : tokens) {
        std::cout << token.toString() << std::endl;
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

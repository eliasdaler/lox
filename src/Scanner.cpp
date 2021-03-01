#include "lox/Scanner.h"

#include "lox/Lox.h"

#include <cctype> // isdigit, isalpha

#include <fmt/core.h>

namespace Lox
{
Scanner::Scanner(std::string source) : source(std::move(source))
{
    /* clang-format off */
    keywords = {
        {"and", TokenType::And},
        {"class", TokenType::Class},
        {"else", TokenType::Else},
        {"false", TokenType::False},
        {"for", TokenType::For},
        {"fun", TokenType::Fun},
        {"if", TokenType::If},
        {"nil", TokenType::Nil},
        {"or", TokenType::Or},
        {"print", TokenType::Print},
        {"return", TokenType::Return},
        {"super", TokenType::Super},
        {"this", TokenType::This},
        {"true", TokenType::True},
        {"var", TokenType::Var},
        {"while", TokenType::While},
    };
    /* clang-format on */
}

std::vector<Token> Scanner::scanTokens()
{
    while (!isAtEnd()) {
        start = current;
        scanToken();
    }
    tokens.emplace_back(TokenType::TokenEOF, "", line);
    return tokens;
}

bool Scanner::isAtEnd() const
{
    return current >= static_cast<int>(source.size());
}

char Scanner::advance()
{
    ++current;
    return source.at(current - 1);
}

bool Scanner::match(char expected)
{
    if (isAtEnd()) {
        return false;
    }
    if (source.at(current) != expected) {
        return false;
    }

    ++current;
    return true;
}

char Scanner::peek() const
{
    if (isAtEnd()) {
        return '\0';
    }
    return source.at(current);
}

char Scanner::peekNext() const
{
    if (current + 1 >= static_cast<int>(source.size())) {
        return '\0';
    }
    return source.at(current + 1);
}

void Scanner::addToken(TokenType type, std::any literal)
{
    tokens.emplace_back(type, source.substr(start, current - start), literal, line);
}

void Scanner::addToken(TokenType type)
{
    tokens.emplace_back(type, source.substr(start, current - start), line);
}

void Scanner::comment()
{
    // A comment goes until the end of the line.
    while (peek() != '\n' && !isAtEnd()) {
        advance();
    }
}

void Scanner::string()
{
    while (peek() != '"' && !isAtEnd()) {
        if (peek() == '\n') {
            ++line;
        }
        advance();
    }

    // Unterminated string
    if (isAtEnd()) {
        Lox::Error(line, "Unterminated string.");
    }

    advance(); // the closing '"'

    // Trim the surrounding quotes
    addToken(TokenType::String, source.substr(start + 1, current - start - 2));
}

void Scanner::number()
{
    while (std::isdigit(peek())) {
        advance();
    }

    // Look for a fractional part.
    if (peek() == '.' && std::isdigit(peekNext())) {
        // Consume the "."
        advance();

        while (std::isdigit(peek())) {
            advance();
        }
    }

    addToken(TokenType::Number, std::stod(source.substr(start, current - start)));
}

void Scanner::identifier()
{
    while (std::isdigit(peek()) || std::isalpha(peek()) || peek() == '_') {
        advance();
    }

    const auto text = source.substr(start, current - start);
    if (const auto it = keywords.find(text); it != keywords.end()) {
        addToken(it->second);
    } else {
        addToken(TokenType::Identifier);
    }
}

void Scanner::scanToken()
{
    auto c = advance();
    switch (c) {
    case '(':
        addToken(TokenType::LeftParen);
        break;
    case ')':
        addToken(TokenType::RightParen);
        break;
    case '{':
        addToken(TokenType::LeftBrace);
        break;
    case '}':
        addToken(TokenType::RightBrace);
        break;
    case ',':
        addToken(TokenType::Comma);
        break;
    case '.':
        addToken(TokenType::Dot);
        break;
    case '-':
        addToken(TokenType::Minus);
        break;
    case '+':
        addToken(TokenType::Plus);
        break;
    case ';':
        addToken(TokenType::Semicolon);
        break;
    case '*':
        addToken(TokenType::Star);
        break;
    case '!':
        addToken(match('=') ? TokenType::BangEqual : TokenType::Bang);
        break;
    case '=':
        addToken(match('=') ? TokenType::EqualEqual : TokenType::Equal);
        break;
    case '<':
        addToken(match('=') ? TokenType::LessEqual : TokenType::Less);
        break;
    case '>':
        addToken(match('=') ? TokenType::GreaterEqual : TokenType::Greater);
        break;
    case '/':
        if (match('/')) {
            comment();
        } else {
            addToken(TokenType::Slash);
        }
        break;
    case ' ':
    case '\r':
    case '\t':
        // Ignore whitespace.
        break;
    case '\n':
        line++;
        break;
    case '"':
        string();
        break;

    default:
        if (std::isdigit(c)) {
            number();
        } else if (std::isalpha(c) || c == '_') {
            identifier();
        } else {
            Lox::Error(line, fmt::format("Unexpected character: '{}'", c));
        }
        break;
    }
}

} // end of namespace Lox

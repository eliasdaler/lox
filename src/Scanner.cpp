#include "Scanner.h"

#include "Lox.h"

namespace
{
bool isDigit(char c)
{
    return c >= '0' && c <= '9';
}

bool isAlpha(char c)
{
    return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || c == '_';
}

bool isAlphaNumeric(char c)
{
    return isAlpha(c) || isDigit(c);
}

} // end of anonymous namespace

Scanner::Scanner(std::string source) : source(std::move(source))
{
    keywords = {
        {"and", TokenType::And},     {"class", TokenType::Class},   {"else", TokenType::Else},
        {"false", TokenType::False}, {"for", TokenType::For},       {"fun", TokenType::Fun},
        {"if", TokenType::If},       {"nil", TokenType::Nil},       {"or", TokenType::Or},
        {"print", TokenType::Print}, {"return", TokenType::Return}, {"super", TokenType::Super},
        {"this", TokenType::This},   {"true", TokenType::True},     {"var", TokenType::Var},
        {"while", TokenType::While},
    };
}

std::vector<Token> Scanner::scanTokens()
{
    while (!isAtEnd()) {
        start = current;
        scanToken();
    }
    tokens.emplace_back(TokenType::TokenEOF, "", std::any{}, line);
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

char Scanner::peek()
{
    if (isAtEnd()) {
        return '\0';
    }
    return source.at(current);
}

char Scanner::peekNext()
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
    addToken(type, std::any{});
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
    while (::isDigit(peek())) {
        advance();
    }

    // Look for a fractional part.
    if (peek() == '.' && ::isDigit(peekNext())) {
        // Consume the "."
        advance();

        while (::isDigit(peek()))
            advance();
    }

    addToken(TokenType::Number, std::stod(source.substr(start, current - start)));
}

void Scanner::identifier()
{
    while (::isAlphaNumeric(peek())) {
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
        addToken(match('=') ? TokenType::Equal : TokenType::Equal);
        break;
    case '<':
        addToken(match('=') ? TokenType::LessEqual : TokenType::Less);
        break;
    case '>':
        addToken(match('=') ? TokenType::GreaterEqual : TokenType::Greater);
        break;
    case '/':
        if (match('/')) {
            // A comment goes until the end of the line.
            while (peek() != '\n' && !isAtEnd()) {
                advance();
            }
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
        if (::isDigit(c)) {
            number();
        } else if (::isAlpha(c)) {
            identifier();
        } else {
            Lox::Error(line, std::string("Unexpected character: ") + "'" + c + "'");
        }
        break;
    }
}


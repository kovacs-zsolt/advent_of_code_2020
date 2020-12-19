#pragma once

#include <cassert>
#include <cstdint>

#include <iosfwd>

enum class TokenType { VALUE, UNARY_OPERATOR, BINARY_OPERATOR, OPENING_PARENTHESIS, CLOSING_PARENTHESIS, END, ERROR };

struct TokenTypeUtils
{
    static TokenType getType(char peekedNext);
    static bool isBinaryOperator(char value);
    static bool isUnaryOperator(char value);
    static bool isOperator(char value);
    static bool isParenthesis(char value);

    static int precedence(char op);
    enum class Associativity{LEFT, RIGHT};
    static Associativity associativity(char op);
};

struct Token
{
    using Type = TokenType;

    Type getType() const { return type; }
    std::int64_t getValue() const;
    char getSymbol() const;

    bool isValue() const;
    bool isSymbol() const;

    Type type = Type::ERROR;
    std::int64_t value = 0;
};

struct Tokenizer
{
    TokenType peekNextType();

    Token extractNextToken();

    std::istream& stream;
};

#include "Tokenizer.hh"

#include <cctype>

#include <iostream>

///////////////////////////////////////////////////

int TokenTypeUtils::precedence(char op)
{
    switch(op)
    {
        case '+':
            return 2;
        case '-':
            //if(isUnaryOperator(op))
            //    return 400;
            //else
                return 2;
        case '*':
            return 3;
        case '/':
            return 3;
        case '^':
            return 4;
        default:
            assert(false);
            return 0;
    }
}

TokenTypeUtils::Associativity TokenTypeUtils::associativity(char op)
{
    switch(op)
    {
        case '+':
        case '-':
        case '*':
        case '/':
            return Associativity::LEFT;

        case '^':
            return Associativity::RIGHT;
        default:
            assert(false);
            return Associativity::LEFT;
    }
}

TokenType TokenTypeUtils::getType(char value)
{
    if(std::isdigit(value))
    {
        return TokenType::VALUE;
    }
    else if(isUnaryOperator(value))
    {
        return TokenType::UNARY_OPERATOR;
    }
    else if(isBinaryOperator(value))
    {
        return TokenType::BINARY_OPERATOR;
    }
    else if(value == '(')
    {
        return TokenType::OPENING_PARENTHESIS;
    }
    else if(value == ')')
    {
        return TokenType::CLOSING_PARENTHESIS;
    }
    else if(value == EOF)
    {
        return TokenType::END;
    }
    return TokenType::ERROR;
}

bool TokenTypeUtils::isUnaryOperator(char value)
{
    switch(value)
    {
        case '-':
            return true;
        default:
            return false;
    }
}

bool TokenTypeUtils::isBinaryOperator(char value)
{
    switch(value)
    {
        case '+':
        case '-':
        case '*':
        case '/':
        case '^':
            return true;
        default:
            return false;
    }
}

bool TokenTypeUtils::isOperator(char value)
{
    return isBinaryOperator(value) || isUnaryOperator(value);
}

bool TokenTypeUtils::isParenthesis(char value)
{
    switch(getType(value))
    {
        case TokenType::OPENING_PARENTHESIS:
        case TokenType::CLOSING_PARENTHESIS:
            return true;
        default:
            return false;
    }
}

///////////////////////////////////////////////////

bool Token::isValue() const
{
    switch(type)
    {
        case Type::VALUE:
            return true;
        default:
            return false;
    }
}

bool Token::isSymbol() const
{
    return isValue() == false;
}

char Token::getSymbol() const
{
    assert(isValue() == false);
    return static_cast<char>(value);
}

std::int64_t Token::getValue() const
{
    assert(isValue());
    return value;
}

///////////////////////////////////////////////////

Token::Type Tokenizer::peekNextType()
{
    stream >> std::ws;
    char next = stream.peek();
    return TokenTypeUtils::getType(next);
}

Token Tokenizer::extractNextToken()
{
    Token::Type nextTokenType = peekNextType();

    Token token{nextTokenType, 0};
    switch(nextTokenType)
    {
        case TokenType::VALUE:
        {
            int temp = -1;
            stream >> temp;
            assert(stream);
            token.value = temp;
            stream >> std::ws;
            int debug = 123;
        }
        break;
        case TokenType::UNARY_OPERATOR:
        case TokenType::BINARY_OPERATOR:
        case TokenType::OPENING_PARENTHESIS:
        case TokenType::CLOSING_PARENTHESIS:
        {
            char op = 0;
            stream >> op >> std::ws;
            token.value = op;
            int debug = 123;
        }
        break;
        case TokenType::END:
            break;
        case TokenType::ERROR:
            assert(false);
            break;
        default:
            assert(false);
    }
    return token;
}

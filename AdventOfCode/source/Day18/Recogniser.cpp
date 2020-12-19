#include "Recogniser.hh"

#include <cassert>

TokenType Recogniser::next()
{
    return tokenizer.peekNextType();
}

void Recogniser::consume()
{
    Token token = tokenizer.extractNextToken();
}

void Recogniser::expect(TokenType expectedTokenType)
{
    auto nextTokenType = next();
    if(nextTokenType == expectedTokenType)
    {
        consume();
    }
    else
    {
        throw ParseError{};
    }
}

bool Recogniser::recognise()
{
    try
    {
        E();
        expect(TokenType::END);
    }
    catch(const ParseError&)
    {
        return false;
    }
    return true;
}

void Recogniser::E()
{
    P();
    while(next() == TokenType::BINARY_OPERATOR)
    {
        consume();
        P();
    }
}

void Recogniser::P()
{
    if(next() == TokenType::VALUE)
    {
        consume();
    }
    else if(next() == TokenType::OPENING_PARENTHESIS)
    {
        consume();
        E();
        expect(TokenType::CLOSING_PARENTHESIS);
    }
    else if(next() == TokenType::UNARY_OPERATOR)
    {
        consume();
        P();
    }
    else
        throw ParseError{};
}

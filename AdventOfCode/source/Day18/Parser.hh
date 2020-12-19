#pragma once

#include "Tokenizer.hh"

#include <sstream>
#include <stack>
#include <string>

// http://www.engr.mun.ca/~theo/Misc/exp_parsing.htm

struct Tree
{
    Tree(const Token& token, std::unique_ptr<Tree> left = {}, std::unique_ptr<Tree> right = {}) : token{token}, left{std::move(left)}, right{std::move(right)}{}
    Token token;

    double getValue() const;

    std::unique_ptr<Tree> left;
    std::unique_ptr<Tree> right;
};

struct Parser
{
public:
    Parser(const std::string& line) : ss{line}, tokenizer{ss} {}
    double parse();

private:
    void expect(TokenType expectedTokenType);
    TokenType next();
    Token consume();

    void E();
    void P();

    void popOperator();
    void pushOperator(Token token);

    static bool compare(const Token& lhs, const Token& rhs);

private:
    std::stringstream ss;
    Tokenizer tokenizer;

    std::stack<Token> operators;
    std::stack<std::unique_ptr<Tree>> operands;
};
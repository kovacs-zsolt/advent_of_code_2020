#include "Parser.hh"

#include "Tokenizer.hh"

#include <cmath>

#include <memory>

struct ParseError {};

double Tree::getValue() const
{
    if(token.isValue())
    {
        return static_cast<double>(token.getValue());
    }
    else if(token.getType() == TokenType::BINARY_OPERATOR)
    {
        assert(left);
        assert(right);
        double v0 = left->getValue();
        double v1 = right->getValue();

        switch(token.getSymbol())
        {
            case '+':
                return v0 + v1;
            case '-':
                return v0 - v1;

            case '*':
                return v0 * v1;

            case '/':
                return v0 / v1;

            case '^':
                return std::pow(v0, v1);
            default:
                assert(false);
                return 0.0;
        }
    }
    else if(token.getType() == TokenType::UNARY_OPERATOR)
    {
        assert(left);
        assert((bool)right == false);
        double v0 = left->getValue();
        switch(token.getSymbol())
        {
            case '-':
                return -v0;
            default:
                assert(false);
                return 0.0;
        }
    }
    assert(false);
    return 0.0;
}

void Parser::expect(TokenType expectedTokenType)
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

TokenType Parser::next()
{
    return tokenizer.peekNextType();
}

Token Parser::consume()
{
    Token token = tokenizer.extractNextToken();
    return token;
}

double Parser::parse()
{
    E();
    expect(TokenType::END);
    std::unique_ptr<Tree> tree = std::move(operands.top());
    operands.pop();
    double result = tree->getValue();
    return result;
}

void Parser::E()
{
    P();
    while(next() == TokenType::BINARY_OPERATOR)
    {
        pushOperator(consume());
        P();
    }
    while(operators.empty() == false && operators.top().getType() != TokenType::OPENING_PARENTHESIS)
    {
        popOperator();
    }
}

void Parser::P()
{
    auto nextType = next();
    if(nextType == TokenType::VALUE)
    {
        std::unique_ptr<Tree> operand{std::make_unique<Tree>(consume())};
        operands.push(std::move(operand));
    }
    else if(nextType == TokenType::OPENING_PARENTHESIS)
    {
        operators.push(consume());
        E();
        expect(TokenType::CLOSING_PARENTHESIS);
        operators.pop();
    }
    else if(nextType == TokenType::UNARY_OPERATOR)
    {
        pushOperator(consume());
        consume();
        P();
    }
    else
    {
        throw ParseError{};
    }
}

void Parser::popOperator()
{
    const Token& topOperators = operators.top();
    if(topOperators.getType() == TokenType::BINARY_OPERATOR)
    {
        std::unique_ptr<Tree> v0 = std::move(operands.top());
        operands.pop();
        std::unique_ptr<Tree> v1 = std::move(operands.top());
        operands.pop();
        Token op = operators.top();
        operators.pop();
        operands.push(std::make_unique<Tree>(op, std::move(v0), std::move(v1)));
    }
    else
    {
        std::unique_ptr<Tree> v0 = std::move(operands.top());
        operands.pop();
        Token op = operators.top();
        operators.pop();
        operands.push(std::make_unique<Tree>(op, std::move(v0)));
    }
}

bool Parser::compare(const Token& lhs, const Token& rhs)
{
    assert(lhs.isSymbol());
    assert(TokenTypeUtils::isOperator(lhs.getSymbol()) || TokenTypeUtils::isParenthesis(lhs.getSymbol()));
    assert(rhs.isSymbol());
    assert(TokenTypeUtils::isOperator(rhs.getSymbol()) || TokenTypeUtils::isParenthesis(lhs.getSymbol()));

    if(lhs.getType() == TokenType::BINARY_OPERATOR && rhs.getType() == TokenType::BINARY_OPERATOR)
    {
        auto lhsPrecedence = TokenTypeUtils::precedence(lhs.getSymbol());
        auto rhsPrecedence = TokenTypeUtils::precedence(rhs.getSymbol());

        auto lhsAssociativity = TokenTypeUtils::associativity(lhs.getSymbol());

        return lhsPrecedence > rhsPrecedence || (lhsPrecedence == rhsPrecedence && lhsAssociativity == TokenTypeUtils::Associativity::LEFT);
    }
    else if(lhs.getType() == TokenType::UNARY_OPERATOR && rhs.getType() == TokenType::BINARY_OPERATOR)
    {
        auto lhsPrecedence = TokenTypeUtils::precedence(lhs.getSymbol());
        auto rhsPrecedence = TokenTypeUtils::precedence(rhs.getSymbol());
        return lhsPrecedence >= rhsPrecedence;
    }
    else if(rhs.getType() == TokenType::UNARY_OPERATOR)
    {
        return false;
    }
    else if(TokenTypeUtils::isParenthesis(lhs.getSymbol()))
    {
        return false;
    }
    else
    {
        assert(false);
        return false;
    }
}

void Parser::pushOperator(Token op)
{
    if(operators.empty() == false)
    {
        for(Token operatorsTop = operators.top(); compare(operatorsTop, op); operatorsTop = operators.top())
        {
            popOperator();
        }
    }
    operators.push(op);
}

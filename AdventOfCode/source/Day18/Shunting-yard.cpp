#include "Shunting-yard.hh"

#include <stack>
#include <vector>

std::string ShuntingYard::transformToReversePolishNotation()
{
    std::vector<Token> outputQueue;
    std::stack<Token> operatorStack;

    for(;tokenizer.peekNextType() != TokenType::END;)
    {
        Token token = tokenizer.extractNextToken();
        if(token.isValue())
        {
            outputQueue.push_back(token);
        }
        else if(TokenTypeUtils::isOperator(token.getSymbol()))
        {
            assert(TokenTypeUtils::isParenthesis(token.getSymbol()) == false);
            for(;operatorStack.empty() == false;)
            {
                Token operatorOnStack = operatorStack.top();
                if(operatorOnStack.getType() == TokenType::OPENING_PARENTHESIS)
                {
                    break;
                }
                auto precedenceCurrent = TokenTypeUtils::precedence(token.getSymbol());
                auto precedenceTopOfStack = TokenTypeUtils::precedence(operatorOnStack.getSymbol());
                auto currentIsLeftAssociative = TokenTypeUtils::associativity(token.getSymbol()) == TokenTypeUtils::Associativity::LEFT;
                if((precedenceTopOfStack > precedenceCurrent) || (precedenceTopOfStack == precedenceCurrent && currentIsLeftAssociative))
                {
                    operatorStack.pop();
                    outputQueue.push_back(operatorOnStack);
                }
                else
                {
                    break;
                }
            }

            operatorStack.push(token);
        }
        else if(token.getType() == TokenType::OPENING_PARENTHESIS)
        {
            operatorStack.push(token);
        }
        else if(token.getType() == TokenType::CLOSING_PARENTHESIS)
        {
            Token operatorOnStack = operatorStack.top();
            for(; operatorOnStack.getType() != TokenType::OPENING_PARENTHESIS; operatorOnStack = operatorStack.top())
            {
                if(operatorStack.empty())
                    assert(false && "Missing ) for (");

                operatorStack.pop();
                outputQueue.push_back(operatorOnStack);
            }

            if(operatorOnStack.getType() == TokenType::OPENING_PARENTHESIS)
            {
                operatorStack.pop();
            }
            //else if() // function token
        }
    }

    for(; operatorStack.empty() == false;)
    {
        Token operatorOnStack = operatorStack.top();
        operatorStack.pop();
        outputQueue.push_back(operatorOnStack);
    }

    assert(operatorStack.empty());
    auto createOutput = [](const std::vector<Token>& outputQueue)
    {
        std::string result;
        for(const Token& token : outputQueue)
        {
            if(token.isValue())
            {
                result += std::to_string(token.getValue());
                result += " ";
            }
            else if(token.isSymbol())
            {
                result += token.getSymbol();
                result += " ";
            }
            else
            {
                assert(false);
            }
        }
        assert(result.back() == ' ');
        result.erase(result.size() - 1);
        return result;
    };

    std::string result{createOutput(outputQueue)};
    return result;
}

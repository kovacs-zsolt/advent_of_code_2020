#pragma once

#include "Tokenizer.hh"

#include <sstream>
#include <string>

// https://en.wikipedia.org/wiki/Shunting-yard_algorithm

struct ShuntingYard
{
    ShuntingYard(const std::string& line) : ss{line}, tokenizer{ss} {}

    std::string transformToReversePolishNotation();

    std::stringstream ss;
    Tokenizer tokenizer;
};

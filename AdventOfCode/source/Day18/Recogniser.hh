#pragma once

#include "Tokenizer.hh"

#include <sstream>
#include <string>

// http://www.engr.mun.ca/~theo/Misc/exp_parsing.htm

struct ParseError {};

struct Recogniser
{
    Recogniser(const std::string& line) : ss{line}, tokenizer{ss} {}
    bool recognise();

private:

    void expect(TokenType expectedTokenType);
    TokenType next();
    void consume();

    void E();
    void P();

    std::stringstream ss;
    Tokenizer tokenizer;
};


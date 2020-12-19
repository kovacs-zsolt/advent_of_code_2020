#include "Day18/Parser.hh"
#include "Day18/Recogniser.hh"
#include "Day18/Shunting-yard.hh"

#include <cassert>
#define _USE_MATH_DEFINES
#include <cmath>

#include <algorithm>
#include <bitset>
#include <filesystem>
#include <functional>
#include <fstream>
#include <iostream>
#include <map>
#include <numeric>
#include <unordered_map>
#include <set>
#include <string>
#include <sstream>
#include <tuple>
#include <vector>

namespace Day18
{

using Number = std::uint64_t;

std::vector<std::string> loadExpressions(std::istream& stream)
{
    std::vector<std::string> result;
    for(std::string line; std::getline(stream, line);)
    {
        result.push_back(line);
    }
    return result;
}

Number getAnswerPart1(const std::filesystem::path& path)
{
    std::ifstream file{path};
    assert(file);
    Number result = 0;
    return result;
}

Number getAnswerPart2(const std::filesystem::path& path)
{
    std::ifstream file{path};
    assert(file);
    Number result = 0;
    return result;
}

void test()
{
    std::string line = "4 * 3 * (2 * 6 + (3 * 3 + 8 + 7 + 2) + 9 + 8) * 4 * 6";
    auto i0 = std::stoi(line);

    std::stringstream ss{line};
    
    int dummy = -1;
    ss >> dummy;
    char next = ss.peek();
    ss >> std::ws;
    next = ss.peek();

    Recogniser{"1"}.recognise();
    Recogniser{"(1)"}.recognise();
    Recogniser{"1 + 2"}.recognise();
    Recogniser{"1 + 2*3"}.recognise();
    Recogniser{"1 + (2*3)"}.recognise();
    Recogniser{line}.recognise();

    assert(ShuntingYard{"2^3^4"}.transformToReversePolishNotation() == "2 3 4 ^ ^");
    assert(ShuntingYard{"1 + 2 * 3"}.transformToReversePolishNotation() == "1 2 3 * +");
    assert(ShuntingYard{"1 * 2 + 3"}.transformToReversePolishNotation() == "1 2 * 3 +");

    auto rpn0 =
    {
        ShuntingYard{"1"}.transformToReversePolishNotation(),
        ShuntingYard{"(1)"}.transformToReversePolishNotation(),
        ShuntingYard{"1 + 3"}.transformToReversePolishNotation(),
        ShuntingYard{"1 + 3 * 4"}.transformToReversePolishNotation(),
        ShuntingYard{"1 + (3 * 4)"}.transformToReversePolishNotation(),
        ShuntingYard{"(1 + 3) * 4"}.transformToReversePolishNotation(),
        ShuntingYard{"3 + 4 * 2 /(1-5)^2^3"}.transformToReversePolishNotation(),
    };

    auto d0 = Parser{"1"}.parse();
    auto d1 = Parser{"(1)"}.parse();
    auto d3 = Parser{"(1) + 2 * 3"}.parse();
    auto d4 = Parser{"((1) + 2) * 3"}.parse();
    int debug = 1234;
}

void solve()
{
    test();

    std::filesystem::path inputPath{std::filesystem::current_path().parent_path()};
    std::filesystem::path testPath{std::filesystem::current_path().parent_path()};
    inputPath += "/data/PuzzleInput/Day18/input.txt";
    testPath += "/data/PuzzleInput/Day18/test";

    auto resultTest = getAnswerPart1(testPath);
    std::cout << "test part1: " << resultTest << "\n";
    assert(resultTest == 112);

    auto resultInput = getAnswerPart1(inputPath);
    std::cout << "real part1: " << resultInput << "\n";
    assert(resultInput == 304);



    auto resultTest2 = getAnswerPart2(testPath);
    std::cout << "test part2: " << resultTest2 << "\n";
    //assert(resultTest2 == 208);

    auto resultInput2 = getAnswerPart2(inputPath);
    std::cout << "real part2: " << resultInput2 << "\n";
    //assert(resultInput2 == 3348493585827);

    int debug = 123;
}

}

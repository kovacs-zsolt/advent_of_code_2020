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
#include <vector>

namespace Day15
{

using Number = std::uint64_t;

struct Game
{
public:
    Game(const std::filesystem::path& initialNumbersPath) : initialNumbersPath{initialNumbersPath}{}
    Number run(Number turnCount);

private:
    static std::vector<Number> load(std::istream& stream);
    const std::filesystem::path initialNumbersPath;
    std::vector<Number> initialNumbers;
    
    void addNumber(Number number, Number turn);

    struct Data
    {
        Number spokenCount = 0;
        std::vector<Number> turns;
    };

    std::vector<Number> history;
    std::map<Number, Data> extendedHistory;
};

std::vector<Number> Game::load(std::istream& stream)
{
    std::vector<Number> result;
    for(; stream;)
    {
        Number temp = 0;
        stream >> temp;
        result.push_back(temp);
        char ignore;
        stream >> ignore;
        assert(ignore == ',' || ignore == EOF);
    }
    return result;
}

void Game::addNumber(Number number, Number turn)
{
    history.push_back(number);
    Data& data = extendedHistory[number];
    ++data.spokenCount;
    data.turns.push_back(turn);
}

Number Game::run(Number turnCount)
{
    std::ifstream file{initialNumbersPath};
    assert(file);
    initialNumbers = load(file);

    Number turn = 1;
    for(auto number : initialNumbers)
    {
        addNumber(number, turn);
        ++turn;
    }
    assert(history.empty() == false);
    for(;turn <= turnCount; ++turn)
    {
        Number number = history.back();
        Data& data = extendedHistory[number];
        assert(data.spokenCount > 0);
        if(data.spokenCount == 1)
        {
            addNumber(0, turn);
        }
        else
        {
            auto spokenPrev = *(data.turns.end() - 1);
            auto spokenPrevPrev = *(data.turns.end() - 2);
            Number nextNumber = spokenPrev - spokenPrevPrev;
            addNumber(nextNumber, turn);
        }
    }
    assert(history.size() == turnCount);
    Number result = history[static_cast<std::size_t>(turnCount) - 1];
    return result;
}

Number getAnswerPart1(const std::filesystem::path& path)
{
    static constexpr Number TURN_COUNT = 2020;
    Game game{path};
    return game.run(TURN_COUNT);
    //std::ifstream file{path};
    //assert(file);
    //auto numbers = load(file);
    //return run(numbers);
}

Number getAnswerPart2(const std::filesystem::path& path)
{
    static constexpr Number TURN_COUNT = 30000000;
    Game game{path};
    return game.run(TURN_COUNT);
    //std::ifstream file{path};
    //assert(file);
    //auto numbers = load(file);
    //return run(numbers);
}


void solve()
{
    std::filesystem::path inputPath{std::filesystem::current_path().parent_path()};
    std::filesystem::path testPath{std::filesystem::current_path().parent_path()};
    inputPath += "/data/PuzzleInput/Day15/input.txt";
    testPath += "/data/PuzzleInput/Day15/test";

    auto resultTest = getAnswerPart1(testPath);
    std::cout << "test part1: " << resultTest << "\n";
    assert(resultTest == 436);

    auto resultInput = getAnswerPart1(inputPath);
    std::cout << "real part1: " << resultInput << "\n";
    assert(resultInput == 1373);



    std::filesystem::path testPath2{std::filesystem::current_path().parent_path()};
    testPath2 += "/data/PuzzleInput/Day14/test2";

    //auto resultTest2 = getAnswerPart2(testPath2);
    //std::cout << "test part2: " << resultTest2 << "\n";
    ////assert(resultTest2 == 208);

    auto resultInput2 = getAnswerPart2(inputPath);
    std::cout << "real part2: " << resultInput2 << "\n";
    assert(resultInput2 == 112458);

    int debug = 123;
}

}
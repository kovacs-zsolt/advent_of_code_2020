#include <cassert>

#include <algorithm>
#include <filesystem>
#include <functional>
#include <fstream>
#include <iostream>
#include <map>
#include <numeric>
#include <set>
#include <string>
#include <sstream>
#include <vector>

namespace Day12
{

static const std::vector ACTIONS = {
    'N',
    'S',
    'E',
    'W',
    'L',
    'R',
    'F',
};


struct Instruction
{
    bool isMove() const;
    bool isTurn() const;

    char action;
    int value;
};

bool Instruction::isMove() const
{
    return isTurn() == false;
}

bool Instruction::isTurn() const
{
    switch(action)
    {
        case 'L':
        case 'R':
            return true;
        default:
            return false;
    }
}

using Instructions = std::vector<Instruction>;

Instructions loadInstructions(std::istream& stream)
{
    Instructions result;
    for(std::string line; std::getline(stream, line);)
    {        
        Instruction instruction;
        instruction.action = line[0];
        std::stringstream ss{line.substr(1)};
        ss >> instruction.value;
        result.push_back(instruction);
    }
    return result;
}

using Point = std::pair<int, int>;
using Direction = Point;
using Position = Point;

constexpr Direction NORTH = {0, 1};
constexpr Direction EAST = {1, 0};
constexpr Direction SOUTH = {0, -1};
constexpr Direction WEST = {-1, 0};

Direction operator*(const Direction& direction, int value)
{
    Direction result{direction.first * value, direction.second * value};
    return result;
}

Direction operator+(const Direction& lhs, const Direction& rhs)
{
    return {lhs.first + rhs.first, lhs.second + rhs.second};    
}

Direction& operator+=(Direction& lhs, const Direction& rhs)
{
    lhs.first += rhs.first;
    lhs.second += rhs.second;
    return lhs;
}

static std::vector DIRECTIONS = {NORTH, EAST, SOUTH, WEST};
static std::map<char, Direction> DIRECTION_MAPPING =
{
    {'N', NORTH},
    {'E', EAST},
    {'S', SOUTH},
    {'W', WEST},
};

struct Where
{
    void apply(Instruction instruction);

    Where& rotate(Instruction instruction);
    Where& move(Instruction instruction);

    Direction direction = EAST;
    Position position = {0, 0};
};

Where& Where::rotate(Instruction instruction)
{
    assert(instruction.isTurn());
    int indexOffset = (instruction.value / 90);
    switch(instruction.action)
    {
        case 'L':
            indexOffset *= -1;
            break;
        case 'R':
            break;
    }
    int currentIndex = 0;
    for(const auto& dirIt : DIRECTIONS)
    {
        if(dirIt == direction)
            break;
        ++currentIndex;
    }
    Direction newDirection = DIRECTIONS[(currentIndex + indexOffset) % DIRECTIONS.size()];    
    direction = newDirection;

    return *this;
}

Where& Where::move(Instruction instruction)
{
    assert(instruction.isMove());
    Direction offsetDirection = instruction.action == 'F' ? direction : DIRECTION_MAPPING[instruction.action];
    Point offset{offsetDirection * instruction.value};

    position += offset;

    return *this;
}

std::uint64_t calculateManhattanDistance(const Where& lhs, const Where& rhs)
{
    return std::abs(lhs.position.first - rhs.position.first) + std::abs(lhs.position.second - rhs.position.second);
}

void Where::apply(Instruction instruction)
{
    if(instruction.isMove())
    {
        move(instruction);
    }
    else if(instruction.isTurn())
    {
        rotate(instruction);
    }
    else
    {
        assert(false);
    }
}

void test()
{
    Where base{NORTH};
    assert(Where(base).rotate({'L', 90}).direction == WEST);
    assert(Where(base).rotate({'L', 180}).direction == SOUTH);
    assert(Where(base).rotate({'R', 90}).direction == EAST);
}

int getAnswer(const std::filesystem::path& path)
{
    std::ifstream file{path};
    assert(file);
    Instructions instructions = loadInstructions(file);

    Where where;
    for(const Instruction& instruction : instructions)
    {
        where.apply(instruction);
    }

    auto result = calculateManhattanDistance(Where{}, where);
    return result;
}

void part1()
{
    test();

    std::filesystem::path inputPath{std::filesystem::current_path().parent_path()};
    std::filesystem::path testPath{std::filesystem::current_path().parent_path()};
    inputPath += "/data/PuzzleInput/Day12/input.txt";
    testPath += "/data/PuzzleInput/Day12/test";
    auto resultInput = getAnswer(inputPath);
    auto resultTest = getAnswer(testPath);

    assert(resultInput == 879);
    assert(resultTest == 25);

    std::cout << "part1: " << resultInput << "\n"; // 879
    
    int debug = 123;
}

}

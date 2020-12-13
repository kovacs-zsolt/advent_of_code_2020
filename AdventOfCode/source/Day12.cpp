#include <cassert>
#define _USE_MATH_DEFINES
#include <cmath>

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

using Number = std::int64_t;

struct Instruction
{
    bool isMove() const;
    bool isTurn() const;

    char action;
    Number value;
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

using Point = std::pair<Number, Number>;
using Direction = Point;
using Position = Point;

constexpr Direction NORTH = {0, 1};
constexpr Direction EAST = {1, 0};
constexpr Direction SOUTH = {0, -1};
constexpr Direction WEST = {-1, 0};

Direction operator*(const Direction& direction, Number value)
{
    Direction result{direction.first * value, direction.second * value};
    return result;
}

Direction operator*(Number value, const Direction& direction)
{
    return direction * value;
}

Direction operator+(const Direction& lhs, const Direction& rhs)
{
    return {lhs.first + rhs.first, lhs.second + rhs.second};    
}

Direction operator-(const Direction& lhs, const Direction& rhs)
{
    return {lhs.first - rhs.first, lhs.second - rhs.second};
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
    Where& rotatePoint(Instruction instruction);

    Direction direction = EAST;
    Position position = {0, 0};
};

Where& Where::rotate(Instruction instruction)
{
    assert(instruction.isTurn());
    int indexOffset = static_cast<int>(instruction.value / 90);
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

Where& Where::rotatePoint(Instruction instruction)
{
    assert(instruction.action == 'R' || instruction.action == 'L');
    auto sign = instruction.action == 'R' ? 1 : -1;
    sign *= -1; // internet math thought it rotates the other way
    auto degree = sign * instruction.value;
    double radian = (degree/180.0) * M_PI;

    auto x = position.first;
    auto y = position.second;

    auto sin = std::sin(radian);
    auto cos = std::cos(radian);
    
    Number x_ = static_cast<Number>(std::round(x * cos - y * sin));
    Number y_ = static_cast<Number>(std::round(x * sin + y * cos));
    position = {x_, y_};
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

std::uint64_t getAnswerPart1(const std::filesystem::path& path)
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
    auto resultInput = getAnswerPart1(inputPath);
    auto resultTest = getAnswerPart1(testPath);

    assert(resultInput == 879);
    assert(resultTest == 25);

    std::cout << "part1: " << resultInput << "\n"; // 879
    
    Number debug = 123;
}

std::uint64_t getAnswerPart2(const std::filesystem::path& path)
{
    std::ifstream file{path};
    assert(file);
    Instructions instructions = loadInstructions(file);

    Point waypointOffset{(10 * EAST + 1 * NORTH)};

    Where ship;
    Where waypoint{ship};

    waypoint.position = waypointOffset;

    for(const Instruction& instruction : instructions)
    {
        if(instruction.action != 'F')
        {
            if(instruction.isMove())
            {
                waypoint.apply(instruction);
            }
            else
            {
                waypoint.rotatePoint(instruction);
            }
        }
        else
        {
            ship.position += waypoint.position * instruction.value;
            Number debug = 123;
        }
        int debug = 123;
    }

    auto result = calculateManhattanDistance(Where{}, ship);
    return result;
}

void part2()
{
    std::filesystem::path inputPath{std::filesystem::current_path().parent_path()};
    std::filesystem::path testPath{std::filesystem::current_path().parent_path()};
    inputPath += "/data/PuzzleInput/Day12/input.txt";
    testPath += "/data/PuzzleInput/Day12/test";

    auto answerTest = getAnswerPart2(testPath);
    assert(answerTest == 286);

    auto answerReal = getAnswerPart2(inputPath);
    assert(answerReal == 18107);
    std::cout << "part2: real: " << getAnswerPart2(inputPath) << "\n"; // 18107
    Number debug = 123;
}

}

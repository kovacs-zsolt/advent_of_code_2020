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

    static Direction rotate(Direction direction, Instruction instruction);
    static Where move(Where where, Instruction instruction);

    Direction direction = EAST;
    Position position = {0, 0};
};

Direction Where::rotate(Direction direction, Instruction instruction)
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
    return newDirection;
}

Where Where::move(Where where, Instruction instruction)
{
    assert(instruction.isMove());
    Direction direction = instruction.action == 'F' ? where.direction : DIRECTION_MAPPING[instruction.action];
    Point offset{direction.first * instruction.value, direction.second * instruction.value};
    Where result{where.direction, {where.position.first + offset.first, where.position.second + offset.second}};
    return result;
}

std::uint64_t calculateManhattanDistance(const Where& lhs, const Where& rhs)
{
    return std::abs(lhs.position.first - rhs.position.first) + std::abs(lhs.position.second - rhs.position.second);
}

void Where::apply(Instruction instruction)
{
    if(instruction.isMove())
    {
        (*this) = Where::move(*this, instruction);
    }
    else if(instruction.isTurn())
    {
        direction = Where::rotate(direction, instruction);
    }
    else
    {
        assert(false);
    }
}

void test()
{
    assert(Where::rotate(NORTH, {'L', 90}) == WEST);
    assert(Where::rotate(NORTH, {'L', 180}) == SOUTH);
    assert(Where::rotate(NORTH, {'R', 90}) == EAST);
}

void part1()
{
    test();

    std::filesystem::path path{std::filesystem::current_path().parent_path()};
    path += "/data/PuzzleInput/Day12/input.txt";
    //path += "/data/PuzzleInput/Day12/test";
    //path += "/data/PuzzleInput/Day12/test2";
    std::ifstream file{path};
    assert(file);
    Instructions instructions = loadInstructions(file);

    Where where;
    for(const Instruction& instruction : instructions)
    {
        where.apply(instruction);
    }

    auto result = calculateManhattanDistance(Where{}, where);

    std::cout << "part1: " << result << "\n"; // 879
    
    int debug = 123;
}

}

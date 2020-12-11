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

namespace Day11
{

using u64 = std::uint64_t;

struct State
{
    static constexpr char OCCUPIED = '#';
    static constexpr char EMPTY = 'L';
    static constexpr char FLOOR = '.';

    u64 countOccupied() const;
    State nextStepPart1() const;
    char nextStepPart1(std::size_t row, std::size_t column) const;
    State nextStepPart2() const;
    char nextStepPart2(std::size_t row, std::size_t column) const;
    char getCurrentState(std::size_t row, std::size_t column) const;
    u64 countOccupiedNeighbouringSeats(std::size_t row, std::size_t column) const;
    u64 countOccupiedNeighbouringSeatsPart2(std::size_t row, std::size_t column) const;

    std::size_t getNumRows() const;
    std::size_t getNumColumns() const;

    bool isValid(std::size_t row, std::size_t column) const;
    bool isValidRow(std::size_t row) const;
    bool isValidColumn(std::size_t column) const;

    std::vector<std::string> seats;
};

std::size_t State::getNumRows() const
{
    return seats.size();
}

std::size_t State::getNumColumns() const
{
    if(seats.empty())
    {
        return 0;
    }
    return seats.front().size();
}

bool State::isValidRow(std::size_t row) const
{
    return row < getNumRows();
}

bool State::isValidColumn(std::size_t column) const
{
    return column < getNumColumns();
}

bool State::isValid(std::size_t row, std::size_t column) const
{
    return isValidRow(row) && isValidColumn(column);
}

char State::getCurrentState(std::size_t row, std::size_t column) const
{
    assert(isValidColumn(column));
    assert(isValidRow(row));
    return seats[row][column];
}

u64 State::countOccupiedNeighbouringSeats(std::size_t row, std::size_t column) const
{
    std::vector<std::pair<std::size_t, std::size_t>> neighbours = {
        {row - 1, column - 1},
        {row - 1, column},
        {row - 1, column + 1},
        {row, column - 1},
        {row, column + 1},
        {row + 1, column - 1},
        {row + 1, column},
        {row + 1, column + 1},
    };
    u64 numOccupiedNeighbours = std::accumulate(neighbours.begin(), neighbours.end(), (u64)0, [this](u64 sum, const auto& neighbour)
    {
        auto [row, column] = neighbour;
        u64 result = sum;
        if(isValidColumn(column) && isValidRow(row))
        {
            if(getCurrentState(row, column) == OCCUPIED)
            {
                result += 1;
            }
        }
        return result;
    });
    return numOccupiedNeighbours;
}

char State::nextStepPart1(std::size_t row, std::size_t column) const
{
    char currentState = getCurrentState(row, column);
    u64 numOccupiedNeighbours = countOccupiedNeighbouringSeats(row, column);
    if(currentState == EMPTY && numOccupiedNeighbours == 0)
    {
        return OCCUPIED;
    }
    if(currentState == OCCUPIED && numOccupiedNeighbours >= 4)
    {
        return EMPTY;
    }
    return currentState;
}

u64 State::countOccupiedNeighbouringSeatsPart2(std::size_t row, std::size_t column) const
{
    std::vector<std::pair<int, int>> offsets = {
        { -1,  -1},
        { -1, 0},
        { -1,  1},
        {0,  -1},
        {0,  1},
        { 1,  -1},
        { 1, 0},
        { 1,  1}
    };
    u64 result = 0;
    for(const auto [rowOffset, columnOffset] : offsets)
    {
        for(std::size_t i = 1;;++i)
        {
            auto neighRow = row + (rowOffset * i);
            auto neighCol = column + (columnOffset * i);
            if(isValid(neighRow, neighCol))
            {
                auto neighState = getCurrentState(neighRow, neighCol);
                if(neighState != FLOOR)
                {
                    if(neighState == OCCUPIED)
                    {
                        ++result;
                    }
                    break;
                }
            }
            else
            {
                break;
            }
        }
    }

    return result;
}

char State::nextStepPart2(std::size_t row, std::size_t column) const
{
    char currentState = getCurrentState(row, column);
    u64 numOccupiedNeighbours = countOccupiedNeighbouringSeatsPart2(row, column);
    if(currentState == EMPTY && numOccupiedNeighbours == 0)
    {
        return OCCUPIED;
    }
    if(currentState == OCCUPIED && numOccupiedNeighbours >= 5)
    {
        return EMPTY;
    }
    return currentState;
}

u64 State::countOccupied() const
{
    u64 result = 0;
    for(std::size_t row = 0; row < seats.size(); ++row)
    {
        for(std::size_t column = 0; column < seats[row].size(); ++column)
        {
            if(getCurrentState(row, column) == OCCUPIED)
            {
                ++result;
            }
        }
    }
    return result;
}

State State::nextStepPart1() const
{
    State newState{*this};
    for(std::size_t row = 0; row < seats.size(); ++row)
    {
        for(std::size_t column = 0; column < seats[row].size(); ++column)
        {
            newState.seats[row][column] = nextStepPart1(row, column);
        }
    }
    return newState;
}

State State::nextStepPart2() const
{
    State newState{*this};
    for(std::size_t row = 0; row < seats.size(); ++row)
    {
        for(std::size_t column = 0; column < seats[row].size(); ++column)
        {
            newState.seats[row][column] = nextStepPart2(row, column);
        }
    }
    return newState;
}

State loadState(std::istream& stream)
{
    State state;
    for(std::string line; std::getline(stream, line);)
    {
        state.seats.push_back(line);
    }
    return state;
}

bool operator==(const State& lhs, const State& rhs)
{
    assert(lhs.getNumColumns() == rhs.getNumColumns());
    assert(lhs.getNumRows() == rhs.getNumRows());
    for(std::size_t row = 0; row < lhs.seats.size(); ++row)
    {
        for(std::size_t column = 0; column < lhs.seats[row].size(); ++column)
        {
            if(lhs.seats[row][column] != rhs.seats[row][column])
            {
                return false;
            }
        }
    }
    return true;
}

void part1()
{
    std::filesystem::path path{std::filesystem::current_path().parent_path()};
    path += "/data/PuzzleInput/Day11/input.txt";
    //path += "/data/PuzzleInput/Day11/test";
    //path += "/data/PuzzleInput/Day11/test2";
    std::ifstream file{path};
    assert(file);
    State state = loadState(file);

    std::vector<State> states{state};
    for(;;)
    {
        const State& oldState = states.back();
        State newState = oldState.nextStepPart2();
        std::cout << ".";
        if(newState == oldState)
        {
            u64 occupied = newState.countOccupied();
            std::cout << "\npart1: " << occupied << "\n"; // 2281
            break;
        }
        states.push_back(newState);
    }
    // part2: 2085
    int debug = 123;
}

}

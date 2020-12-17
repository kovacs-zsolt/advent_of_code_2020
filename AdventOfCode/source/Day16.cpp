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

namespace Day16
{

using Number = std::uint64_t;

struct Range
{
    bool isInRange(Number value) const;

    Number min;
    Number max;
};

struct Field
{
    bool isInRange(Number value) const;

    std::string name;
    std::vector<Range> ranges;
};

struct Ticket
{
    static Ticket load(const std::string& line);

    std::vector<Number> values;
};

struct Data
{
    static Data load(std::istream& stream);

    bool isValid(Number value) const;
    bool isValid(const Ticket& ticket) const;
    Number calculateErrorRate() const;

    void removeInvalidTickets();

    Number calculateFields();

    std::vector<Field> fields;
    std::vector<Ticket> tickets;
};

Ticket Ticket::load(const std::string& line)
{
    std::stringstream ss{line};

    Ticket ticket;
    for(std::string value; std::getline(ss, value, ',');)
    {
        ticket.values.push_back(std::stoll(value));
    }
    return ticket;
}

Data Data::load(std::istream& stream)
{
    static const std::string YOUR_TICKET_SEPARATOR = "your ticket:";
    static const std::string NEARBY_TICKETS_SEPARATOR = "nearby tickets:";

    Data result;
    enum class State {FIELDS, OWN_TICKET, OTHER_TICKETS};
    State state = State::FIELDS;
    for(std::string line; std::getline(stream, line);)
    {
        if(line.empty())
            continue;

        if(line == YOUR_TICKET_SEPARATOR)
        {
            state = State::OWN_TICKET;
            continue;
        }
            
        else if(line == NEARBY_TICKETS_SEPARATOR)
        {
            state = State::OTHER_TICKETS;
            continue;
        }

        switch(state)
        {
            case State::FIELDS:
            {
                Field field;
                auto colonPos = line.find(':');
                assert(colonPos != std::string::npos);
                field.name = line.substr(0, colonPos);
    
                std::string remaining{line.substr(colonPos)};
                std::string rangeStr;
                for(std::stringstream ss{remaining}; std::getline(ss, rangeStr, 'o');)
                {
                    std::stringstream rangeSs{rangeStr};
                    Range range{0, 0};
                    char dummy = 0;
                    rangeSs >> dummy;
                    rangeSs >> range.min;
                    rangeSs >> dummy;
                    rangeSs >> range.max;
                    field.ranges.push_back(range);
                }
                result.fields.push_back(std::move(field));
            }
            break;
            case State::OWN_TICKET:
            case State::OTHER_TICKETS:
            {
                result.tickets.push_back(Ticket::load(line));
            }
            break;
        }
    }
    return result;
}

bool Range::isInRange(Number value) const
{
    return min <= value && value <= max;
}

bool Field::isInRange(Number value) const
{
    bool matchesAny = std::any_of(ranges.begin(), ranges.end(), [value](const Range& range)
    {
        return range.isInRange(value);
    });
    return matchesAny;
}

bool Data::isValid(Number value) const
{
    for(const Field& field : fields)
    {
        if(field.isInRange(value))
        {
            return true;
        }
    }
    return false;
}

bool Data::isValid(const Ticket& ticket) const
{
    for(Number value : ticket.values)
    {
        if(isValid(value) == false)
        {
            return false;
        }
    }
    return true;
}

Number Data::calculateErrorRate() const
{
    Number result = 0;

    for(const Ticket& ticket : tickets)
    {
        for(Number value : ticket.values)
        {
            if(isValid(value) == false)
            {
                result += value;
            }
        }
    }
    return result;
}

Number getAnswerPart1(const std::filesystem::path& path)
{
    std::ifstream file{path};
    assert(file);
    Data data = Data::load(file);

    Number result = data.calculateErrorRate();
    return result;
}

void Data::removeInvalidTickets()
{
    auto it = std::remove_if(tickets.begin(), tickets.end(), [this](const Ticket& ticket)
    {
        return isValid(ticket) == false;
    });

    tickets.erase(it, tickets.end());
}

Number Data::calculateFields()
{
    removeInvalidTickets();

    const std::size_t numFields = tickets.front().values.size();

    std::vector<std::vector<std::size_t>> validFields; // Ticket value pos -> possible Fields

    for(std::size_t pos = 0; pos < numFields; ++pos)
    {
        std::size_t index = 0;
        for(const Field& field : fields)
        {

            ++index;
        }
    }
    return 0;
}


Number getAnswerPart2(const std::filesystem::path& path)
{
    std::ifstream file{path};
    assert(file);
    Data data = Data::load(file);

    

    Number result = data.calculateFields();
    return result;
}

void solve()
{
    std::filesystem::path inputPath{std::filesystem::current_path().parent_path()};
    std::filesystem::path testPath{std::filesystem::current_path().parent_path()};
    inputPath += "/data/PuzzleInput/Day16/input.txt";
    testPath += "/data/PuzzleInput/Day16/test";

    auto resultTest = getAnswerPart1(testPath);
    std::cout << "test part1: " << resultTest << "\n";
    assert(resultTest == 71);

    auto resultInput = getAnswerPart1(inputPath);
    std::cout << "real part1: " << resultInput << "\n";
    //assert(resultInput == 304);


    auto resultTest2 = getAnswerPart2(testPath);
    std::cout << "test part2: " << resultTest2 << "\n";
    //assert(resultTest2 == 208);

    auto resultInput2 = getAnswerPart2(inputPath);
    std::cout << "real part2: " << resultInput2 << "\n";
    //assert(resultInput2 == 3348493585827);

    int debug = 123;
}
}

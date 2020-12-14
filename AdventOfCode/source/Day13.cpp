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

namespace Day13
{

using Number = std::int64_t;

struct DepartureData
{
    Number earliest;
    std::vector<Number> busIds;
};

DepartureData load(std::istream& stream)
{
    DepartureData result;
    stream >> result.earliest;
    for(; stream;)
    {
        Number temp;
        if(stream.peek() == 'x')
        {
            char dummy;
            stream >> dummy;
            assert(dummy == 'x');
        }
        else
        {
            stream >> temp;
            if(stream)
            {
                result.busIds.push_back(temp);
            }
        }        
        char dummy = 0;
        stream >> dummy;
        assert(dummy == ',' || dummy == EOF || (bool)stream == false);
    }
    return result;
}

Number getAnswerPart1(const std::filesystem::path& path)
{
    std::ifstream file{path};
    assert(file);
    DepartureData departureData = load(file);

    Number id = -1;
    Number diff = std::numeric_limits<Number>::max();
    for(Number idCurrent : departureData.busIds)
    {
        //auto diffCurrent = (departureData.earliest + idCurrent) % idCurrent;
        auto diffCurrent = ((departureData.earliest / idCurrent) * idCurrent + idCurrent) - departureData.earliest;
        if(diffCurrent < diff)
        {
            diff = diffCurrent;
            id = idCurrent;
        }
    }

    Number result = id * diff;
    return result;
}

void part1()
{
    std::filesystem::path inputPath{std::filesystem::current_path().parent_path()};
    std::filesystem::path testPath{std::filesystem::current_path().parent_path()};
    inputPath += "/data/PuzzleInput/Day13/input.txt";
    testPath += "/data/PuzzleInput/Day13/test";

    auto resultTest = getAnswerPart1(testPath);
    std::cout << "test part1: " << resultTest << "\n";
    assert(resultTest == 295);

    auto resultInput = getAnswerPart1(inputPath);
    std::cout << "real part1: " << resultInput << "\n";
    assert(resultInput == 2382);

    int debug = 123;
}

struct Data
{
    Number id;
    Number offset;
};

std::vector<Data> load2(std::istream& stream)
{
    std::vector<Data> result;

    Number dummy;
    stream >> dummy;
    Number offset = 0;
    for(; stream; ++offset)
    {
        if(stream.peek() == 'x')
        {
            char dummy;
            stream >> dummy;
            assert(dummy == 'x');
        }
        else
        {
            Number busId = -1;
            stream >> busId;
            if(stream)
            {
                result.push_back({busId, offset});
            }
        }
        char dummy = 0;
        stream >> dummy;
        assert(dummy == ',' || dummy == EOF || (bool) stream == false);
    }
    return result;
}
bool check(Number t, const std::vector<Data>& data)
{
    for(const Data& item : data)
    {
        auto id = item.id;
        auto offset = item.offset;
        if((t + offset) % id != 0)
            return false;
    }
    return true;
}

Number getBruteForceT(const std::vector<Data>& data)
{
    Number firstId = data.front().id;
    int i = 0;
    for(std::uint64_t t = 100000000000*firstId; ; t += firstId)
    {
        if(check(t, data))
            return t;
        
        if(t > t + firstId)
            break;
        
    }
    assert(false);
    return -1;
}

Number doStuff(const std::vector<Data>& data)
{
    std::vector<Number> base;
    for(const auto& item : data)
    {
        base.push_back(item.id - item.offset);
    }

    Number result = 0;
    return result;
}

Number getAnswerPart2(const std::filesystem::path& path)
{
    std::ifstream file{path};
    assert(file);
    std::vector<Data> data = load2(file);

    Number result = getBruteForceT(data);
    //Number result = doStuff(data);
    return result;
}

void part2()
{
    std::filesystem::path inputPath{std::filesystem::current_path().parent_path()};
    std::filesystem::path testPath{std::filesystem::current_path().parent_path()};
    inputPath += "/data/PuzzleInput/Day13/input.txt";
    testPath += "/data/PuzzleInput/Day13/test";

    //auto resultTest = getAnswerPart2(testPath);
    //std::cout << "test part2: " << resultTest << "\n";
    //assert(resultTest == 1068781);

    auto resultInput = getAnswerPart2(inputPath);
    std::cout << "real part2: " << resultInput << "\n";
    //assert(resultInput == 2382);

    int debug = 123;
}

}

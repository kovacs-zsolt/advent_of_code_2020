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
#include <unordered_map>
#include <set>
#include <string>
#include <sstream>
#include <vector>

namespace Day14
{

struct Mask
{
    static Mask load(const std::string& line);

    std::uint64_t apply(std::uint64_t value) const;

    std::uint64_t mask0 = 0;
    std::uint64_t mask1 = 0;
};

std::uint64_t Mask::apply(std::uint64_t value) const
{
    value &= mask0;
    value |= mask1;
    return value;
}

Mask Mask::load(const std::string& line)
{
    assert(line.size() == 36);
    std::uint64_t mask0 = 0;
    std::uint64_t mask1 = 0;

    auto clearFirst = [](std::uint64_t& value)
    {
        constexpr std::uint64_t CLEAR_FIRST = static_cast<std::uint64_t>(-1) << 1; // 11110
        value = value & CLEAR_FIRST; // aaaa0
    };

    auto setFirst = [](std::uint64_t& value)
    {
        constexpr std::uint64_t SET_FIRST = 1; // 00001
        value = value | SET_FIRST; // aaaa1
    };

    for(char bit : line)
    {
        mask0 = mask0 << 1;
        mask1 = mask1 << 1;

        switch(bit)
        {
            case '0':
                clearFirst(mask0);
                clearFirst(mask1);
                break;
            case '1':
                setFirst(mask0);
                setFirst(mask1);
                break;
            case 'X':
                setFirst(mask0);
                clearFirst(mask1);
                break;
        }
    }
    return Mask{mask0, mask1};
}

struct Data
{
    struct ProgramLine
    {
        enum class Type{MASK, MEMORY, NUM};
        Type        type = Type::NUM;
        std::size_t index = -1;
    };
    struct MemoryEntry
    {
        std::uint64_t address = 0;
        std::uint64_t value = 0; 
    };
    std::vector<ProgramLine> program;
    std::vector<Mask> masks;
    std::vector<MemoryEntry> memory;
};

Data load(std::istream& stream)
{
    Data result;
    for(std::string line; std::getline(stream, line);)
    {
        if(line.find("mask") != std::string::npos)
        {
            const static std::string MASK_PREFIX = "mask = ";
            Mask mask = Mask::load(line.substr(MASK_PREFIX.size()));

            result.masks.push_back(mask);
            auto index = result.masks.size() - 1;
            result.program.push_back({Data::ProgramLine::Type::MASK, index});

        }
        else if(line.find("mem") != std::string::npos)
        {
            const static std::string MEM_ADDRESS_PREFIX = "mem[";
            std::stringstream ss(line.substr(MEM_ADDRESS_PREFIX.size()));

            std::uint64_t address = 0;
            ss >> address;
            assert(ss);
            char dummy = 0;
            ss >> dummy;
            ss >> dummy;
            
            //ss.ignore('=');
            assert(ss);
            std::uint64_t value = 0;
            ss >> value;
            //assert(ss);

            result.memory.push_back({address, value});
            auto index = result.memory.size() - 1;
            result.program.push_back({Data::ProgramLine::Type::MEMORY, index});
        }
        else
        {
            assert(false);
        }
    }
    return result;
}

std::uint64_t run(const Data& data)
{
    std::unordered_map<std::uint64_t, std::uint64_t> memory;
    Mask currentMask = Mask{};

    for(const Data::ProgramLine& programLine : data.program)
    {
        switch(programLine.type)
        {
            case Data::ProgramLine::Type::MASK:
            {
                currentMask = data.masks[programLine.index];
            }
            break;
            case Data::ProgramLine::Type::MEMORY:
            {
                const Data::MemoryEntry& memoryEntry = data.memory[programLine.index];
                memory[memoryEntry.address] = currentMask.apply(memoryEntry.value);
            }
            break;
        }
    }

    std::uint64_t result = 0;
    for(auto [address, value] : memory)
    {
        result += value;
    }
    return result;
}

std::uint64_t getAnswerPart1(const std::filesystem::path& path, std::function<std::uint64_t(const Data& data)> runFunc)
{
    std::ifstream file(path);
    assert(file);
    Data data = load(file);

    std::uint64_t result = runFunc(data);
    return result;
}

void test1()
{
    auto mask = Mask::load("XXXXXXXXXXXXXXXXXXXXXXXXXXXXX1XXXX0X");
    int debug = 123;
}

void part1()
{
    test1();

    std::filesystem::path inputPath{std::filesystem::current_path().parent_path()};
    std::filesystem::path testPath{std::filesystem::current_path().parent_path()};
    inputPath += "/data/PuzzleInput/Day14/input.txt";
    testPath += "/data/PuzzleInput/Day14/test";

    auto resultTest = getAnswerPart1(testPath, run);
    std::cout << "test part1: " << resultTest << "\n";
    //assert(resultTest == 165);

    auto resultInput = getAnswerPart1(inputPath, run);
    std::cout << "real part1: " << resultInput << "\n";
    assert(resultInput == 4886706177792);

    int debug = 123;
}


}

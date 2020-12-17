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

namespace Day14
{

struct Mask
{
    static Mask parse1(const std::string& line);

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

Mask Mask::parse1(const std::string& line)
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
        std::string address = 0;
        std::uint64_t value = 0; 
    };
    std::vector<ProgramLine> program;
    std::vector<std::string> masks;
    std::vector<MemoryEntry> memory;
};

void fill(std::string& value)
{
    const std::size_t fill = 36 - value.size();
    std::string result;
    result.resize(fill, '0');
    result += value;
    value = result;
}

Data load(std::istream& stream)
{
    Data result;
    for(std::string line; std::getline(stream, line);)
    {
        if(line.find("mask") != std::string::npos)
        {
            const static std::string MASK_PREFIX = "mask = ";
            std::string mask = line.substr(MASK_PREFIX.size());
            assert(mask.size() == 36);
            result.masks.push_back(std::move(mask));
            auto index = result.masks.size() - 1;
            result.program.push_back({Data::ProgramLine::Type::MASK, index});

        }
        else if(line.find("mem") != std::string::npos)
        {
            auto posOpen = line.find('[');
            auto posClose = line.find(']');
            auto posEqual = line.find('=');
            std::string address = line.substr(posOpen + 1, posClose - posOpen - 1);
            fill(address);
            assert(address.size() == 36);
            auto remaining = line.substr(posEqual + 1);
            std::stringstream ss(remaining);
            std::uint64_t value = 0;
            ss >> value;

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

std::uint64_t run1(const Data& data)
{
    std::unordered_map<std::uint64_t, std::uint64_t> memory;
    Mask currentMask = Mask{};

    for(const Data::ProgramLine& programLine : data.program)
    {
        switch(programLine.type)
        {
            case Data::ProgramLine::Type::MASK:
            {
                currentMask = Mask::parse1(data.masks[programLine.index]);
            }
            break;
            case Data::ProgramLine::Type::MEMORY:
            {
                const Data::MemoryEntry& memoryEntry = data.memory[programLine.index];
                const std::uint64_t address = std::stoll(memoryEntry.address);
                memory[address] = currentMask.apply(memoryEntry.value);
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

std::uint64_t getAnswer(const std::filesystem::path& path, std::function<std::uint64_t(const Data& data)> runFunc)
{
    std::ifstream file(path);
    assert(file);
    Data data = load(file);

    std::uint64_t result = runFunc(data);
    return result;
}

std::uint64_t getAnswerPart1(const std::filesystem::path& path)
{
    return getAnswer(path, run1);
}

std::vector<std::uint64_t> decodeMemoryAddresses(const std::string& mask, const std::string& address)
{
    std::vector<std::string> addresses;

    auto addBit = [](std::vector<std::string>& addresses, char bit)
    {
        assert(bit == '0' || bit == '1');
        if(addresses.empty())
        {
            addresses.push_back({bit});
        }
        else
        {
            for(std::string& value : addresses)
            {
                value += bit;
            }
        }
    };

    for(std::size_t i = 0; i < mask.size(); ++i)
    {
        char maskBit = mask[i];
        char addressBit = address[i];

        switch(maskBit)
        {
            case '0':
            {
                addBit(addresses, addressBit);
            }
            break;
            case '1':
            {
                addBit(addresses, '1');
            }
            break;
            case 'X':
            {
                std::vector<std::string> withZero{addresses};
                std::vector<std::string> withOne{addresses};
                addBit(withZero, '0');
                addBit(withOne, '1');

                addresses.clear();
                addresses.insert(addresses.end(), withZero.begin(), withZero.end());
                addresses.insert(addresses.end(), withOne.begin(), withOne.end());
            }
            break;
            default:
                assert(false);
        }
    }
    std::vector<std::uint64_t> result;

    for(const std::string& address : addresses)
    {
        result.push_back(std::stoll(address, nullptr, 2));
    }
    return result;
}

std::uint64_t run2(const Data& data)
{
    std::unordered_map<std::uint64_t, std::uint64_t> memory;
    std::string mask;

    for(const Data::ProgramLine& programLine : data.program)
    {
        switch(programLine.type)
        {
            case Data::ProgramLine::Type::MASK:
            {
                mask = data.masks[programLine.index];
            }
            break;
            case Data::ProgramLine::Type::MEMORY:
            {
                auto [address, value] = data.memory[programLine.index];
                std::string addressBinary = std::bitset<36>(std::stoi(address)).to_string();
                std::vector<std::uint64_t> decodedAddresses = decodeMemoryAddresses(mask, addressBinary);
                for(std::uint64_t decodedAddress : decodedAddresses)
                {
                    memory[decodedAddress] = value;
                }
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

std::uint64_t getAnswerPart2(const std::filesystem::path& path)
{
    return getAnswer(path, run2);
}

void test1()
{
    auto mask = Mask::parse1("XXXXXXXXXXXXXXXXXXXXXXXXXXXXX1XXXX0X");
    int debug = 123;
}

void solve()
{
    test1();

    std::filesystem::path inputPath{std::filesystem::current_path().parent_path()};
    std::filesystem::path testPath{std::filesystem::current_path().parent_path()};
    inputPath += "/data/PuzzleInput/Day14/input.txt";
    testPath += "/data/PuzzleInput/Day14/test";

    auto resultTest = getAnswerPart1(testPath);
    std::cout << "test part1: " << resultTest << "\n";
    assert(resultTest == 165);

    auto resultInput = getAnswerPart1(inputPath);
    std::cout << "real part1: " << resultInput << "\n";
    assert(resultInput == 4886706177792);



    std::filesystem::path testPath2{std::filesystem::current_path().parent_path()};
    testPath2 += "/data/PuzzleInput/Day14/test2";

    auto resultTest2 = getAnswerPart2(testPath2);
    std::cout << "test part2: " << resultTest2 << "\n";
    assert(resultTest2 == 208);

    auto resultInput2 = getAnswerPart2(inputPath);
    std::cout << "real part2: " << resultInput2 << "\n";
    assert(resultInput2 == 3348493585827);

    int debug = 123;
}


}

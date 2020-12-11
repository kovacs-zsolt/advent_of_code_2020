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

namespace Day8
{

struct Instruction
{
    std::string code;
    int parameter = 0;
};

using Instructions = std::vector<Instruction>;

Instructions load(std::istream& stream)
{
    Instructions result;
    for (std::string line; std::getline(stream, line);)
    {
        Instruction instruction;
        constexpr int MAGIC = 3;
        instruction.code = line.substr(0, MAGIC);
        std::stringstream ss(line.substr(MAGIC));
        ss >> instruction.parameter;
        result.push_back(instruction);
    }
    return result;
}

std::pair<std::int64_t, bool> accBeforeLoop(const Instructions& instructions)
{
    std::int64_t acc = 0;
    std::set<std::size_t> used;
    bool infinite = false;
    for (std::size_t i = 0; i < instructions.size();)
    {
        auto [dontCare, inserted] = used.insert(i);
        if (inserted == false)
        {
            infinite = true;
            break;
        }

        assert(i < instructions.size());
        const Instruction& instruction = instructions[i];
        if (instruction.code == "acc")
        {
            acc += instruction.parameter;
            ++i;
        }
        else if (instruction.code == "jmp")
        {
            i += instruction.parameter;
        }
        else if (instruction.code == "nop")
        {
            ++i;
        }
    }
    return {acc, infinite};
}

void part1()
{
    std::filesystem::path path{ std::filesystem::current_path().parent_path() };
    path += "/data/PuzzleInput/Day8/input.txt";
    //path += "/data/PuzzleInput/Day8/test";
    std::ifstream file{ path };
    Instructions instructions = load(file);
    auto [answer, infinite]= accBeforeLoop(instructions);
    assert(infinite);
    std::cout << "part1: " << answer << "\n"; // 1553
    int debug = 123;
}


bool needsNew(const Instructions& instructions, const std::size_t i)
{
    return instructions[i].code == "nop" || instructions[i].code == "jmp";
}

void flip(Instructions& instructions, const std::size_t i)
{
    if (instructions[i].code == "nop")
    {
        instructions[i].code = "jmp";
    }
    else if (instructions[i].code == "jmp")
    {
        instructions[i].code = "nop";
    }
}

void part2()
{
    std::filesystem::path path{ std::filesystem::current_path().parent_path() };
    path += "/data/PuzzleInput/Day8/input.txt";
    //path += "/data/PuzzleInput/Day8/test";
    std::ifstream file{ path };
    Instructions instructions = load(file);

    auto [answer, infinite] = accBeforeLoop(instructions);
    if (infinite == false)
    {
        std::cout << "part2: " << answer << "\n"; // 1877
        return;
    }
    for (std::size_t i = 0; i < instructions.size(); ++i)
    {
        if (needsNew(instructions, i))
        {
            flip(instructions, i);
            auto [answer, infinite] = accBeforeLoop(instructions);
            if (infinite == false)
            {
                std::cout << "part2: " << answer << "\n";
                return;
            }
            flip(instructions, i);
        }
    }
    int debug = 123;
}

}

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

namespace Day6
{

std::vector<std::vector<std::string>> readBatches(const std::filesystem::path& path)
{
    std::ifstream file{path};
    assert(file);
    std::vector<std::vector<std::string>> result;
    std::vector<std::string> temp;
    for(std::string line; std::getline(file, line);)
    {
        char next = file.peek();
        if(line.empty() == false)
        {
            temp.push_back(line);
        }

        if(next == '\n' || next == EOF)
        {
            result.push_back(std::move(temp));
            temp = {};
        }        
    }
    return result;
}

int countYes(const std::vector<std::string>& batch)
{
    std::set<char> answers;
    for (const std::string& line : batch)
    {
        for(char c: line)
        {
            answers.insert(c);
        }
    }
    return answers.size();
}

int countEveryoneYes(const std::vector<std::string>& batch)
{
    std::set<char> result;
    if (batch.empty() == false)
    {
        result.insert(batch[0].begin(), batch[0].end());
    }
    for(const std::string& line : batch)
    {
        std::vector<char> toErase;
        for (char c : result)
        {
            auto isInLine = std::find(line.begin(), line.end(), c) != line.end();
            if (isInLine == false)
            {
                toErase.push_back(c);
            }
        }
        for (char c : toErase)
        {
            result.erase(c);
        }
    }
    return result.size();
}

void part1()
{   
    std::filesystem::path path{ std::filesystem::current_path().parent_path() };
    path += "/data/PuzzleInput/Day6/input.txt";
    //path += "/data/PuzzleInput/Day6/test";
    auto batches = readBatches(path);
    int debug = 123;

    int sum = 0;
    for (const auto& batch : batches)
    {
        sum += countYes(batch);
    }
    std::cout << "part1: " << sum << "\n"; // 6680
}

void part2()
{
    std::filesystem::path path{ std::filesystem::current_path().parent_path() };
    path += "/data/PuzzleInput/Day6/input.txt";
    //path += "/data/PuzzleInput/Day6/test";
    auto batches = readBatches(path);
    int debug = 123;

    int sum = 0;
    for (const auto& batch : batches)
    {
        sum += countEveryoneYes(batch);
    }
    std::cout << "part2: " << sum << "\n"; // 3117
}

}

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

namespace Day7
{

struct Content
{
    int num;
    std::string color;
};

struct Bag
{
    std::vector<Content> contents;
    std::set<std::string> parents;
};

using Bags = std::map<std::string, Bag>;
// parse bag line
// minden baghez: lista a parentekről, amiben benne lehet

void parseBag(const std::string& line, Bags& bags)
{
    static const std::string MAGIC{"bags contain "};
    auto bagsPos = line.find(MAGIC);
    assert(bagsPos != std::string::npos);

    Bag bag;

    std::string color = line.substr(0, bagsPos - 1);
    std::stringstream parentsStream{line.substr(bagsPos + MAGIC.size() - 1)};
    auto foo = parentsStream.str();
    for(std::string part; std::getline(parentsStream, part, ',');)
    {
        if (part == " no other bags.")
        {
            break;
        }
        if (part[part.size() - 1] == '.')
        {
            part.erase(part.end() - 1);
        }
        std::stringstream ss{ part };
        Content content;
        ss >> content.num;
        ss >> std::ws;
        std::getline(ss, content.color);
        static const std::string BAG_MAGIC{" bag"};
        content.color = content.color.substr(0, content.color.find(BAG_MAGIC));
        bag.contents.push_back(content);
        bags[content.color].parents.insert(color);
    }
    bags[color].contents = bag.contents; // contents are loaded once, parents are loaded throughout the parse
}

Bags parseBags(std::ifstream& file)
{
    Bags result;
    for(std::string line; std::getline(file, line);)
    {
        parseBag(line, result);
    }
    return result;
}

void countRecursiveParents_internal(const std::string& color, const Bags& bags, std::set<std::string>& parents)
{
    if(bags.find(color) != bags.end())
    { 
        const Bag& bag = bags.at(color);
        for (const std::string& parentColor : bag.parents)
        {
            parents.insert(parentColor);
            countRecursiveParents_internal(parentColor, bags, parents);
        }
    }
}

int countRecursiveParents(const std::string& color, const Bags& bags)
{
    std::set<std::string> parents;
    countRecursiveParents_internal(color, bags, parents);
    return parents.size();
}

void part1()
{
    std::filesystem::path path{ std::filesystem::current_path().parent_path() };
    path += "/data/PuzzleInput/Day7/input.txt";
    //path += "/data/PuzzleInput/Day7/test";
    std::ifstream file{path};
    Bags bags = parseBags(file);

    static const std::string COLOR = "shiny gold";
    auto sum = countRecursiveParents(COLOR, bags);
    std::cout << "part1: " << sum << "\n"; // 300

    int debug = 1323;
}

int calculateNumBags_internal(const std::string& color, const Bags& bags)
{
    int result = 0;
    if (bags.find(color) != bags.end())
    {
        const Bag& bag = bags.at(color);
        ++result;
        for (const Content& content : bag.contents)
        {
            result += content.num * calculateNumBags_internal(content.color, bags);
        }
    }
    return result;
}

int calculateNumBags(const std::string& color, const Bags& bags)
{
    return calculateNumBags_internal(color, bags) - 1;
}

void part2()
{
    std::filesystem::path path{ std::filesystem::current_path().parent_path() };
    path += "/data/PuzzleInput/Day7/input.txt";
    //path += "/data/PuzzleInput/Day7/test";
    //path += "/data/PuzzleInput/Day7/test2";
    std::ifstream file{ path };
    Bags bags = parseBags(file);
    static const std::string COLOR = "shiny gold";
    auto num = calculateNumBags(COLOR, bags);
    std::cout << "part2: " << num << "\n";
    int debug = 123;
}

}
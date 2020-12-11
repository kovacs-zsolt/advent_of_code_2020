#include <cassert>

#include <filesystem>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

namespace Day3
{

struct Coord
{
	int right = 0;
	int down = 0;
	Coord& operator+=(const Coord& other)
	{
		right += other.right;
		down += other.down;
		return *this;
	}
};

struct Map
{
	constexpr static char EMPTY = '.';
	constexpr static char TREE = '#';	

	char get(const Coord& coord) const
	{
		assert(0 <= coord.down);
		assert(static_cast<size_t>(coord.down) < data.size());
		const std::string& line = data[coord.down];
		auto y = coord.right % line.size();
		return line[y];
	}

	std::vector<std::string> data;
};

Map loadMap(const std::filesystem::path path)
{
	std::ifstream file(path);
	Map map;
	for(std::string line; std::getline(file, line);)
	{
		map.data.push_back(line);
	}
	return map;
}

uint64_t countTrees(const Map& map, const Coord& start, const Coord& slope)
{
	uint64_t count = 0;
	Coord coord = start;
	//Coord coord{ 0, 0 };
	//Coord slope{ 3, 1 };
	for (;static_cast<std::size_t>(coord.down) < map.data.size(); coord += slope)
	{
		auto field = map.get(coord);
		if (field == Map::TREE)
			++count;
	}
	return count;
}

void part1()
{
	std::filesystem::path path{ std::filesystem::current_path().parent_path() };
	path += "/data/PuzzleInput/Day3/input.txt";
	Map map = loadMap(path);

	Coord slope{3, 1};
	Coord coord{ 0, 0 };

	auto t = map.get(coord);
	assert(map.get({ 0,0 }) == '.');
	assert(map.get(slope) == '.');

	auto numTrees = countTrees(map, { 0, 0 }, {3, 1}); //195
	std::cout << "numTrees: " << numTrees << "\n";

	auto sum = 
	countTrees(map, { 0, 0 }, { 1, 1 })*
	countTrees(map, { 0, 0 }, { 3, 1 })*
	countTrees(map, { 0, 0 }, { 5, 1 })*
	countTrees(map, { 0, 0 }, { 7, 1 })*
	countTrees(map, { 0, 0 }, { 1, 2 });
	std::cout << "sum: " << sum << "\n";
	// 3772314000
}

}

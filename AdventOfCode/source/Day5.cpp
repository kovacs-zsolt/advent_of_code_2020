#include <cassert>

#include <algorithm>
#include <filesystem>
#include <functional>
#include <fstream>
#include <iostream>
#include <map>
#include <numeric>
#include <string>
#include <sstream>
#include <vector>

namespace Day5
{

struct BoardingPass
{
	char toBinary(char bit) const
	{
		switch (bit)
		{
			case 'R':
			case 'B':
				return '1';
			case 'L':
			case 'F':
				return '0';
		}
		assert(false);
		return 0;
	}

	unsigned toInt() const
	{
		assert(raw.size() == 10);
		std::string temp;
		for (char bit : raw)
		{
			temp  += toBinary(bit);			
		}
		unsigned result{ static_cast<unsigned>(std::stol(temp, nullptr, 2)) };
		return result;
	}
	
	unsigned getRow() const
	{
		auto code = toInt();
		unsigned result = code >> 3;
		return result;
	}

	unsigned getColumn() const
	{
		auto code = toInt();
		unsigned MAGIC = (1 << 3) - 1;
		unsigned result = code & MAGIC;
		return result;
	}

	unsigned getCode() const
	{
		auto row = getRow();
		auto col = getColumn();
		unsigned result = row * 8 + col;
		return result;
	}

	std::string raw;
};

std::vector<BoardingPass> load()
{
	std::filesystem::path path{ std::filesystem::current_path().parent_path() };
	path += "/data/PuzzleInput/Day5/input.txt";
	std::ifstream file(path);

	std::vector<BoardingPass> result;
	for (std::string line; std::getline(file, line); result.push_back({ line }))
	{

	}
	return result;
}

void part1()
{
	std::vector<BoardingPass> boardingPasses{ load() };
	int debug = 123;
	unsigned maxCode = 0;
	for (const auto& boardingPass : boardingPasses)
	{
		auto code = boardingPass.getCode();
		if (code > maxCode)
		{
			maxCode = code;
		}
	}
	std::sort(boardingPasses.begin(), boardingPasses.end(), [](const auto& lhs, const auto& rhs)
	{
		return lhs.getCode() < rhs.getCode();
	});

	for (std::size_t i = 0; i < boardingPasses.size(); ++i)
	{
		if (i + 1 >= boardingPasses.size())
			continue;
		auto current = boardingPasses[i].getCode();
		auto next = boardingPasses[i + 1].getCode();
		//std::cout << "current: " << current << " next: " << next << "\n";
		if (next - current != 1)
		{
			std::cout << "own place: " << boardingPasses[i].getCode() + 1 << "\n"; // 599
		}
	}

	std::cout << "maxCode: " << maxCode << "\n"; // 850
}

}
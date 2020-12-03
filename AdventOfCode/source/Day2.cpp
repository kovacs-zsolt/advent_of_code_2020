#include <cassert>

#include <algorithm>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

namespace Day2
{
	struct Policy
	{
		int min;
		int max;
		char letter;
	};

	struct Line
	{
		Policy policy;
		std::string password;
	};


	std::vector<Line> load()
	{
		std::filesystem::path path{ std::filesystem::current_path().parent_path() };
		path += "/data/PuzzleInput/Day2/input.txt";
		std::ifstream file(path);
		assert(file);

		std::vector<Line> result;
		for (std::string rawLine; std::getline(file, rawLine);)
		{
			char temp{ 0 };
			std::istringstream ss(rawLine);
			Line line;
			ss >> line.policy.min;
			ss >> temp;
			assert(temp == '-');
			ss >> line.policy.max;
			ss >> line.policy.letter;
			ss >> temp;
			assert(temp == ':');
			ss >> line.password;
			result.push_back(line);
		}

		return result;
	}

	bool check_oldSledRentalPlace(const Line& line)
	{
		auto count = std::count(line.password.begin(), line.password.end(), line.policy.letter);
		return line.policy.min <= count && count <= line.policy.max;
	}

	bool check_new(const Line& line)
	{
		auto countMatchingChars = [](const std::string& password, int position, char letter)
		{
			return (password[position - 1] == letter) ? 1 : 0;
		};
		
		return
			countMatchingChars(line.password, line.policy.min, line.policy.letter) +
			countMatchingChars(line.password, line.policy.max, line.policy.letter) == 1;
	}

	void part1()
	{
		auto lines = load();
		//auto result = std::count_if(lines.begin(), lines.end(), [](const Line& line) {return check_oldSledRentalPlace(line); });   // 620
		auto result = std::count_if(lines.begin(), lines.end(), [](const Line& line) {return check_new(line); });					 // 727
		std::cout << result << "\n"; 
	}
}



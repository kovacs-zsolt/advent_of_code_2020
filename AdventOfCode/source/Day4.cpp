#include "Day4.h"

#include <cassert>

#include <algorithm>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <string>
#include <sstream>
#include <vector>

namespace Day4
{

bool isValid(const std::vector<std::string>& fields)
{
	std::vector required = { "byr", "iyr", "eyr", "hgt", "hcl", "ecl", "pid" };
	std::vector optional = { "cid" };

	auto allRequired = std::all_of(required.begin(), required.end(), [&fields](const std::string& requiredField)
	{
		auto hasRequired = std::find(fields.begin(), fields.end(), requiredField) != fields.end();
		return hasRequired;
	});


	auto isAllowed = std::all_of(fields.begin(), fields.end(), [&required, &optional](const auto& field)
	{
		auto isRequired = std::find(required.begin(), required.end(), field) != required.end();
		auto isOptional = std::find(optional.begin(), optional.end(), field) != optional.end();
		return isRequired || isOptional;
	});

	return allRequired;
	//return allRequired && isAllowed;
}
	
std::string fieldStr(const std::vector<std::string>& fields)
{
	std::stringstream ss;
	for (const auto& field : fields)
	{
		ss << field << ", ";
	}
	return ss.str();
}

std::uint64_t countBatches(std::filesystem::path path)
{
	std::ifstream file(path);
	std::string content(std::istreambuf_iterator<char>(file), {});
	std::uint64_t count = 0;
	for (std::size_t i = 0; i < content.size(); ++i)
	{
		if (content[i] == '\n')
		{
			if (i + 1 == content.size())
			{
				++count;
			}
			else if (content[i + 1] == '\n')
			{
				++count;
			}
		}
	}
	return count;
}

void part1()
{
	std::filesystem::path path{ std::filesystem::current_path().parent_path() };
	path += "/data/PuzzleInput/Day4/input.txt";
	std::ifstream file(path);

	int allCount = 0;
	int invalidAtleast7 = 0;
	int count = 0;
	
	std::vector<std::string> fields;

	for (std::string line; std::getline(file, line);)
	{
		if (!line.empty())
		{
			std::stringstream ss(line);
			for (;ss;)
			{
				std::string token;
				ss >> token;
				if (!ss)
					break;
				auto pos = token.find(':');
				auto field = token.substr(0, pos);
				fields.push_back(field);
			}
		}
		auto fileok = (bool)file;
		auto iseof = file.peek() == EOF;
		if(line.empty() || file.peek() == EOF)
		{
			std::sort(fields.begin(), fields.end());
			++allCount;
			if (auto valid = isValid(fields))
			{
				++count;
			}
			fields.clear();
		}
	}
	std::cout << "batches: " << countBatches(path) << "\n";
	std::cout << "valid: " << count << "\n"; // 191
	std::cout << "allCount: " << allCount << "\n"; // 191
}

}

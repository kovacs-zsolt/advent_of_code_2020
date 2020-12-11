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

namespace Day4
{

using namespace std::string_literals;
using Batch = std::map<std::string, std::string>;

bool validRange(const std::string& value, int size, int min, int max)
{
	if (value.size() != size)
		return false;
	try
	{
		int num = std::stoi(value);
		bool result = min <= num && num <= max;
		return result;
	}
	catch(...)
	{
		return false;
	}
}

bool hasAllRequiredFields(const Batch& batch)
{
	std::vector required = { "byr", "iyr", "eyr", "hgt", "hcl", "ecl", "pid" };
	std::vector optional = { "cid" };

	auto hasRequired = [](const Batch& batch, const std::string& requiredField)
	{
		auto hasRequired = batch.find(requiredField) != batch.end();
		return hasRequired;
	};

	auto hasAllRequired = std::all_of(required.begin(), required.end(), [&batch, hasRequired](const std::string& requiredField) {return hasRequired(batch, requiredField); });
	return hasAllRequired;
}

bool isValid(const Batch& batch)
{
	if (hasAllRequiredFields(batch) == false)
		return false;

	bool result = true;
	result &= validRange(batch.at({ "byr" }), 4, 1920, 2002);
	result &= validRange(batch.at({ "iyr" }), 4, 2010, 2020);
	result &= validRange(batch.at({ "eyr" }), 4, 2020, 2030);

	auto checkHeight = [](const Batch& batch)
	{
		const std::string& whole{ batch.at("hgt") };
		std::stringstream ss{ whole };
		int value = -1;
		ss >> value;
		if (whole.find("cm") != std::string::npos)
		{
			bool result = 150 <= value && value <= 193;
			return result;
		}
		else if (whole.find("in") != std::string::npos)
		{
			bool result = 59 <= value && value <= 76;
			return result;
		}
		return false;
	};

	result &= checkHeight(batch);

	auto checkHairColor = [](const Batch& batch)
	{
		const std::string& value{ batch.at("hcl") };

		bool result = value.size() == 7 && value[0] == '#';
		for (std::size_t i = 1; i < value.size(); ++i)
		{
			result &= ('0' <= value[i] && value[i] <= '9') ||
				('a' <= value[i] && value[i] <= 'f');
		}
		return result;
	};
	result &= checkHairColor(batch);

	auto checkEyeColor = [](const Batch& batch)
	{
		const std::string& value{ batch.at("ecl") };
		std::vector<std::string> values{ "amb", "blu", "brn", "gry", "grn", "hzl", "oth"};

		auto it = std::find(values.begin(), values.end(), value);
		bool result = it != values.end();
		return result;
	};
	result &= checkEyeColor(batch);

	auto checkPassportId = [](const Batch& batch)
	{
		const std::string& value{ batch.at("pid") };
		if (value.size() != 9)
			return false;
		for (char c : value)
		{
			bool isValid = '0' <= c && c <= '9';
			if(isValid == false)
				return false;
		}

		return true;
	};
	result &= checkPassportId(batch);

	return result;
}

std::vector<Batch> parseBatches(std::ifstream& file)
{
	std::vector<Batch> batches;

	Batch batch;
	for (std::string line; std::getline(file, line);)
	{
		if (!line.empty())
		{
			std::stringstream ss(line);
			for (; ss;)
			{
				std::string token;
				ss >> token;
				if (!ss)
					break;
				auto pos = token.find(':');
				auto field = token.substr(0, pos);
				auto value = token.substr(pos + 1);
				batch[field] = value;
			}
		}
		auto fileok = (bool)file;
		auto iseof = file.peek() == EOF;
		if (line.empty() || file.peek() == EOF)
		{
			batches.push_back(std::move(batch));
			batch = {};
		}
	}
	return batches;
}

void part1()
{
	//batches: 259
	//valid : 192
	//allCount : 259

	std::filesystem::path path{ std::filesystem::current_path().parent_path() };
	path += "/data/PuzzleInput/Day4/input.txt";
	std::ifstream file(path);

	std::vector<Batch> batches = parseBatches(file);
	auto allBatches = batches.size();
	assert(allBatches == 259);

	auto allRequiredBatches = std::count_if(batches.begin(), batches.end(), [](const Batch& batch) {return hasAllRequiredFields(batch); });
	assert(allRequiredBatches == 192);
	auto validBatches = std::count_if(batches.begin(), batches.end(), [](const Batch& batch) {return isValid(batch); });
	// nem 96, 105
	std::cout << "all batches: " << allBatches << "\n";
	std::cout << "valid batches: " << validBatches << "\n";

}

}

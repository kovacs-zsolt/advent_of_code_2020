#include <cassert>

#include <filesystem>
#include <fstream>
#include <iostream>
#include <vector>

std::vector<int> loadExpenseReport()
{
	std::filesystem::path path{ std::filesystem::current_path().parent_path() };
	path += "/data/PuzzleInput/Day1/input.txt";
	std::ifstream inputFile{ "../data/PuzzleInput/Day1/input.txt" };

	int MAGIC = 2020;

	std::vector<int> numbers;
	while (inputFile)
	{
		int temp = 0;
		inputFile >> temp;
		if (inputFile)
			numbers.push_back(temp);
	}
	return numbers;
}

void part1()
{
	const std::vector<int> numbers{ loadExpenseReport() };
	int MAGIC = 2020;
	int result = 0;
	for (std::size_t i = 0; i < numbers.size(); ++i)
	{
		for (std::size_t j = i; j < numbers.size(); ++j)
		{
			if (numbers[i] + numbers[j] == MAGIC)
			{
				assert(result == 0);
				result = numbers[i] * numbers[j];
			}
		}
	}
	std::cout << "part1 result: " << result << "\n"; // 326211
	assert(result == 326211);
}

void part2()
{
	const std::vector<int> numbers{ loadExpenseReport() };
	int MAGIC = 2020;
	int result = 0;
	for (std::size_t i = 0; i < numbers.size(); ++i)
	{
		for (std::size_t j = i; j < numbers.size(); ++j)
		{
			for (std::size_t k = j; k < numbers.size(); ++k)
			{
				if (numbers[i] + numbers[j] + numbers[k] == MAGIC)
				{
					assert(result == 0);
					result = numbers[i] * numbers[j] * numbers[k];
				}
			}
		}
	}
	std::cout << "part2 result: " << result << "\n"; // 131347190
}
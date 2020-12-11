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

namespace Day9
{

    using Number = std::uint64_t;
    using Numbers = std::vector<Number>;

    Numbers loadNumbers(std::istream& stream)
    {
        Numbers result;
        assert(stream);
        for (; stream;)
        {
            std::uint64_t temp = 0;
            stream >> temp;
            if (stream)
            {
                result.push_back(temp);
            }
        }
        return result;
    }

    std::set<Number> calculateValidValues(const Numbers& numbers, std::size_t beginIndex, std::size_t endIndex)
    {
        assert(beginIndex <= endIndex);
        assert(beginIndex < numbers.size());
        assert(endIndex <= numbers.size());

        std::set<Number> result;
        for (std::size_t i = beginIndex; i < endIndex; ++i)
        {
            for (std::size_t j = i; j < endIndex; ++j)
            {
                result.insert(numbers[i] + numbers[j]);
            }
        }
        return result;
    }

    std::tuple<bool, std::size_t, std::size_t> getContiguousRange(const Numbers& numbers, Number sum)
    {
        for (std::size_t i = 0; i < numbers.size(); ++i)
        {
            Number tempSum = 0;
            for (std::size_t j = i; j < numbers.size(); ++j)
            {
                tempSum += numbers[j];
                if (sum == tempSum)
                {
                    return {true, i, j };
                }
                if (sum < tempSum)
                {
                    break;
                }
            }
        }
        return {false, 0, 0};
    }

    Number sumSmallestAndLargest(const Numbers& numbers, std::size_t firstIndex, std::size_t lastIndex) // not begin-end, last is inclusive
    {
        Number min = std::numeric_limits<Number>::max();
        Number max = std::numeric_limits<Number>::min();
        for (std::size_t i = firstIndex; i <= lastIndex; ++i)
        {
            auto current = numbers[i];
            if (current < min)
                min = current;
            if (max < current)
                max = current;
        }
        return min + max;
    }

    bool isValid(const Numbers& numbers, std::size_t index, std::size_t preambleSize)
    {
        assert(index < numbers.size());
        assert(preambleSize <= index);
        std::size_t preambleBegin = index - preambleSize;
        std::size_t preambleEnd = index; // begin end like with iterators, end is after the last item
        auto validNumbers = calculateValidValues(numbers, preambleBegin, preambleEnd);

        auto value = numbers[index];
        bool result = validNumbers.find(value) != validNumbers.end();
        return result;
    }

    std::pair<bool, Number> getFirstInvalid(const Numbers& numbers, std::size_t preambleSize)
    {
        for (std::size_t i = preambleSize; i < numbers.size(); ++i)
        {
            if (isValid(numbers, i, preambleSize) == false)
            {
                return { true, numbers[i] };
            }
        }
        return { false, 0 };
    }

    void part1()
    {
        std::filesystem::path path{ std::filesystem::current_path().parent_path() };
        path += "/data/PuzzleInput/Day9/input.txt";
        //path += "/data/PuzzleInput/Day9/test";
        std::ifstream file{ path };
        assert(file);

        //auto PREAMBLE_SIZE = 5; // test
        auto PREAMBLE_SIZE = 25;

        auto numbers = loadNumbers(file);
        auto [foundFirstInvalid, firstInvalid] = getFirstInvalid(numbers, PREAMBLE_SIZE);
        assert(foundFirstInvalid);
        //assert(firstInvalid == 675280050);
        std::cout << "part1: " << firstInvalid << "\n";
        // part1: 675280050

        auto[foundRange, i, j] = getContiguousRange(numbers, firstInvalid);
        assert(foundRange);
        Number part2Result = sumSmallestAndLargest(numbers, i, j);
        //Number result = numbers[i] + numbers[j];
        std::cout << "part2: " << part2Result << "\n"; // 96081673

        int debug = 123;
    }

}
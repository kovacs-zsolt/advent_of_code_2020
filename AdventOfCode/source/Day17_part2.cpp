#include <cassert>
#define _USE_MATH_DEFINES
#include <cmath>

#include <algorithm>
#include <bitset>
#include <filesystem>
#include <functional>
#include <fstream>
#include <iostream>
#include <map>
#include <numeric>
#include <unordered_map>
#include <set>
#include <string>
#include <sstream>
#include <tuple>
#include <vector>

namespace Day17_part2
{

using Number = std::uint64_t;

using Coordinate = std::int64_t;

struct Point
{
    static std::tuple<Coordinate, Coordinate, Coordinate, Coordinate> toTuple(const Point& point);
    Coordinate x;
    Coordinate y;
    Coordinate z;
    Coordinate w;
};

std::tuple<Coordinate, Coordinate, Coordinate, Coordinate> Point::toTuple(const Point& point)
{
    return {point.x, point.y, point.z, point.w};
}

std::ostream& operator<<(std::ostream& stream, const Point& point)
{
    return stream << "(" << point.x << "," << point.y << "," << point.z << "," << point.w << ")";
}

bool operator<(const Point& lhs, const Point& rhs)
{
    return Point::toTuple(lhs) < Point::toTuple(rhs);
}

enum class Status { ACTIVE = '#', INACTIVE = '.' };

struct Boundary
{
    Point min;
    Point max;
};

using SpaceData = std::map<Point, Status>;

struct Space
{
public:
    Space(std::istream& stream) :space{load(stream)} {}
    Number countActiveCubes() const;
    void run(int numSteps);
    static void print(const SpaceData& spaceData, std::ostream& stream);

private:
    static Boundary getBounds(const SpaceData& spaceData);

    static std::map<Point, Status> load(std::istream& stream);

    Status getStatusInNextStep(const Point& point, Status status) const;

    Number countActiveNeighbours(const Point& point) const;

    Status getStatus(const Point& point) const;

    SpaceData createFrame() const;
    SpaceData step() const;

    SpaceData space;
};

std::map<Point, Status> Space::load(std::istream& stream)
{
    std::map<Point, Status> result;

    Coordinate x = 0;
    Coordinate y = 0;
    Coordinate z = 0;
    Coordinate w = 0;
    std::string line;
    for(y = 0; std::getline(stream, line); ++y)
    {
        for(x = 0; x < line.size(); ++x)
        {
            Point point{x, y, z, w};
            Status status{line[static_cast<std::size_t>(x)]};
            auto [it, inserted] = result.insert({point, status});
            assert(inserted);
        }        
    }
    return result;
}

Status Space::getStatus(const Point& point) const
{
    auto it = space.find(point);
    return it != space.end() ? it->second : Status::INACTIVE;
}

Number Space::countActiveNeighbours(const Point& point) const
{
    auto [x, y, z, w] = point;

    Number sum = 0;
    for(int i = -1; i <= 1; ++i)
    {
        for(int j = -1; j <= 1; ++j)
        {
            for(int k = -1; k <= 1; ++k)
            {
                for(int l = -1; l <= 1; ++l)
                {
                    if(i == 0 && j == 0 && k == 0 && l == 0)
                    {
                        continue;
                    }
                    Point neighbour = {x + i, y + j, z + k, w + l};
                    Status neighbourStatus = getStatus(neighbour);
                    if(neighbourStatus == Status::ACTIVE)
                    {
                        ++sum;
                    }
                }
            }
        }
    }
    return sum;
}

Boundary Space::getBounds(const SpaceData& spaceData)
{
    Coordinate minX = std::numeric_limits<Coordinate>::max(), maxX = std::numeric_limits<Coordinate>::min();
    Coordinate minY = std::numeric_limits<Coordinate>::max(), maxY = std::numeric_limits<Coordinate>::min();
    Coordinate minZ = std::numeric_limits<Coordinate>::max(), maxZ = std::numeric_limits<Coordinate>::min();
    Coordinate minW = std::numeric_limits<Coordinate>::max(), maxW = std::numeric_limits<Coordinate>::min();

    std::map<Point, Status> framePoints;
    for(const auto& [point, status] : spaceData)
    {
        auto [x, y, z, w] = point;
        if(x < minX)
            minX = x;
        if(y < minY)
            minY = y;
        if(z < minZ)
            minZ = z;
        if(w < minW)
            minW = z;

        if(x > maxX)
            maxX = x;
        if(y > maxY)
            maxY = y;
        if(z > maxZ)
            maxZ = z;
        if(w > maxW)
            maxW = w;
    }
    Boundary result;
    result.min = {minX, minY, minZ, minW};
    result.max = {maxX, maxY, maxZ, maxW};
    return result;
}

void Space::print(const SpaceData& spaceData, std::ostream& stream)
{
    auto [min, max] = getBounds(spaceData);
    auto [minX, minY, minZ, minW] = min;
    auto [maxX, maxY, maxZ, maxW] = max;
    stream << "min: " << min << " max: " << max << "\n";
    for(auto w = minW; w <= maxW; ++w)
    {
        stream << "W = " << w << ":\n";
        for(auto z = minZ; z <= maxZ; ++z)
        {
            stream << "Z = " << z << ", W = " << w << ":\n";

            for(auto y = minY; y <= maxY; ++y)
            {
                for(auto x = minX; x <= maxX; ++x)
                {
                    Point point{x, y, z};

                    auto it = spaceData.find(point);
                    Status status = it == spaceData.end() ? Status::INACTIVE : it->second;
                    stream << static_cast<char>(status);
                }
                stream << "\n";
            }
            stream << "\n";
        }
    }
}

std::map<Point, Status> Space::createFrame() const
{
    auto [min, max] = getBounds(space);
    auto [minX, minY, minZ, minW] = min;
    auto [maxX, maxY, maxZ, maxW] = max;
    std::map<Point, Status> framePoints;

    for(Coordinate y = minY - 1; y <= maxY + 1; ++y)
    {
        for(Coordinate z = minZ - 1; z <= maxZ + 1; ++z)
        {
            for(Coordinate w = minW - 1; w <= maxW + 1; ++w)
            {
                framePoints.insert({{minX - 1, y, z, w}, Status::INACTIVE});
                framePoints.insert({{maxX + 1, y, z, w}, Status::INACTIVE});
            }
        }
    }

    for(Coordinate x = minX - 1; x <= maxX + 1; ++x)
    {
        for(Coordinate z = minZ - 1; z <= maxZ + 1; ++z)
        {
            for(Coordinate w = minW - 1; w <= maxW + 1; ++w)
            {
                framePoints.insert({{x, minY - 1, z, w}, Status::INACTIVE});
                framePoints.insert({{x, maxY + 1, z, w}, Status::INACTIVE});
            }
        }
    }

    for(Coordinate x = minX - 1; x <= maxX + 1; ++x)
    {
        for(Coordinate y = minY - 1; y <= maxY + 1; ++y)
        {
            for(Coordinate w = minW - 1; w <= maxW + 1; ++w)
            {
                framePoints.insert({{x, y, minZ - 1, w}, Status::INACTIVE});
                framePoints.insert({{x, y, maxZ + 1, w}, Status::INACTIVE});
            }
        }
    }

    for(Coordinate x = minX - 1; x <= maxX + 1; ++x)
    {
        for(Coordinate y = minY - 1; y <= maxY + 1; ++y)
        {
            for(Coordinate z = minZ - 1; z <= maxZ + 1; ++z)
            {
                framePoints.insert({{x, y, z, minW - 1}, Status::INACTIVE});
                framePoints.insert({{x, y, z, maxW + 1}, Status::INACTIVE});
            }
        }
    }

    return framePoints;
}

Status Space::getStatusInNextStep(const Point& point, Status status) const
{
    auto activeNeighbours = countActiveNeighbours(point);
    Status newStatus;
    switch(status)
    {
        case Status::ACTIVE:
        {
            if(activeNeighbours == 2 || activeNeighbours == 3)
                newStatus = Status::ACTIVE;
            else
                newStatus = Status::INACTIVE;
        }
        break;


        case Status::INACTIVE:
        {
            if(activeNeighbours == 3)
                newStatus = Status::ACTIVE;
            else
                newStatus = Status::INACTIVE;
        }
        break;

        default:
            assert(false);
    }
    return newStatus;
}

std::map<Point, Status> Space::step() const
{
    auto framePoints = createFrame();
    auto spaceWithFrame{space};
    spaceWithFrame.insert(framePoints.begin(), framePoints.end());

    std::map<Point, Status> result;

    for(const auto& [point, status] : spaceWithFrame)
    {
        Status newStatus = getStatusInNextStep(point, status);
        auto [newIt, newInserted] = result.insert({point, newStatus});
        assert(newInserted);
    }
    return result;
}

void Space::run(int numSteps)
{
    for(int i = 0; i < numSteps; ++i)
    {
        std::cout << "cycle " << i << "\n";
        //print(space, std::cout);
        auto next = step();
        space = std::move(next);
    }
}

Number Space::countActiveCubes() const
{
    Number result = 0;
    for(const auto& [point, status] : space)
    {
        if(status == Status::ACTIVE)
            ++result;
    }
    return result;
}

Number getAnswerPart2(const std::filesystem::path& path)
{
    std::ifstream file{path};
    assert(file);
    Space space{file};
    space.run(6);

    Number result = space.countActiveCubes();
    return result;
}

void solve()
{
    std::filesystem::path inputPath{std::filesystem::current_path().parent_path()};
    std::filesystem::path testPath{std::filesystem::current_path().parent_path()};
    inputPath += "/data/PuzzleInput/Day17/input.txt";
    testPath += "/data/PuzzleInput/Day17/test";
    std::cout << "solve";
    auto resultTest2 = getAnswerPart2(testPath);
    std::cout << "test part2: " << resultTest2 << "\n";
    assert(resultTest2 == 848);

    auto resultInput2 = getAnswerPart2(inputPath);
    std::cout << "real part2: " << resultInput2 << "\n";
    assert(resultInput2 == 1868);

    int debug = 123;
}

}

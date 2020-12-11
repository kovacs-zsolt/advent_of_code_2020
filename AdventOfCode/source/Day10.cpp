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

namespace Day10
{

using i64 = std::int64_t;
using u64 = std::uint64_t;

std::set<u64> loadAdapters(std::istream& stream)
{
	std::set<u64> result;
	for (;;)
	{
		u64 temp = 0;
		stream >> temp;
		if (stream)
			result.insert(temp);
		else
			break;
	}
	return result;
}

void part1()
{
	std::filesystem::path path{std::filesystem::current_path().parent_path()};
	path += "/data/PuzzleInput/Day10/input.txt";
	//path += "/data/PuzzleInput/Day10/test";
	std::ifstream file{path};
	assert(file);
	std::set<u64> adapters = loadAdapters(file);
	std::map<u64, u64> diff;

	u64 prev = 0;
	for(u64 jolt : adapters)
	{
		++diff[jolt - prev];
		prev = jolt;
	}
	++diff[3];
    auto part1 = diff[1] * diff[3]; // 2100
	int debug = 123;
}


struct Node
{
	u64 jolt = 0;
	u64 pathCount = 0;
	std::vector<const Node*> children;
};

bool operator<(const Node& lhs, const Node& rhs)
{
	return lhs.jolt < rhs.jolt;
}

bool canConnect(u64 newJolt, u64 oldJolt)
{
	u64 min = oldJolt > 3 ? oldJolt - 3 : 0;
	return newJolt < oldJolt &&
		min <= newJolt;
}

u64 calculatePathCount(const Node& node)
{
	u64 result = std::accumulate(node.children.begin(), node.children.end(), (u64) 0, [](u64 value, const Node* child)
	{
		return child->pathCount + value;
	});
	return result;
}

u64 getNumberofPaths(const std::set<Node>& graph)
{
	u64 sum = 0;
	for(auto it = graph.begin(); it != graph.end(); ++it)
	{
		if(canConnect(0, it->jolt))
		{
			sum += it->pathCount;
		}
		else
		{
			break;
		}
	}
	return sum;
}

void part2()
{
	std::filesystem::path path{std::filesystem::current_path().parent_path()};
	path += "/data/PuzzleInput/Day10/input.txt";
	//path += "/data/PuzzleInput/Day10/test";
	//path += "/data/PuzzleInput/Day10/test2";
	std::ifstream file{path};
	assert(file);
	std::set<u64> adapters = loadAdapters(file);
	assert(adapters.empty() == false);

	u64 deviceJolt = *adapters.rbegin() + 3;

	std::set<Node> graph{Node{deviceJolt, 1, {}}};
	std::set<Node> dangling;
	for(auto itAdapter = adapters.rbegin(); itAdapter != adapters.rend(); ++itAdapter)
	{
		u64 adapterJolt = *itAdapter;
		Node node{adapterJolt, 0, {}};
		for(auto itGraph = graph.begin(); itGraph != graph.end(); ++itGraph)
		{
			const Node& graphNode = *itGraph;
			if(canConnect(adapterJolt, graphNode.jolt))
			{
				node.children.push_back(&graphNode);
			}
		}
		if(node.children.empty() == false)
		{
			node.pathCount = calculatePathCount(node);
			graph.insert(node);
		}
		else
		{
			dangling.insert(node);
		}
	}

	auto pathCount = getNumberofPaths(graph);
	std::cout << "part2: " << pathCount << "\n"; // 16198260678656
	int debug = 123;
}

}

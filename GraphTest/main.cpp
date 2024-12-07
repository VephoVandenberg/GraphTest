#include "src/core/util.hpp"
#include "src/core/graph.hpp"

using namespace Core;
using namespace Core::Util;

int main(int argc, char* argv[])
{
	Core::Graph<float> graph = generatePoints<float>(g_radius, g_nPoints);
	
	Core::Util::connectNodes(graph, 2, 6);

	for (auto& p : graph)
	{
		std::cout << p << '\n';
		std::cout << "And neighbours" << '\n';
		for (auto& n : p.neighbours)
		{
			std::cout << "To " << graph[n] << " d = " << Util::getDistance(p, graph[n]) << '\n';
		}
		std::cout << "\n\n";
	}

	std::vector <size_t> path;
	Core::Util::findHammiltonGreedy(0, 33, 10, graph, path);

	for (auto& p : path)
	{
		 
	}

	return 0;
}
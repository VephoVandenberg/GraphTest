#include "src/core/util.hpp"
#include "src/core/graph.hpp"

int main(int argc, char* argv[])
{
	Core::Graph<float> graph = Core::Util::generatePoints<float>(g_radius, g_nPoints);

	Core::Util::connectNodes(graph, 2, 6);

	std::vector <size_t> path;
	const float cost = Core::Util::dijkstra(graph, path, 0, 23);// Core::Util::findHammiltonGreedy(0, 33, 10, graph, path);

	for (auto& p : path)
	{
		std::cout << p << ' ';
	}

	return 0;
}
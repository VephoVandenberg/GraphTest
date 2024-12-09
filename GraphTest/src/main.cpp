﻿#include "core/util.hpp"
#include "core/graph.hpp"

#include "app/app.hpp"

constexpr size_t g_width = 1000;
constexpr size_t g_height = 1000;
constexpr size_t g_nPoints = 100;
constexpr size_t g_start = 0;
constexpr size_t g_target = 4;
constexpr float g_radius = 500;
constexpr float g_cost = 5.0f;

const std::string g_title = "Graph Test";

int main(int argc, char* argv[])
{
	Core::Graph<float> graph = Core::Util::generatePoints<float>(g_radius, g_nPoints);

	Core::Util::connectNodes(graph, 2, 6);

	std::vector <size_t> path;
	const float cost = Core::Util::dijkstra(graph, path, 0, 23, g_cost);

	for (auto& p : path)
	{
		std::cout << p << ' ';
	}
	std::cout << '\n';
	std::cout << "Total cost: " << cost << "$" << '\n';
	
	App::Application app(g_width, g_height, g_title);
	app.run(graph, path, 0, 55, g_radius);

	return 0;
}
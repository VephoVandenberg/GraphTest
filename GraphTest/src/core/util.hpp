#include <iostream>
#include <vector>
#include <random>
#include <math.h>
#include <map>

static constexpr size_t g_nPoints = 100;
static constexpr float g_radius = 60;
static constexpr float g_cost = 5.0f;

namespace Core
{
	template <typename T>
	struct Point;

	template <typename T>
	using Graph = std::vector<Point<T>>;

	namespace Util
	{
		template <typename T>
		float getDistance(const Point<T>& p1, const Point<T>& p2)
		{
			const float dxSquared = std::pow(p1.x - p2.x, 2);
			const float dySquared = std::pow(p1.y - p2.y, 2);
			return std::sqrt(dxSquared + dySquared);
		}

		template <typename T>
		Graph<T> generatePoints(const float radius, const size_t nPoints)
		{
			Graph<T> graph;
			std::random_device rd;
			std::mt19937 gen(rd());
			std::uniform_real_distribution<float> radiusDist(0.0f, 1.0f);
			std::uniform_real_distribution<float> angleDistr(0.0f, 2.0f * 3.14f);

			for (size_t iPoint = 0; iPoint < nPoints; iPoint++)
			{
				const float shrinkedRadius = radius * radiusDist(gen);
				const float alpha = angleDistr(gen);

				graph.push_back({
					shrinkedRadius * std::cos(alpha),
					shrinkedRadius * std::sin(alpha) });
			}

			return graph;
		}

		template<typename T>
		void connectNodes(Graph<T>& graph, 
			const size_t minNeighbours, const size_t maxNeighbours)
		{
			for (size_t iPoint = 0; iPoint < graph.size(); iPoint++)
			{
				std::vector<std::pair<float, size_t>> distToID;
				for (size_t jPoint = 0; jPoint < graph.size(); jPoint++)
				{
					if (iPoint != jPoint)
					{
						const float distance = getDistance(graph[iPoint], graph[jPoint]);
						distToID.push_back({ distance, jPoint });
					}
				}

				const size_t nNeighbours = std::min(maxNeighbours, std::max(minNeighbours, distToID.size()));
				std::sort(distToID.begin(), distToID.end());
				for (size_t iDist = 0; iDist < nNeighbours; iDist++)
				{
					graph[iPoint].neighbours.push_back(distToID[iDist].second);
				}
			}
		}

		template <typename T>
		float findHammiltonGreedy(const size_t start, const size_t target, const float costPerUnit, 
			const Graph<T> graph, std::vector<size_t>& path)
		{
			std::vector<bool> visited(graph.size(), false);
			size_t current = start;
			float totalCost = 0.0f;

			path.push_back(current);

			while (path.size() < graph.size())
			{
				float minDist = std::numeric_limits<float>::max();
				int32_t nextPoint = -1;
				for (auto neighbour : graph[current].neighbours)
				{
					if (!visited[neighbour] && neighbour != target)
					{
						const float dist = getDistance(graph[current], graph[neighbour]);
						if (dist < minDist)
						{
							minDist = dist;
							nextPoint = static_cast<int32_t>(neighbour);
						}
					}
				}
				if (path.size() == graph.size() - 1)
				{
					nextPoint = target;
					minDist = getDistance(graph[current], graph[target]);
				}
				if (nextPoint == -1)
				{
					std::cout << "Couldn't find " << '\n';
					break;
				}
				totalCost += costPerUnit * minDist;
				path.push_back(nextPoint);
				visited[nextPoint] = true;
				current = nextPoint;
			}

			return totalCost;
		}

		template <typename T>
		void nearestNeighbourTour(const size_t  start, const size_t target, const Graph<T>& graph, std::vector<size_t> path)
		{
			std::vector<size_t> visited(graph.size(), false);

		}
	}
}

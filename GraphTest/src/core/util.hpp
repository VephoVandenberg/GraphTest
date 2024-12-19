#pragma once

#include <iostream>
#include <vector>
#include <random>
#include <math.h>
#include <queue>
#include <map>

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

				std::sort(distToID.begin(), distToID.end());
				for (size_t iDist = 0; iDist < maxNeighbours && iDist < distToID.size(); iDist++)
				{
					graph[iPoint].neighbours.push_back(distToID[iDist].second);
				}
			}
		}

		// Not sure if it is working
		template <typename T>
		float findHammiltonGreedy(const Graph<T> graph, std::vector<size_t>& path, 
			const size_t start, const size_t target, const float costPerUnit)
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
		float dijkstra(const Graph<T> graph, std::vector<size_t>& path, const size_t start, const size_t target, const float cost)
		{
			std::vector<float> distances(graph.size(), std::numeric_limits<float>::max());
			std::vector<size_t> previous(graph.size(), static_cast<size_t>(-1));
			std::vector<bool> visited(graph.size(), false);
			distances[start] = 0.0f;

			using NodePair = std::pair<float, size_t>;
			std::priority_queue<NodePair, std::vector<NodePair>, std::greater<>> pq;
			pq.emplace(0.0f, start);

			while (!pq.empty()) 
			{
				const float currentDist = pq.top().first;
				const size_t currentNode = pq.top().second;
				pq.pop();

				if (visited[currentNode])
				{
					continue;
				}
				
				visited[currentNode] = true;

				for (const size_t neighbour : graph[currentNode].neighbours)
				{
					const float edgeWeight = getDistance(graph[currentNode], graph[neighbour]) * cost;
					const float newDist = currentDist + edgeWeight;

					if (newDist < distances[neighbour])
					{
						distances[neighbour] = newDist;
						previous[neighbour] = currentNode;
						pq.emplace(newDist, neighbour);
					}
				}
			}

			for (size_t at = target; at != static_cast<size_t>(-1); at = previous[at])
			{
				path.push_back(at);
			}
			std::reverse(path.begin(), path.end());

			return distances[target];
		}

		template <typename T>
		float TSPHeuristics(const Graph<T> graph, std::vector<size_t>& path, 
			const size_t start, const size_t target, const float costPerUnit)
		{
			std::vector<bool> visited(graph.size(), false);
			float totalCost = 0.0;

			size_t current = start;
			visited[current] = true;
			path.push_back(current);

			while (current != target) 
			{
				float minCost = std::numeric_limits<float>::max();
				size_t nextNode = static_cast<size_t>(-1);

				for (size_t i = 0; i < graph.size(); i++) 
				{
					if (!visited[i]) 
					{
						const float cost = getDistance(graph[current], graph[i]);
						if (cost < minCost) 
						{
							minCost = cost;
							nextNode = i;
						}
					}
				}

				visited[nextNode] = true;
				path.push_back(nextNode);
				totalCost += minCost;
				current = nextNode;
			}

			return totalCost * costPerUnit;
		}

		template <typename T>
		float TSPHeuristics(const Graph<T> graph, std::vector<size_t>& path, 
			const size_t start, const size_t target, 
			const float costPerUnit, const size_t minNodes, const float randomnessFactor)
		{
			std::vector<bool> visited(graph.size(), false);
			float totalCost = 0.0;

			size_t current = start;
			visited[current] = true;
			path.push_back(current);

			std::random_device rd;
			std::mt19937 gen(rd());

			while (current != target || path.size() < minNodes)
			{
				std::vector<std::pair<float, size_t>> candidates;

				for (size_t i = 0; i < graph.size(); i++)
				{
					if (!visited[i])
					{
						const float cost = getDistance(graph[i], graph[current]);
						candidates.emplace_back(cost, i);
					}
				}

				if (candidates.empty())
				{
					break;
				}

				std::sort(candidates.begin(), candidates.end());

				size_t range = std::min<size_t>(candidates.size(), static_cast<size_t>(randomnessFactor));
				std::uniform_int_distribution<size_t> dist(0, range - 1);
				size_t chosenIndex = dist(gen);

				size_t nextNode = candidates[chosenIndex].second;
				double chosenCost = candidates[chosenIndex].first;

				visited[nextNode] = true;
				path.push_back(nextNode);
				totalCost += chosenCost;
				current = nextNode;
			}

			while (path.size() < minNodes)
			{
				float minCost = std::numeric_limits<float>::max();
				size_t nextNode = static_cast<size_t>(-1);

				for (size_t i = 0; i < graph.size(); i++)
				{
					if (!visited[i])
					{
						const float cost = getDistance(graph[current], graph[i]);
						if (cost < minCost)
						{
							minCost = cost;
							nextNode = i;
						}
					}
				}

				if (nextNode == static_cast<size_t>(-1))
				{
					break;
				}

				visited[nextNode] = true;
				path.push_back(nextNode);
				totalCost += minCost;
				current = nextNode;
			}

			if (current != target)
			{
				totalCost += getDistance(graph[current], graph[target]);
				path.push_back(target);
			}

			return totalCost * costPerUnit;
		}
	}
}

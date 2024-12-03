#include <iostream>
#include <vector>
#include <random>
#include <math.h>

constexpr size_t g_nPoints = 100;
constexpr float g_radius = 60;

struct Point
{
	float x, y;

	friend std::ostream& operator<<(std::ostream& o, const Point& p);
};

std::ostream& operator<<(std::ostream& o, const Point& p)
{
	o << "{ x: " << p.x << ", y: " << p.y << "}";
	return o;
}

float getDistance(const Point& p1, const Point& p2)
{
	const float dxSquared = std::pow(p1.x - p2.x, 2);
	const float dySquared = std::pow(p1.y - p2.y, 2);
	return std::sqrt(dxSquared + dySquared);
}

std::vector<Point> generatePoints(const float radius, const size_t nPoints)
{
	std::vector<Point> points;
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_real_distribution<float> radiusDist(0.0f, 1.0f);
	std::uniform_real_distribution<float> angleDistr(0.0f, 2.0f * 3.14f);

	for (size_t iPoint = 0; iPoint < nPoints; iPoint++)
	{
		const float dr = radius * std::sqrt(radiusDist(gen));
		const float da = angleDistr(gen);

		points.push_back({ dr * std::cos(da), dr * std::sin(da) });
	}

	return points;
}

int main(int argc, char* argv[])
{
	std::vector<Point> points = generatePoints(g_radius, g_nPoints);
	for (auto& p : points)
	{
		std::cout << p << '\n';
	}

	return 0;
}
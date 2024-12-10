#pragma once

namespace Core
{
	template <typename T>
	struct Point
	{
		T x, y;
		std::vector<size_t> neighbours;

		bool operator==(const Point& p)
		{
			return x == p.x && y == p.y;
		}

		bool operator!=(const Point& p)
		{
			return !(*this == p);
		}

		friend std::ostream& operator<<(std::ostream& o, const Point& p)
		{
			o << "{ x: " << p.x << ", y: " << p.y << " }";
			return o;
		}
	};

	template <typename T>
	using Graph = std::vector<Point<T>>;
}
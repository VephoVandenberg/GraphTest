#pragma once

#include <memory>

#include <SFML/Graphics.hpp>

#include "../core/graph.hpp"
#include "../core/util.hpp"

namespace App
{
	class Application
	{
	public:
		Application(const size_t width, const size_t height, const std::string& title);

		void run(const Core::Graph<float>& graph, const std::vector<size_t>& path, 
			const size_t start, const size_t end, const float radius);

		~Application() = default;

		Application() = delete;
		Application(const Application&) = delete;
		Application& operator=(const Application&) = delete;
		Application(Application&&) = delete;
		Application& operator=(Application&&) = delete;

	private:
		void pollEvents();
		void draw(const Core::Graph<float>& graph, const std::vector<size_t>& path, 
			const size_t start, const size_t end, const float radius);

		std::unique_ptr<sf::RenderWindow> m_window = nullptr;

		const sf::Vector2f m_center;
	};
}


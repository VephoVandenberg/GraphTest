#include <SFML/Graphics/Font.hpp>

#include "app.hpp"

using namespace App;

Application::Application(const size_t width, const size_t height, const std::string& title)
    : m_center(width / 2, height / 2)
{
	m_window = std::make_unique<sf::RenderWindow>(sf::VideoMode(width, height), title);
}

void Application::run(const Core::Graph<float>& graph, const std::vector<size_t>& path, 
	const size_t start, const size_t end, const float radius)
{
	while (m_window->isOpen())
	{
		pollEvents();

        m_window->clear();
        draw(graph, path, start, end, radius);
        m_window->display();
	}
}

void Application::pollEvents()
{
	sf::Event e;
	while (m_window->pollEvent(e))
	{
		if (e.type == sf::Event::Closed)
		{
			m_window->close();
		}
	}
}

void Application::draw(const Core::Graph<float>& graph, const std::vector<size_t>& path,
    const size_t start, const size_t end, const float radius)
{
    sf::CircleShape circle(radius);
    circle.setPosition(m_center - sf::Vector2f(radius, radius));
    circle.setFillColor(sf::Color::Black);
    circle.setOutlineThickness(2);
    circle.setOutlineColor(sf::Color::White);

    m_window->draw(circle);

    sf::Text costText;
    sf::Font font;
    font.loadFromFile("arial.ttf");
    costText.setString("Test");
    costText.setFont(font);
    costText.setCharacterSize(24);

    m_window->draw(costText);

    for (const auto& point : graph) 
    {
        for (size_t neighbour : point.neighbours) 
        {
            sf::Vertex line[] = {
                sf::Vertex(m_center + sf::Vector2f(point.x, point.y)),
                sf::Vertex(m_center + sf::Vector2f(graph[neighbour].x, graph[neighbour].y))
            };
            m_window->draw(line, 2, sf::Lines);
        }
    }

    for (size_t iPoint = 1; iPoint < path.size(); iPoint++) 
    {
        size_t u = path[iPoint - 1];
        size_t v = path[iPoint];
        sf::Vertex line[] = {
            sf::Vertex(m_center + sf::Vector2f(graph[u].x, graph[u].y), sf::Color::Red),
            sf::Vertex(m_center + sf::Vector2f(graph[v].x, graph[v].y), sf::Color::Red)
        };
        m_window->draw(line, 2, sf::Lines);
    }

    for (const auto& point : graph) 
    {
        sf::CircleShape circle(3);
        circle.setPosition(m_center + sf::Vector2f(point.x - 3, point.y - 3));
        circle.setFillColor(sf::Color::Green);
        m_window->draw(circle);
    }
}

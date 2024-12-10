#include <SFML/Graphics/Font.hpp>

#include "app.hpp"

using namespace App;

Application::Application(const size_t width, const size_t height, const float radius, const std::string& title)
    : m_center(width / 2, height / 2)
    , m_circle(radius)
{
	m_window = std::make_unique<sf::RenderWindow>(sf::VideoMode(width, height), title);

    m_circle.setPosition(m_center - sf::Vector2f(radius, radius));
    m_circle.setFillColor(sf::Color::Black);
    m_circle.setOutlineThickness(2);
    m_circle.setOutlineColor(sf::Color::White);
}

void Application::run(const Core::Graph<float>& graph, const std::vector<size_t>& path, 
	const size_t start, const size_t end, const float totalCost)
{
	while (m_window->isOpen())
	{
		pollEvents();

        m_window->clear();
        draw(graph, path, start, end, totalCost);
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
    const size_t start, const size_t end, const float totalCost)
{    
    m_window->draw(m_circle);

    sf::Font font;
    font.loadFromFile("resources/Arial.ttf");

    sf::Text text;
    text.setFont(font);
    text.setString("Total cost " + std::to_string(totalCost) + "$");
    text.setFillColor(sf::Color::White);

    m_window->draw(text);

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

    for (size_t iPoint = 0; iPoint < graph.size(); iPoint++)
    {
        sf::CircleShape circle(5);
        circle.setPosition(m_center + sf::Vector2f(graph[iPoint].x - 5, graph[iPoint].y - 5));

        if (iPoint == path[0]) // Start node
        {        
            circle.setFillColor(sf::Color::Red);
        }
        else if (iPoint == path[path.size() - 1]) // end node
        {
            circle.setFillColor(sf::Color::Blue);
        }
        else
        {
            circle.setFillColor(sf::Color::Green);
        }
        m_window->draw(circle);
    }
}

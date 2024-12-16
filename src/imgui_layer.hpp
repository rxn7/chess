#pragma once

#include <SFML/Window/Event.hpp>
#include <SFML/Graphics/RenderWindow.hpp>

class ImGuiLayer {
public:
        ImGuiLayer(sf::RenderWindow &window);
	void handleEvent(const sf::Event &e);
	void update(sf::RenderWindow &window, const sf::Time &frameTime);
	void render(sf::RenderWindow &window);
};

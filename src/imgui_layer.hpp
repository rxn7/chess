#pragma once

#include "board.hpp"
#include "renderers/board_renderer.hpp"
#include <SFML/Window/Event.hpp>
#include <SFML/Graphics/RenderWindow.hpp>

class ImGuiLayer {
public:
	ImGuiLayer(sf::RenderWindow &window);
	~ImGuiLayer();

	void handleEvent(sf::RenderWindow &window, const sf::Event &e);
	void update(sf::RenderWindow &window, sf::Time frameTime);
	void render(sf::RenderWindow &window, Board &board, BoardRenderer &boardRenderer);
};

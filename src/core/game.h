#pragma once

#include "chess/board.h"
#include "chess/piece_renderer.h"
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/View.hpp>
#include <SFML/Window/Event.hpp>
#include <memory>

const sf::Color LIGHT_COLOR(0xf0d9b5ff);
const sf::Color DARK_COLOR(0xb58863ff);
const sf::Color CLEAR_COLOR(0x282828ff);

class Game {
public:
	Game();
	void start();

private:
	void handleEvent(const sf::Event &e);

private:
	sf::Font m_font;
	sf::View m_view;
	sf::RenderWindow m_window;
	std::unique_ptr<Board> m_board;
};
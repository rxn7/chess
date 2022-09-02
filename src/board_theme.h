#pragma once

#include <SFML/Graphics/Color.hpp>
#include <cstdlib>

struct BoardTheme {
	sf::Color lightColor;
	sf::Color darkColor;
	sf::Color lastMoveColor;
	sf::Color legalMoveColor;
	sf::Color outlineColor;

	static BoardTheme generateRandomTheme();
};

const BoardTheme DEFAULT_BOARD_THEME = {
	.lightColor = sf::Color(232, 235, 239),
	.darkColor = sf::Color(125, 135, 150),
	.lastMoveColor = sf::Color(100, 255, 100, 120),
	.legalMoveColor = sf::Color(200, 100, 100, 120),
	.outlineColor = sf::Color(0, 0, 0),
};
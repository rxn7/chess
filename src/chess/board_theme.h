#pragma once

#include <SFML/Graphics/Color.hpp>
#include <cstdlib>

struct BoardTheme {
	sf::Color lightColor;
	sf::Color darkColor;
	sf::Color highlightColor;
	sf::Color outlineColor;

	static BoardTheme generateRandomTheme();
};

const BoardTheme DEFAULT_BOARD_THEME = {
	.lightColor = sf::Color(232, 235, 239),
	.darkColor = sf::Color(125, 135, 150),
	.highlightColor = sf::Color(100, 255, 100, 120),
	.outlineColor = sf::Color(0, 0, 0),
};
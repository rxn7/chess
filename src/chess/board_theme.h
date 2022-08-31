#pragma once

#include <SFML/Graphics/Color.hpp>
#include <cstdlib>

struct BoardTheme {
	sf::Color lightColor;
	sf::Color darkColor;

	static BoardTheme generateRandomTheme() {
		return (BoardTheme){
			.lightColor = sf::Color(rand() % 127 + 127, rand() % 127 + 127, rand() % 127 + 127),
			.darkColor = sf::Color(rand() % 127, rand() % 127, rand() % 127),
		};
	}
};

const BoardTheme DEFAULT_BOARD_THEME = {
	.lightColor = sf::Color(0xffffffff),
	.darkColor = sf::Color(0x000000ff),
};
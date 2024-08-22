#include "board_theme.hpp"

BoardTheme BoardTheme::generateRandomTheme() {
	return (BoardTheme){
		.lightColor = sf::Color(rand() % 127 + 127, rand() % 127 + 127, rand() % 127 + 127),
		.darkColor = sf::Color(rand() % 127, rand() % 127, rand() % 127),
		.lastMoveColor = DEFAULT_BOARD_THEME.lastMoveColor,
		.legalMoveColor = DEFAULT_BOARD_THEME.legalMoveColor,
		.outlineColor = sf::Color(rand() % 255, rand() % 255, rand() % 255),
	};
}

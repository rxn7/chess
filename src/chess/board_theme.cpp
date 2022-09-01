#include "board_theme.h"

BoardTheme BoardTheme::generateRandomTheme() {
	return (BoardTheme){
		.lightColor = sf::Color(rand() % 127 + 127, rand() % 127 + 127, rand() % 127 + 127),
		.darkColor = sf::Color(rand() % 127, rand() % 127, rand() % 127),
		.highlightColor = DEFAULT_BOARD_THEME.highlightColor,
		.outlineColor = sf::Color(rand() % 50 + 205, rand() % 50 + 205, rand() % 50 + 205),
	};
}
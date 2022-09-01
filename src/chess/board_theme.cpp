#include "board_theme.h"

BoardTheme BoardTheme::generateRandomTheme() {
	return (BoardTheme){
		.lightColor = sf::Color(rand() % 127 + 127, rand() % 127 + 127, rand() % 127 + 127),
		.darkColor = sf::Color(rand() % 127, rand() % 127, rand() % 127),
		.highlightColor = sf::Color(rand() % 40, rand() % 50 + 205, rand() % 40, 120), 
		.outlineColor = sf::Color(rand() % 255, rand() % 255, rand() % 255),
	};
}
#pragma once

#include "board_theme.h"
#include <SFML/Graphics.hpp>
#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/VertexArray.hpp>
#include <array>

class BoardRenderer {
public:
	BoardRenderer(const sf::Font &font, const BoardTheme &theme = DEFAULT_BOARD_THEME);

	void renderCoords(sf::RenderWindow &window);
	void renderSquares(sf::RenderWindow &window);
	void setTheme(const BoardTheme &theme);
	inline const BoardTheme &getTheme() { return m_theme; }

private:
	void generateVa();
	void generateCoordTexts();
	void updateCoordTextsColors();

private:
	const sf::Font &m_font;
	sf::VertexArray m_vertexArray;
	sf::Text m_coordTexts[16];
	BoardTheme m_theme;
};
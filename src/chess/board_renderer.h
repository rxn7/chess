#pragma once

#include "board_theme.h"
#include "move.h"
#include <SFML/Graphics.hpp>
#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/VertexArray.hpp>
#include <array>

class BoardRenderer {
public:
	BoardRenderer(sf::RenderWindow &window, const sf::Font &font, const BoardTheme &theme = DEFAULT_BOARD_THEME);

	void renderCoords();
	void renderSquares();
	void setTheme(const BoardTheme &theme);
	void renderSquareLastMove(uint8_t idx);
	void renderSquareLegalMove(uint8_t idx);
	void renderSquareOutline(uint8_t idx);
	inline const BoardTheme &getTheme() const { return m_theme; }

private:
	void generateVa();
	void generateCoordTexts();
	void updateCoordTextsColors();

private:
	sf::RectangleShape m_squareHighlight, m_squareOutline;
	sf::RenderWindow &m_window;
	const sf::Font &m_font;
	sf::VertexArray m_vertexArray;
	sf::Text m_coordTexts[16];
	BoardTheme m_theme;
};
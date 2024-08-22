#pragma once

#include "../board_theme.hpp"

#include <SFML/Graphics.hpp>
#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/VertexArray.hpp>
#include <cstdint>

class BoardRenderer {
  public:
	BoardRenderer();

	void init(const sf::Font &font);
	void renderCoords(sf::RenderTarget &target);
	void renderSquares(sf::RenderTarget &target);
	void renderSquareLastMove(sf::RenderTarget &target, const std::uint8_t idx);
	void renderSquareLegalMove(sf::RenderTarget &target, const std::uint8_t idx);
	void renderSquareOutline(sf::RenderTarget &target, const std::uint8_t idx);
	void renderSquareCheck(sf::RenderTarget &target, const std::uint8_t idx);
	void setTheme(const BoardTheme &theme);

	inline const BoardTheme &getTheme() const {
		return m_theme;
	}

  private:
	void renderSquareHighlight(sf::RenderTarget &target, const std::uint8_t idx, const sf::Color &color);
	void generateVa();
	void generateCoordTexts(const sf::Font &font);
	void updateCoordTextsColors();

  private:
	BoardTheme m_theme;

	sf::VertexArray m_vertexArray;
	sf::Text m_coordTexts[16];
	sf::RectangleShape m_squareHighlight, m_squareOutline;
};

#pragma once

#include "board_theme.h"
#include "core/renderable.h"
#include <SFML/Graphics.hpp>
#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/VertexArray.hpp>
#include <array>

class BoardRenderer : Renderable {
public:
	BoardRenderer(const sf::Font &font, const BoardTheme &theme = DEFAULT_BOARD_THEME);

	void render(sf::RenderWindow &window) override;
	void setTheme(const BoardTheme &theme);

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
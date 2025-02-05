#include "board_renderer.hpp"
#include "board.hpp"

#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/PrimitiveType.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Text.hpp>
#include <SFML/Graphics/Vertex.hpp>
#include <SFML/System/String.hpp>
#include <SFML/System/Vector2.hpp>
#include <cstdint>

#define OUTLINE_THICKNESS 2

BoardRenderer::BoardRenderer()
	: m_vertexArray(sf::PrimitiveType::Quads, 64 * 4), m_squareHighlight({64, 64}), m_squareOutline({64 - OUTLINE_THICKNESS * 2, 64 - OUTLINE_THICKNESS * 2}) {
	m_squareOutline.setFillColor(sf::Color::Transparent);
	m_squareOutline.setOutlineThickness(OUTLINE_THICKNESS);
	setTheme(DEFAULT_BOARD_THEME);
}

void BoardRenderer::init(const sf::Font &font) {
	generateCoordTexts(font);
}

void BoardRenderer::renderSquares(sf::RenderTarget &target) {
	target.draw(m_vertexArray);
}

void BoardRenderer::renderCoords(sf::RenderTarget &target) {
	for(sf::Text &coordText : m_coordTexts)
		target.draw(coordText);
}

void BoardRenderer::renderSquareHighlight(sf::RenderTarget &target, std::uint8_t idx, const sf::Color &color) {
	if(!Board::isSquareIdxCorrect(idx))
		return;

	m_squareHighlight.setFillColor(color);
	m_squareHighlight.setPosition(idx % 8 * 64, idx / 8 * 64);
	target.draw(m_squareHighlight);
}

void BoardRenderer::renderSquareCheck(sf::RenderTarget &target, const std::uint8_t idx) {
	renderSquareHighlight(target, idx, sf::Color::Red);
}

void BoardRenderer::renderSquareLastMove(sf::RenderTarget &target, const std::uint8_t idx) {
	renderSquareHighlight(target, idx, m_theme.lastMoveColor);
}

void BoardRenderer::renderSquareLegalMove(sf::RenderTarget &target, const std::uint8_t idx) {
	renderSquareHighlight(target, idx, m_theme.legalMoveColor);
}

void BoardRenderer::renderSquareOutline(sf::RenderTarget &target, const std::uint8_t idx) {
	if(!Board::isSquareIdxCorrect(idx))
		return;

	m_squareOutline.setPosition(sf::Vector2f(idx % 8 * 64 + OUTLINE_THICKNESS, idx / 8 * 64 + OUTLINE_THICKNESS));
	target.draw(m_squareOutline);
}

void BoardRenderer::setTheme(const BoardTheme &theme) {
	m_theme = theme;
	m_squareOutline.setOutlineColor(theme.outlineColor);

	updateCoordTextsColors();
	generateVa();
}

void BoardRenderer::setFlipped(const bool flipped) {
	m_flipped = flipped;

	generateVa();
	generateCoordTexts(*m_coordTexts[0].getFont());
}

void BoardRenderer::updateCoordTextsColors() {
	for(std::uint8_t i = 0; i < 8; ++i) {
		sf::Text &text = m_coordTexts[i];
		const sf::Color &color = i % 2 != m_flipped ? m_theme.lightColor : m_theme.darkColor;
		text.setFillColor(color);
	}

	for(std::uint8_t i = 8; i < 16; ++i) {
		sf::Text &text = m_coordTexts[i];
		const sf::Color &color = i % 2 == m_flipped ? m_theme.lightColor : m_theme.darkColor;
		text.setFillColor(color);
	}
}

void BoardRenderer::generateCoordTexts(const sf::Font &font) {
	for(std::uint8_t rank = 0; rank < 8; ++rank) {
		m_coordTexts[rank] = sf::Text(std::to_string(rank + 1), font, 16);
		sf::Text &text = m_coordTexts[rank];

		const std::uint8_t realRank = m_flipped ? 7 - rank : rank;
		text.setPosition(0, realRank * 64);
	}

	for(std::uint8_t file = 0; file < 8; ++file) {
		m_coordTexts[8 + file] = sf::Text(sf::String((char)('a' + file)), font, 16);
		sf::Text &text = m_coordTexts[8 + file];

		const std::uint8_t realFile = m_flipped ? 7 - file : file;
		text.setPosition(realFile * 64 + 64 - 12, 512 - 20);
	}

	updateCoordTextsColors();
}

void BoardRenderer::generateVa() {
	m_vertexArray.clear();

	for(std::uint8_t c = 0; c < 8; ++c) {
		for(std::uint8_t r = 0; r < 8; ++r) {
			sf::Vertex vertex;
			vertex.texCoords = {};

			const bool isLight =(r + c) % 2 == 0;
			vertex.color = isLight ? m_theme.lightColor : m_theme.darkColor;

			// bot left
			vertex.position = sf::Vector2f(r * 64, c * 64);
			m_vertexArray.append(vertex);

			// bot right
			vertex.position = sf::Vector2f((r * 64) + 64, c * 64);
			m_vertexArray.append(vertex);

			// top right
			vertex.position = sf::Vector2f((r * 64) + 64,(c * 64) + 64);
			m_vertexArray.append(vertex);

			// top left
			vertex.position = sf::Vector2f(r * 64,(c * 64) + 64);
			m_vertexArray.append(vertex);
		}
	}
}

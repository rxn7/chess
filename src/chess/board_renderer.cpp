#include "board_renderer.h"
#include "chess/board.h"
#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/PrimitiveType.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Text.hpp>
#include <SFML/Graphics/Vertex.hpp>
#include <SFML/System/String.hpp>
#include <SFML/System/Vector2.hpp>
#include <atomic>
#include <cstdint>
#include <iostream>

#define OUTLINE_THICKNESS 2

BoardRenderer::BoardRenderer(sf::RenderWindow &window, const sf::Font &font, const BoardTheme &theme) : m_window(window), m_font(font), m_vertexArray(sf::PrimitiveType::Quads, 64*4), m_squareHighlight({64,64}), m_squareOutline({64-OUTLINE_THICKNESS*2,64-OUTLINE_THICKNESS*2}) {
	m_squareOutline.setFillColor(sf::Color::Transparent);
	m_squareOutline.setOutlineThickness(OUTLINE_THICKNESS);
	generateCoordTexts();
	setTheme(theme);
}

void BoardRenderer::renderSquares() {
	m_window.draw(m_vertexArray);
}

void BoardRenderer::renderCoords() {
	for(sf::Text &coordText : m_coordTexts)
		m_window.draw(coordText);
}

void BoardRenderer::renderSquareLastMove(uint8_t idx) {
	if(!Board::isSquareIdxCorrect(idx))
		return;

	m_squareHighlight.setFillColor(m_theme.lastMoveColor);
	m_squareHighlight.setPosition(sf::Vector2f((idx % 8) * 64, (idx / 8) * 64));
	m_window.draw(m_squareHighlight);
}

void BoardRenderer::renderSquareLegalMove(uint8_t idx) {
	if(!Board::isSquareIdxCorrect(idx))
		return;

	m_squareHighlight.setFillColor(m_theme.legalMoveColor);
	m_squareHighlight.setPosition(sf::Vector2f((idx % 8) * 64, (idx / 8) * 64));
	m_window.draw(m_squareHighlight);
}

void BoardRenderer::renderSquareOutline(uint8_t idx) {
	if(!Board::isSquareIdxCorrect(idx))
		return;

	m_squareOutline.setPosition(sf::Vector2f((idx % 8) * 64 + OUTLINE_THICKNESS, (idx / 8) * 64 + OUTLINE_THICKNESS));
	m_window.draw(m_squareOutline);
}

void BoardRenderer::setTheme(const BoardTheme &theme) {
	m_theme = theme;

	m_squareOutline.setOutlineColor(theme.outlineColor);

	generateVa();
	updateCoordTextsColors();
}

void BoardRenderer::updateCoordTextsColors() {
	for(auto i = 0; i<8; ++i) {
		sf::Text &text = m_coordTexts[i];
		sf::Color &color = i%2 != 0 ? m_theme.lightColor : m_theme.darkColor;
		text.setFillColor(color);
	}

	for(auto i = 8; i<16; ++i) {
		sf::Text &text = m_coordTexts[i];
		sf::Color &color = i%2 == 0 ? m_theme.lightColor : m_theme.darkColor;
		text.setFillColor(color);
	}
}

void BoardRenderer::generateCoordTexts() {
	// Ranks
	for(auto i=0; i<8; ++i) {
		sf::Text text(std::to_string(i+1), m_font, 16);
		text.setPosition(0, i*64);
		m_coordTexts[i] = text;
	}

	// Files
	for(auto i=0; i<8; ++i) {
		sf::Text text(sf::String((char)('a' + i)), m_font, 16);
		text.setPosition(i*64+64-12, 512-20);
		m_coordTexts[8+i] = text;
	}

	updateCoordTextsColors();
}

void BoardRenderer::generateVa() {
	m_vertexArray.clear();

	sf::Vertex vertex;
	vertex.texCoords = {};

	for(auto c=0; c<8; ++c) {
		for(auto r=0; r<8; ++r) {
			const bool isLight = (r + c) % 2 == 0;
			vertex.color = isLight ? m_theme.lightColor : m_theme.darkColor;

			// bot left
			vertex.position = sf::Vector2f(r*64, c*64);
			m_vertexArray.append(vertex);

			// bot right
			vertex.position = sf::Vector2f((r*64) + 64, c*64);
			m_vertexArray.append(vertex);

			// top right
			vertex.position = sf::Vector2f((r*64) + 64, (c*64) + 64);
			m_vertexArray.append(vertex);

			// top left
			vertex.position = sf::Vector2f(r*64, (c*64) + 64);
			m_vertexArray.append(vertex);
		}
	}
}

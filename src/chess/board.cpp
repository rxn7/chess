#include "board.h"
#include "chess/board_renderer.h"
#include "chess/board_theme.h"
#include "chess/piece.h"
#include <SFML/Graphics/RenderWindow.hpp>
#include <memory>

Board::Board(const sf::Font &font, const BoardTheme &theme) : m_boardRenderer(font, theme) {
	randomizePieces();
}

void Board::randomizePieces() {
	for(auto i=0; i<64; ++i)
		m_pieces[i] = (rand() % 6 + 1) | 8 * (rand() % 2 + 1);
}

void Board::render(sf::RenderWindow &window) {
	m_boardRenderer.renderSquares(window);
	renderPieces(window);
	m_boardRenderer.renderCoords(window);
}

void Board::renderPieces(sf::RenderWindow &window) {
	for(auto i=0; i<m_pieces.size(); ++i)
		m_pieceRenderer.renderPiece(window, m_pieces[i], i);
}
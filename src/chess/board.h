#pragma once

#include "board_renderer.h"
#include "chess/board_theme.h"
#include "chess/piece.h"
#include "chess/piece_renderer.h"
#include <SFML/Graphics/RenderWindow.hpp>
#include <array>
#include <memory>

class Board {
public:
	Board(const sf::Font &font, const BoardTheme &theme = DEFAULT_BOARD_THEME);
	void render(sf::RenderWindow &window);
	inline BoardRenderer &getBoardRenderer() { return m_boardRenderer; }
	void applyFen(const std::string &fen);

private:
	void renderHeldPiece(sf::RenderWindow &window);
	void renderPieces(sf::RenderWindow &window);

private:
	std::array<PieceValue, 64> m_pieces;
	PieceValue m_heldPiece = Piece::None;
	BoardRenderer m_boardRenderer;
	PieceRenderer m_pieceRenderer;
};
#pragma once

#include "board_renderer.h"
#include "chess/board_theme.h"
#include "chess/piece.h"
#include "chess/piece_renderer.h"
#include <SFML/Graphics/RenderWindow.hpp>
#include <array>
#include <memory>

#define DEFAULT_FEN "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR"

struct HeldPieceData {
	PieceValue value = 0;
	uint8_t previousIdx = 64;

	void reset() {
		value = 0;
		previousIdx = 64;
	}
};

class Board {
public:
	Board(sf::RenderWindow &window, const sf::Font &font, const BoardTheme &theme = DEFAULT_BOARD_THEME);
	void render();
	void handleEvent(const sf::Event &e);
	void applyFen(const std::string &fen);
	void resetBoard();

	inline BoardRenderer &getBoardRenderer() {
		 return m_boardRenderer; 
	}

private:
	void renderHeldPiece();
	void renderPieces();
	void handlePieceDrag();
	void handlePieceDrop();

private:
	sf::RenderWindow &m_window;
	std::array<PieceValue, 64> m_pieces;
	HeldPieceData m_heldPiece;
	BoardRenderer m_boardRenderer;
	PieceRenderer m_pieceRenderer;
};
#pragma once

#include "board_renderer.h"
#include "board_theme.h"
#include "piece.h"
#include "piece_renderer.h"
#include "move.h"
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Audio.hpp>
#include <array>
#include <memory>

#define DEFAULT_FEN "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1"

class Board {
public:
	Board(sf::RenderWindow &window, const sf::Font &font, const BoardTheme &theme = DEFAULT_BOARD_THEME);
	void render();
	void handleEvent(const sf::Event &e);
	void applyFen(const std::string &fen);
	bool moveHeldPiece(uint8_t toIdx);
	void processPawnPromotion(uint8_t idx);
	uint8_t getHoveredSquareIdx() const;
	void reset();

	inline BoardRenderer &getBoardRenderer() {
		 return m_boardRenderer; 
	}

	inline bool isAnyPieceHeld() const {
		return m_heldPieceIdx < 64 && !Piece::isNull(m_pieces[m_heldPieceIdx]);
	}

	static inline bool isSquareIdxCorrect(uint8_t idx) {
		return idx >= 0 && idx < 64;
	}

private:
	void renderHeldPiece();
	void renderPieces();
	void handlePieceDrag();
	void handlePieceDrop();

	inline void resetHeldPiece() {
		m_heldPieceIdx = 255; // every number above 63 and below 0 is ignored
		m_legalMoves.clear();
	}

private:
	std::vector<uint8_t> m_legalMoves;
	sf::RenderWindow &m_window;
	std::array<PieceValue, 64> m_pieces;
	uint8_t m_heldPieceIdx;
	BoardRenderer m_boardRenderer;
	PieceRenderer m_pieceRenderer;
	uint8_t m_turnColor;
	Move m_lastMove;
};
#pragma once

#include "board_renderer.h"
#include "board_theme.h"
#include "move.h"
#include "piece.h"
#include "piece_renderer.h"
#include <array>
#include <memory>
#include <SFML/Audio.hpp>
#include <SFML/Graphics/RenderWindow.hpp>

#define DEFAULT_FEN "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1"

class Board {
  public:
	Board();
	Board(const Board &board);

	void applyFen(const std::string &fen);
	void processPawnPromotion(std::uint8_t idx);
	void applyMove(const Move &move);
	void reset();
	std::pair<bool, std::uint8_t> isInCheck(PieceColor color);

	inline const std::array<Piece, 64> &getPieces() const {
		return m_pieces;
	}

	inline const Piece &getPiece(std::uint8_t idx) const {
		return m_pieces[idx];
	}

	static inline bool isSquareIdxCorrect(std::uint8_t idx) {
		return idx >= 0 && idx < 64;
	}

  private:
	Move m_lastMove;
	std::array<Piece, 64> m_pieces;

	friend class Game;
};

#pragma once

#include "renderers/board_renderer.h"
#include "renderers/piece_renderer.h"
#include "board_theme.h"
#include "move.h"
#include "piece.h"
#include <array>
#include <memory>
#include <SFML/Audio.hpp>
#include <SFML/Graphics/RenderWindow.hpp>

#define DEFAULT_FEN "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1"

struct CheckResult {
	bool isCheck;
	std::uint8_t kingIdx;
	std::uint8_t checkingPieceIdx;
};

class Board {
  public:
	Board();
	Board(const Board &board);

	void applyFen(const std::string &fen);
	void applyMove(const Move &move, const bool updateMoves = true, const bool updateCheckResult = true);
	void reset();
	CheckResult calculateCheck(const PieceColor color);

	inline const std::array<Piece, 64> &getPieces() const {
		return m_pieces;
	}

	inline const Piece &getPiece(const std::uint8_t idx) const {
		return m_pieces[idx];
	}

	static inline bool isSquareIdxCorrect(const std::uint8_t idx) {
		return idx >= 0 && idx < 64;
	}

	inline PieceColor getTurnColor() const {
		return m_turnColor;
	}

	inline CheckResult getCheckResult() const {
		return m_checkResult;
	}

	inline const Move &getLastMove() const {
		return m_lastMove;
	}

	inline std::vector<Move> &getLegalMoves() {
		return m_legalMoves;
	}

  private:
	void checkPawnPromotion(const Move &move);
	void updateLegalMoves();

  private:
	std::vector<Move> m_legalMoves;
	PieceColor m_turnColor;
	CheckResult m_checkResult;
	Move m_lastMove;
	std::array<Piece, 64> m_pieces;
};

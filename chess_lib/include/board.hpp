#pragma once

#include "debug_data.hpp"
#include "move.hpp"
#include "piece.hpp"
#include "board_state.hpp"

#include <array>
#include <cassert>
#include <optional> 
#include <vector> 

constexpr std::string_view DEFAULT_FEN = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";

struct CheckResult {
	bool isCheck;
	std::uint8_t kingIdx;
	std::uint8_t checkingPieceIdx;
};

enum class BoardStatus {
	Playing,
	Draw,
	WhiteWin,
	BlackWin
};

class Board {
friend class FEN;

public:
	Board();
	Board(const Board &board);
	void reset(bool applyDefaultFen = true);

	bool applyMove(const Move &move, const bool updateCheckResult = true);
	CheckResult fakeMove(const Move &move);
	CheckResult calculateCheck(const ChessColor color);
	const Player &getPlayer(const ChessColor color) const;

	// For non-const access
	inline Player &getPlayer(const ChessColor color) {
		return const_cast<Player &>(const_cast<const Board *>(this)->getPlayer(color));
	}

	inline BoardStatus getStatus() const {
		return m_status;
	}

	inline const DebugData &getDebugData() const {
		return m_debugData;
	}

	inline const std::array<Piece, 64> &getPieces() const {
		return m_pieces;
	}

	inline const Piece &getPiece(const std::uint8_t idx) const {
		assert(isSquareIdxCorrect(idx));
		return m_pieces[idx];
	}

	inline CheckResult getCheckResult() const {
		return m_checkResult;
	}

	inline const std::optional<Move> getLastMove() const {
		return m_lastMove;
	}

	inline const BoardState &getState() const {
		return m_state;
	}

	inline std::vector<Move> &getLegalMoves() {
		return m_legalMoves;
	}

	inline std::int16_t getMaterialBalance(const ChessColor color) const {
		return getPlayer(color).materialValue - getPlayer(OPPOSITE_COLOR(color)).materialValue;
	}

	static constexpr bool isSquareIdxCorrect(const std::uint8_t idx)  {
		return idx >= 0 && idx < 64;
	}

	static constexpr std::uint8_t getSquareIdx(const std::uint8_t file, const std::uint8_t rank) {
		return rank * 8 + file;
	}

	static constexpr std::uint8_t getSquareIdx(const std::string_view position) {
		uint8_t file = position[0] - 'a';
		uint8_t rank = '8' - position[1];
		return getSquareIdx(file, rank);
	}

private:
	void castle(ChessColor color, bool isQueenSide);
	bool handleCastling(const Move &move);
	void handlePawnPromotion(const Move &move);
	void handleEnPassant(const Move &move, const bool changeEnPassantTarget, const bool isFake);
	void updateLegalMoves();
	void updateStatus();

private:
	BoardStatus m_status = BoardStatus::Playing;
	DebugData m_debugData;

	std::optional<Move> m_lastMove;
	std::vector<Move> m_legalMoves;
	CheckResult m_checkResult;

	std::array<Piece, 64> m_pieces;
	BoardState m_state;
};

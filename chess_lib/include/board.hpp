#pragma once

#include "debug_data.hpp"
#include "move.hpp"
#include "piece.hpp"
#include "board_state.hpp"

#include <array>
#include <iostream>
#include <optional> 
#include <vector> 

#define DEFAULT_FEN "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1"

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

	CheckResult calculateCheck(const PieceColor color);

	inline const Player &getPlayer(const PieceColor color) const {
		switch(color) {
			case White: return m_state.whitePlayer;
			case Black: return m_state.blackPlayer;
		}

		std::cerr << "Invalid player color: " << (int)color << std::endl;
		return m_state.whitePlayer;
	}

	// For non-const access
	inline Player &getPlayer(const PieceColor color) {
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
		return m_pieces[idx];
	}

	static inline bool isSquareIdxCorrect(const std::uint8_t idx) {
		return idx >= 0 && idx < 64;
	}

	static inline std::uint8_t getSquareIdx(const std::string &position) {
		uint8_t rank = '8' - position[1];
		uint8_t file = position[0] - 'a';
		return rank * 8 + file;
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

private:
	void castle(PieceColor color, bool isQueenSide);
	bool handleCastling(const Move &move);
	void handlePawnPromotion(const Move &move);
	void handleEnPassant(const Move &move, const bool changeEnPassantTarget);
	void updateLegalMoves();
	void updateStatus();

private:
	BoardStatus m_status = BoardStatus::Playing;
	DebugData m_debugData;

	std::optional<Move> m_lastMove;
	CheckResult m_checkResult;
	std::vector<Move> m_legalMoves;

	std::array<Piece, 64> m_pieces;
	BoardState m_state;
};

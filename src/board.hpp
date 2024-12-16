#pragma once

#include "move.hpp"
#include "piece.hpp"
#include "board_state.hpp"

#include <SFML/Audio.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <array>
#include <format>
#include <iostream>
#include <optional> 

#define DEFAULT_FEN "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1"

class Game;

struct CheckResult {
	bool isCheck;
	std::uint8_t kingIdx;
	std::uint8_t checkingPieceIdx;
};

class Board {
  public:
	Board(Game &game);

	void reset();
	void applyFen(const std::string &fen);
	void convertToFen(std::string &fen) const; 
	void applyMove(const Move &move, const bool isFake = false, const bool updateCheckResult = true);
	void revertLastMove();
	void performCastling(PieceColor color, bool isQueenSide);
	void handleMove(const Move &move);
	void handlePawnPromotion(const Move &move);
	void handleEnPassant(const Move &move);
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

	inline const std::array<Piece, 64> &getPieces() const {
		return m_pieces;
	}

	inline const Piece &getPiece(const std::uint8_t idx) const {
		return m_pieces[idx];
	}

	static inline bool isSquareIdxCorrect(const std::uint8_t idx) {
		return idx >= 0 && idx < 64;
	}

	static inline std::uint8_t getSquareIdx(const char file, const char rank) {
		return (rank - '1') * 8 + (file - 'a');
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

	static inline std::string positionToString(const std::uint8_t idx) {
		return positionToString(idx % 8, idx / 8);
	}

	static inline std::string positionToString(const std::uint8_t x, const std::uint8_t y) {
		const char file = 'a' + x;
		const char rank = '8' - y;
		return std::format("{}{}", file, rank);
	}

  private:
	void updateLegalMoves();

  private:
	Game &m_game;
	std::optional<Move> m_lastMove;
	CheckResult m_checkResult;
	std::vector<Move> m_legalMoves;

	std::array<Piece, 64> m_pieces;
	std::array<Piece, 64> m_previousPieces;

	BoardState m_state;
	BoardState m_previousState;
};

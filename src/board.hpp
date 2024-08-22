#pragma once

#include "player.hpp"
#include "move.hpp"
#include "piece.hpp"

#include <SFML/Audio.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <array>
#include <format>
#include <optional>
#include <unordered_map>

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
	Board(const Board &board);

	void applyFen(const std::string &fen);
	void applyMove(const Move &move, const bool isFake = false, const bool updateCheckResult = true);
	void reset();
	void handleMove(const Move &move);
	void handleCastling(const Move &move);
	void handlePawnPromotion(const Move &move);
	void handleEnPassant(const Move &move);
	CheckResult calculateCheck(const PieceColor color);

	inline const Player &getPlayer(const PieceColor color) const {
		return m_players.at(color);
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

	inline PieceColor getTurnColor() const {
		return m_turnColor;
	}

	inline CheckResult getCheckResult() const {
		return m_checkResult;
	}

	inline const std::optional<Move> &getLastMove() const {
		return m_lastMove;
	}

	inline const std::optional<uint8_t> &getEnPassantTarget() const {
		return m_enPassantTarget;
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
	std::vector<Move> m_legalMoves;
	PieceColor m_turnColor;
	CheckResult m_checkResult;
	std::optional<Move> m_lastMove = std::nullopt;
	std::uint16_t m_fullMoves;
	std::uint16_t m_halfMoveClock;
	std::optional<uint8_t> m_enPassantTarget = std::nullopt;
	std::array<Piece, 64> m_pieces;
	std::unordered_map<PieceColor, Player> m_players;
};

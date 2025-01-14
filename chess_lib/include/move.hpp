#pragma once

#include "piece.hpp"
#include <iostream>
#include <format>

class Board;

struct Move {
	Move() { }
	Move(const Board &board, const std::uint8_t fromIdx, const std::uint8_t toIdx);

	constexpr bool operator==(const Move &other) const {
		return fromIdx == other.fromIdx && toIdx == other.toIdx;
	}

	friend inline std::ostream &operator<<(std::ostream &os, const Move &move) {
		os << positionToString(move.fromIdx) << "(" << move.piece.toChar() << ") -> " << positionToString(move.toIdx) << "(" << move.targetPiece.toChar() << ")";
		return os;
	}

	static inline std::string positionToString(const std::uint8_t idx) {
		return positionToString(idx % 8, idx / 8);
	}

	static inline std::string positionToString(const std::uint8_t x, const std::uint8_t y) {
		const char file = 'a' + x;
		const char rank = '8' - y;
		return std::format("{}{}", file, rank);
	}

	Piece piece = 0;
	Piece targetPiece = 0;

	bool isCapture = false;
	bool isPawnPromotion = false;
	bool isKingSideCastling = false;
	bool isQueenSideCastling = false;

	union {
		struct {
			std::uint8_t fromIdx;
			std::uint8_t toIdx;
		};

		uint8_t indices[2];
	};
};

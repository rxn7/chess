#pragma once

#include "piece.hpp"

class Board;

struct Move {
	Move() {
	}

	Move(const Board &board, const std::uint8_t fromIdx, const std::uint8_t toIdx);

	inline bool operator==(const Move &other) const {
		return fromIdx == other.fromIdx && toIdx == other.toIdx;
	}

	Piece piece;
	Piece targetPiece;

	bool isCapture;
	bool isPawnPromotion;
	bool isKingSideCastling;
	bool isQueenSideCastling;

	union {
		struct {
			std::uint8_t fromIdx;
			std::uint8_t toIdx;
		};

		uint8_t indices[2];
	};
};

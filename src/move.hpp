#pragma once

#include "piece.hpp"

class Board;

struct Move {
	Move() { }
	Move(const Board &board, const std::uint8_t fromIdx, const std::uint8_t toIdx);

	inline bool operator==(const Move &other) const {
		return fromIdx == other.fromIdx && toIdx == other.toIdx;
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

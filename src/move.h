#pragma once

#include "piece.h"
#include <cstdint>

struct Move {
	Move() {
		reset();
	}

	Move(Piece piece, std::uint8_t fromIdx, std::uint8_t toIdx) : piece(piece), fromIdx(fromIdx), toIdx(toIdx) {}

	inline bool operator==(const Move &other) const {
		return piece == other.piece && fromIdx == other.fromIdx && toIdx == other.toIdx;
	}

	inline void reset() {
		piece = fromIdx = toIdx = 0;
	}

	inline bool isNull() const {
		return piece.isNull();
	}

	Piece piece;

	union {
		struct {
			std::uint8_t fromIdx;
			std::uint8_t toIdx;
		};

		uint8_t indices[2];
	};
};

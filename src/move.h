#pragma once

#include "piece.h"
#include <cstdint>

struct Move {
	Move() {
		reset();
	}

	Move(Piece piece, std::uint8_t fromIdx, std::uint8_t toIdx) : piece(piece), fromIdx(fromIdx), toIdx(toIdx) {}

	inline void reset() {
		piece = 0;
		for (std::uint8_t &i : indices)
			i = 64;
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

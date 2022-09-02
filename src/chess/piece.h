#pragma once
#include <array>
#include <cstdint>
#include <vector>

typedef uint8_t PieceValue;

#define TYPE_MASK 0b00111
#define COLOR_MASK 0b11000
#define BLACK_MASK 0b10000
#define WHITE_MASK 0b01000

namespace Piece {
	constexpr uint8_t None = 0;
	constexpr uint8_t King = 1;
	constexpr uint8_t Queen = 2;
	constexpr uint8_t Bishop = 3;
	constexpr uint8_t Knight = 4;
	constexpr uint8_t Rook = 5;
	constexpr uint8_t Pawn = 6;

	constexpr uint8_t White = 8;
	constexpr uint8_t Black = 16;

	static inline bool isNull(PieceValue val) {
		return !(val & TYPE_MASK) || ((val & COLOR_MASK) != WHITE_MASK && (val & COLOR_MASK) != BLACK_MASK);
	}

	void getLegalMoves(uint8_t idx, std::array<PieceValue, 64> &pieces, std::vector<uint8_t> &moves);
}
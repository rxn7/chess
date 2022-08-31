#pragma once
#include <cstdint>

namespace Piece {
	constexpr uint8_t None = 0;
	constexpr uint8_t Queen = 1;
	constexpr uint8_t King = 2;
	constexpr uint8_t Rook = 3;
	constexpr uint8_t Knight = 4;
	constexpr uint8_t Bishop = 5;
	constexpr uint8_t Pawn = 6;
	constexpr uint8_t White = 8;
	constexpr uint8_t Black = 16;
}

typedef uint8_t PieceValue;

#define TYPE_MASK 0b00111
#define COLOR_MASK 0b11000
#define BLACK_MASK 0b10000
#define WHITE_MASK 0b01000
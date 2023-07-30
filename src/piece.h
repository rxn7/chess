#pragma once

#include <array>
#include <cstdint>
#include <vector>

#define TYPE_MASK 0b00111
#define COLOR_MASK 0b11000
#define BLACK_MASK 0b10000
#define WHITE_MASK 0b01000

enum PieceType : std::uint8_t {
	None = 0,
	King = 1,
	Queen = 2,
	Bishop = 3,
	Knight = 4,
	Rook = 5,
	Pawn = 6,
};

enum PieceColor : std::uint8_t {
	White = 8,
	Black = 16,
};

struct Piece {
	Piece() : value(0) {}
	Piece(std::uint8_t value) : value(value) {}
	explicit Piece(PieceColor color, PieceType type) : value((std::uint8_t)type | (std::uint8_t)color) {}
	operator std::uint8_t() const {
		return value;
	}

	inline PieceColor getColor() const {
		return (PieceColor)(value & COLOR_MASK);
	}
	inline PieceType getType() const {
		return (PieceType)(value & TYPE_MASK);
	}
	inline bool isColor(uint8_t color) const {
		return getColor() == color;
	}
	inline bool isType(uint8_t type) const {
		return getType() == type;
	}

	inline bool isNull() const {
		const std::uint8_t color = getColor();
		return getType() == PieceType::None || (color != WHITE_MASK && color != BLACK_MASK);
	}

	std::uint8_t value;
};

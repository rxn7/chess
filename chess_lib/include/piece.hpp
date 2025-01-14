#pragma once

#include <cstdint>

#define TYPE_MASK 0b00111
#define COLOR_MASK 0b11000

enum PieceType : std::uint8_t {
	None = 0,
	King = 1,
	Queen = 2,
	Bishop = 3,
	Knight = 4,
	Rook = 5,
	Pawn = 6,
};

enum ChessColor : std::uint8_t {
	White = 8,
	Black = 16,
};

#define OPPOSITE_COLOR(color) (color == White ? Black : White)

class Piece {
public:
	Piece() : value(0) {}
	Piece(std::uint8_t value) : value(value) {}

	explicit Piece(ChessColor color, PieceType type) : value((std::uint8_t)type | (std::uint8_t)color) {}

	char toChar() const;

	operator std::uint8_t() const {
		return value;
	}

	inline ChessColor getColor() const {
		return (ChessColor)(value & COLOR_MASK);
	}

	inline PieceType getType() const {
		return (PieceType)(value & TYPE_MASK);
	}

	inline bool isColor(ChessColor color) const {
		return getColor() == color;
	}

	inline bool isNotColor(ChessColor color) const {
		return getColor() != color;
	}

	inline bool isType(PieceType type) const {
		return getType() == type;
	}

	inline bool isNull() const {
		return value == 0;
	}

	inline std::uint8_t getMaterialValue() const {
		return getMaterialValue(getType());
	}

	static constexpr std::uint8_t getMaterialValue(PieceType type) {
		switch(type) {
			case PieceType::Pawn: return 1;
			case PieceType::Knight: return 3;
			case PieceType::Bishop: return 3;
			case PieceType::Rook: return 5;
			case PieceType::Queen: return 9;
			default: return 0;
		}
	}

public:
	std::uint8_t value;
};

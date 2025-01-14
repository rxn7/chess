#pragma once

#include "piece.hpp"
#include <unordered_map>
#include <array>

class Player {
public:
	Player(ChessColor color) : color(color), capturedPieces({{PieceType::Pawn, 0}, {PieceType::Knight, 0}, {PieceType::Bishop, 0}, {PieceType::Rook, 0}, {PieceType::Queen, 0}}) { }

	void capturePiece(const PieceType type);
	std::uint8_t calculateMaterialValue(const std::array<Piece, 64> &pieces);

public:
	ChessColor color;
	bool canCastleKingSide = true;
	bool canCastleQueenSide = true;

	std::uint8_t materialValue = 0;
	std::unordered_map<PieceType, std::uint8_t> capturedPieces;
};

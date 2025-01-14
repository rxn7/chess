#include "player.hpp"
#include "piece.hpp"
#include <cassert>

void Player::capturePiece(const PieceType type) {
	assert(capturedPieces.contains(type));
	++capturedPieces[type];
}

std::uint8_t Player::calculateMaterialValue(const std::array<Piece, 64> &pieces) {
	materialValue = 0;
	for(const Piece &piece : pieces) {
		if(piece.isColor(color)) {
			materialValue += piece.getMaterialValue();
		}
	}

	return materialValue;
}

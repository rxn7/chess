#include "board.hpp"

Move::Move(const Board &board, const std::uint8_t fromIdx, const std::uint8_t toIdx) :
	piece(board.getPiece(fromIdx)),
	targetPiece(board.getPiece(toIdx)),
	fromIdx(fromIdx),
	toIdx(toIdx)
{
	isCapture = !targetPiece.isNull();

	if(board.getState().enPassantTarget == toIdx) {
		isCapture = true;
		targetPiece = Piece(OPPOSITE_COLOR(piece.getColor()), Pawn);
	}

	isPawnPromotion = piece.isType(Pawn) && (toIdx / 8 == 0 || toIdx / 8 == 7);

	const Player &player = board.getPlayer(piece.getColor());

	isKingSideCastling = player.canCastleKingSide && piece.isType(King) && (toIdx == 62 || toIdx == 6);
	isQueenSideCastling = player.canCastleQueenSide && piece.isType(King) && (toIdx == 58 || toIdx == 2);
}

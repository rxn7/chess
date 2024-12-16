#include "rules.hpp"
#include "board.hpp"
 
namespace Rules {
	static void addMoveIfIsEnPassantTargetOrHasOpponentPiece(const AddLegalMoveContext &ctx, const std::uint8_t targetIdx) {
		if(addMoveIfHasOpponentPiece(ctx, targetIdx)) {
			return;
		}

		if(ctx.board.getState().enPassantTarget == targetIdx) {
			addLegalMove(ctx, targetIdx);
		}
	}

	ADD_LEGAL_MOVES_FUNC(Pawn) {
		if(ctx.piece.isColor(White)) {
			if(addMoveIfEmpty(ctx, ctx.pieceIdx - 8))
				if(ctx.pieceY == 6)
					addMoveIfEmpty(ctx, ctx.pieceIdx - 16);

			if(ctx.pieceX != 0)
				addMoveIfIsEnPassantTargetOrHasOpponentPiece(ctx, ctx.pieceIdx - 9);

			if(ctx.pieceX != 7)
				addMoveIfIsEnPassantTargetOrHasOpponentPiece(ctx, ctx.pieceIdx - 7);
		} else {
			if(addMoveIfEmpty(ctx, ctx.pieceIdx + 8))
				if(ctx.pieceY == 1)
					addMoveIfEmpty(ctx, ctx.pieceIdx + 16);

			if(ctx.pieceX != 7)
				addMoveIfIsEnPassantTargetOrHasOpponentPiece(ctx, ctx.pieceIdx + 9);

			if(ctx.pieceX != 0)
				addMoveIfIsEnPassantTargetOrHasOpponentPiece(ctx, ctx.pieceIdx + 7);
		}
	}
}

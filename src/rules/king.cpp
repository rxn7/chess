#include "../rules.hpp"
#include "../board.hpp"

namespace Rules {
	ADD_LEGAL_MOVES_FUNC(King) {
		if(ctx.pieceX < 7) {
			addMoveIfNotBlocked(ctx, ctx.pieceIdx + 1); // Right
			if(ctx.pieceY < 7) {
				addMoveIfNotBlocked(ctx, ctx.pieceIdx + 9); // Bottom Right
			}
			if(ctx.pieceY > 0) {
				addMoveIfNotBlocked(ctx, ctx.pieceIdx - 7); // Top Right
			}
		}
		if(ctx.pieceX > 0) {
			addMoveIfNotBlocked(ctx, ctx.pieceIdx - 1); // Left
			if(ctx.pieceY < 7) {
				addMoveIfNotBlocked(ctx, ctx.pieceIdx + 7); // Bottom Left
			}
			if(ctx.pieceY > 0) {
				addMoveIfNotBlocked(ctx, ctx.pieceIdx - 9); // Top Left
			}
		}

		if(ctx.pieceY < 7) {
			addMoveIfNotBlocked(ctx, ctx.pieceIdx + 8); // Bottom
		}
		if(ctx.pieceY > 0) {
			addMoveIfNotBlocked(ctx, ctx.pieceIdx - 8); // Top
		}

		const Player &player = ctx.board.getPlayer(ctx.piece.getColor());

		if(ctx.board.getCheckResult().isCheck) {
			return;
		}

		if(player.canCastleKingSide) {
			if(ctx.piece.getColor() == White) {
				if(ctx.board.getPiece(61).isNull() && ctx.board.getPiece(62).isNull()) {
					addLegalMove(ctx, 62);
				}
			} else {
				if(ctx.board.getPiece(5).isNull() && ctx.board.getPiece(6).isNull()) {
					addLegalMove(ctx, 6);
				}
			}
		}

		if(player.canCastleQueenSide) {
			if(ctx.piece.getColor() == White) {
				if(ctx.board.getPiece(57).isNull() && ctx.board.getPiece(58).isNull() && ctx.board.getPiece(59).isNull()) {
					addLegalMove(ctx, 58);
				}
			} else {
				if(ctx.board.getPiece(1).isNull() && ctx.board.getPiece(2).isNull() && ctx.board.getPiece(3).isNull()) {
					addLegalMove(ctx, 2);
				}
			}
		}
	}
}

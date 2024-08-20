#include "../rules.h"

namespace Rules {
	ADD_LEGAL_MOVES_FUNC(Pawn) {
		if(ctx.piece.isColor(White)) {
			if(addMoveIfEmpty(ctx, ctx.pieceIdx - 8))
				if(ctx.pieceY == 6)
					addMoveIfEmpty(ctx, ctx.pieceIdx - 16);

			if(ctx.pieceX != 0)
				addMoveIfHasOpponentPiece(ctx, ctx.pieceIdx - 9);

			if(ctx.pieceX != 7)
				addMoveIfHasOpponentPiece(ctx, ctx.pieceIdx - 7);
		} else {
			if(addMoveIfEmpty(ctx, ctx.pieceIdx + 8))
				if(ctx.pieceY == 1)
					addMoveIfEmpty(ctx, ctx.pieceIdx + 16);

			if(ctx.pieceX != 7)
				addMoveIfHasOpponentPiece(ctx, ctx.pieceIdx + 9);

			if(ctx.pieceX != 0)
				addMoveIfHasOpponentPiece(ctx, ctx.pieceIdx + 7);
		}
	}
}

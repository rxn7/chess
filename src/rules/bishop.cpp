#include "../rules.h"

namespace Rules {
	ADD_LEGAL_MOVES_FUNC(Bishop) {
		// Top Right
		for(std::uint8_t i = ctx.pieceIdx - 7; i >= ctx.pieceIdx - (7 - ctx.pieceX) * 7; i -= 7)
			if(addMoveIfHasOpponentPiece(ctx, i) || !addMoveIfEmpty(ctx, i))
				break;

		// Bottom Right
		for(std::uint8_t i = ctx.pieceIdx + 9; i <= ctx.pieceIdx + (7 - ctx.pieceX) * 9; i += 9)
			if(addMoveIfHasOpponentPiece(ctx, i) || !addMoveIfEmpty(ctx, i))
				break;

		// Top Left
		for(std::uint8_t i = ctx.pieceIdx - 9; i >= ctx.pieceIdx - ctx.pieceX * 9; i -= 9)
			if(addMoveIfHasOpponentPiece(ctx, i) || !addMoveIfEmpty(ctx, i))
				break;

		// Bottom Left
		for(std::uint8_t i = ctx.pieceIdx + 7; i <= ctx.pieceIdx + ctx.pieceX * 7; i += 7)
			if(addMoveIfHasOpponentPiece(ctx, i) || !addMoveIfEmpty(ctx, i))
				break;
	}
}

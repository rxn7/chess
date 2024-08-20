#include "../rules.h"

namespace Rules {
	ADD_LEGAL_MOVES_FUNC(Rook) {
		// Right
		for(std::uint8_t i = ctx.pieceIdx + 1; i < ctx.pieceY * 8 + 8; ++i)
			if(addMoveIfHasOpponentPiece(ctx, i) || !addMoveIfEmpty(ctx, i))
				break;

		// Left
		for(std::uint8_t i = ctx.pieceIdx - 1; i >= ctx.pieceY * 8; --i)
			if(addMoveIfHasOpponentPiece(ctx, i) || !addMoveIfEmpty(ctx, i))
				break;

		// Top
		for(std::uint8_t i = ctx.pieceIdx - 8; i >= ctx.pieceX; i -= 8)
			if(addMoveIfHasOpponentPiece(ctx, i) || !addMoveIfEmpty(ctx, i))
				break;

		// Bottom
		for(std::uint8_t i = ctx.pieceIdx + 8; i <= 56 + ctx.pieceX; i += 8)
			if(addMoveIfHasOpponentPiece(ctx, i) || !addMoveIfEmpty(ctx, i))
				break;
	}
}

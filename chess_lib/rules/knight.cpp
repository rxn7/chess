#include "rules.hpp"

namespace Rules {
	ADD_LEGAL_MOVES_FUNC(Knight) {
		if(ctx.pieceX < 6) {
			addMoveIfNotBlocked(ctx, ctx.pieceIdx + 10);
			addMoveIfNotBlocked(ctx, ctx.pieceIdx - 6);
		}

		if(ctx.pieceX != 7) {
			addMoveIfNotBlocked(ctx, ctx.pieceIdx + 17);
			addMoveIfNotBlocked(ctx, ctx.pieceIdx - 15);
		}

		if(ctx.pieceX > 1) {
			addMoveIfNotBlocked(ctx, ctx.pieceIdx + 6);
			addMoveIfNotBlocked(ctx, ctx.pieceIdx - 10);
		}

		if(ctx.pieceX != 0) {
			addMoveIfNotBlocked(ctx, ctx.pieceIdx - 17);
			addMoveIfNotBlocked(ctx, ctx.pieceIdx + 15);
		}
	}
}

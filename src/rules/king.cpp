#include "../rules.h"

namespace Rules {
	ADD_LEGAL_MOVES_FUNC(King) {
		constexpr std::uint8_t offsets[] = {1, 8, 7, 9};
		for(const std::uint8_t of : offsets) {
			addMoveIfNotBlocked(ctx, ctx.pieceIdx + of);
			addMoveIfNotBlocked(ctx, ctx.pieceIdx - of);
		}
	}
}

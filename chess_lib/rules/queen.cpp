#include "rules.hpp"

namespace Rules {
	ADD_LEGAL_MOVES_FUNC(Queen) {
		ADD_LEGAL_MOVES_FUNC_NAME(Rook)(ctx);
		ADD_LEGAL_MOVES_FUNC_NAME(Bishop)(ctx);
	}
}

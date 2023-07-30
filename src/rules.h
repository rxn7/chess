#pragma once

#include "move.h"
#include <vector>

class Board;

namespace Rules {
	std::vector<std::uint8_t> getLegalMoves(const Board &board, std::uint8_t idx);
}

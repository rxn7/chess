#pragma once

#include "move.h"
#include <vector>

class Board;

namespace Rules {
	void getLegalMoves(std::vector<std::uint8_t> &legalMoves, const Board &board, std::uint8_t idx);
}

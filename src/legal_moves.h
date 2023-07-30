#pragma once

#include "move.h"
#include <vector>

class Board;

void addLegalMoves(std::vector<std::uint8_t> &legalMoves, const Board &board, std::uint8_t idx, bool check = true);

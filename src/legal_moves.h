#pragma once

#include "move.h"
#include <vector>

class Board;

bool isMoveLegal(const std::vector<Move> &legalMoves, const Move &move);
void addLegalMoves(std::vector<Move> &legalMoves, const Board &board, const uint8_t idx, const bool ignoreCheck = false);

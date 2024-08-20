#pragma once

#include "piece.h"

struct Player {
	PieceColor color;
	bool canCastleKingSide;
	bool canCastleQueenSide;
};

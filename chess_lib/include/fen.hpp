#pragma once

#include <string>

class Board;

enum FenRecord {
	PiecePlacement = 0,
	ActiveColor = 1,
	CastlingAvailability = 2,
	EnPassantTargetSquare = 3,
	HalfMoveClock = 4,
	FullMoveNumber = 5
};


class FEN {
public:
	static bool applyFen(Board &board, const std::string_view fen);
	static std::string convertToFen(const Board &board);
};

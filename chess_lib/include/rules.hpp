#pragma once

#include "move.hpp"
#include <vector>
#include <functional>

class Board;

namespace Rules {
#define ADD_LEGAL_MOVES_FUNC_NAME(type) getLegalMoves##type
#define ADD_LEGAL_MOVES_FUNC(type) void ADD_LEGAL_MOVES_FUNC_NAME(type)(const Rules::AddLegalMoveContext &ctx)
#define ADD_LEGAL_MOVES_MAP_ENTRY(type) { type, ADD_LEGAL_MOVES_FUNC_NAME(type) }

	struct AddLegalMoveContext {
		std::vector<Move> &legalMoves;
		Board &board;
		Piece piece;
		std::uint8_t pieceIdx;
		std::uint8_t pieceX;
		std::uint8_t pieceY;
		bool ignoreCheck;
	};

	using AddLegalMovesFunc = std::function<void(const AddLegalMoveContext &ctx)>;

	bool isMoveLegal(const std::vector<Move> &legalMoves, const Move &move);
	bool isInCheckAfterMove(Board &board, const Move &move);
	void addLegalMove(const AddLegalMoveContext &ctx, const std::uint8_t targetIdx);
	bool addMoveIfEmpty(const AddLegalMoveContext &ctx, const std::uint8_t targetIdx);
	bool addMoveIfHasOpponentPiece(const AddLegalMoveContext &ctx, const std::uint8_t targetIdx);
	bool addMoveIfNotBlocked(const AddLegalMoveContext &ctx, const std::uint8_t targetIdx);
	void addLegalMoves(std::vector<Move> &legalMoves, Board &board, const std::uint8_t idx, const bool ignoreCheck = false);

	ADD_LEGAL_MOVES_FUNC(Pawn);
	ADD_LEGAL_MOVES_FUNC(Rook);
	ADD_LEGAL_MOVES_FUNC(Bishop);
	ADD_LEGAL_MOVES_FUNC(Knight);
	ADD_LEGAL_MOVES_FUNC(Queen);
	ADD_LEGAL_MOVES_FUNC(King);
}

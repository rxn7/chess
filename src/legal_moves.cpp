#include "legal_moves.h"
#include "board.h"
#include <functional>
#include <unordered_map>

struct AddLegalMoveContext {
	std::vector<std::uint8_t> &legalMoves;
	const Board &board;
	const Piece &piece;
	std::uint8_t pieceIdx;
	std::uint8_t pieceX;
	std::uint8_t pieceY;
	bool check;
};

using AddLegalMovesFunc = std::function<void(const AddLegalMoveContext &ctx)>;

#define ADD_LEGAL_MOVES_FUNC_NAME(type) getLegalMoves##type
#define ADD_LEGAL_MOVES_FUNC(type) static void ADD_LEGAL_MOVES_FUNC_NAME(type)(const AddLegalMoveContext &ctx)
#define ADD_LEGAL_MOVES_MAP_ENTRY(type)                                                                                                                                                                \
	{ type, ADD_LEGAL_MOVES_FUNC_NAME(type) }

ADD_LEGAL_MOVES_FUNC(Pawn);
ADD_LEGAL_MOVES_FUNC(Rook);
ADD_LEGAL_MOVES_FUNC(Bishop);
ADD_LEGAL_MOVES_FUNC(Knight);
ADD_LEGAL_MOVES_FUNC(Queen);
ADD_LEGAL_MOVES_FUNC(King);

static std::unordered_map<PieceType, AddLegalMovesFunc> s_getLegalMovesFuncMap{
	ADD_LEGAL_MOVES_MAP_ENTRY(Pawn),   ADD_LEGAL_MOVES_MAP_ENTRY(Rook),	 ADD_LEGAL_MOVES_MAP_ENTRY(Bishop),
	ADD_LEGAL_MOVES_MAP_ENTRY(Knight), ADD_LEGAL_MOVES_MAP_ENTRY(Queen), ADD_LEGAL_MOVES_MAP_ENTRY(King),
};

void addLegalMoves(std::vector<std::uint8_t> &legalMoves, const Board &board, std::uint8_t idx, bool check) {
	const Piece &piece = board.getPiece(idx);
	const std::uint8_t pieceX = idx % 8;
	const std::uint8_t pieceY = idx / 8;
	const AddLegalMoveContext ctx = {.legalMoves = legalMoves, .board = board, .piece = piece, .pieceIdx = idx, .pieceX = pieceX, .pieceY = pieceY, .check = check};

	legalMoves.clear();
	s_getLegalMovesFuncMap[piece.getType()](ctx);
}

bool isInCheckAfterMove(const Board &board, const Move &move) {
	if (move.piece.isNull())
		return false;

	Board boardTemp = board;
	boardTemp.applyMove(move, false);
	return boardTemp.getCheckResult(move.piece.getColor()).isCheck;
}

void addLegalMove(const AddLegalMoveContext &ctx, std::uint8_t targetIdx) {
	if (ctx.check && isInCheckAfterMove(ctx.board, Move(ctx.piece, ctx.pieceIdx, targetIdx)))
		return;

	ctx.legalMoves.push_back(targetIdx);
}

static bool addMoveIfEmpty(const AddLegalMoveContext &ctx, std::uint8_t targetIdx) {
	if (!Board::isSquareIdxCorrect(targetIdx) || !ctx.board.getPiece(targetIdx).isNull())
		return false;

	addLegalMove(ctx, targetIdx);
	return true;
}

static bool addMoveIfHasOpponentPiece(const AddLegalMoveContext &ctx, std::uint8_t targetIdx) {
	const Piece &targetPiece = ctx.board.getPiece(targetIdx);
	if (!Board::isSquareIdxCorrect(targetIdx) || targetPiece.isNull() || targetPiece.isColor(ctx.piece.getColor()))
		return false;

	addLegalMove(ctx, targetIdx);
	return true;
}

static bool addMoveIfNotBlocked(const AddLegalMoveContext &ctx, std::uint8_t targetIdx) {
	if (!Board::isSquareIdxCorrect(targetIdx))
		return false;

	const Piece &targetPiece = ctx.board.getPiece(targetIdx);
	if (!targetPiece.isNull() && ctx.piece.isColor(targetPiece.getColor()))
		return false;

	addLegalMove(ctx, targetIdx);
	return true;
}

ADD_LEGAL_MOVES_FUNC(Pawn) {
	if (ctx.piece.isColor(White)) {
		if (addMoveIfEmpty(ctx, ctx.pieceIdx - 8))
			if (ctx.pieceY == 6)
				addMoveIfEmpty(ctx, ctx.pieceIdx - 16);

		if (ctx.pieceX != 0)
			addMoveIfHasOpponentPiece(ctx, ctx.pieceIdx - 9);

		if (ctx.pieceX != 7)
			addMoveIfHasOpponentPiece(ctx, ctx.pieceIdx - 7);
	} else {
		if (addMoveIfEmpty(ctx, ctx.pieceIdx + 8))
			if (ctx.pieceY == 1)
				addMoveIfEmpty(ctx, ctx.pieceIdx + 16);

		if (ctx.pieceX != 7)
			addMoveIfHasOpponentPiece(ctx, ctx.pieceIdx + 9);

		if (ctx.pieceX != 0)
			addMoveIfHasOpponentPiece(ctx, ctx.pieceIdx + 7);
	}
}

ADD_LEGAL_MOVES_FUNC(Rook) {
	// Right
	for (std::uint8_t i = ctx.pieceIdx + 1; i < ctx.pieceY * 8 + 8; ++i)
		if (addMoveIfHasOpponentPiece(ctx, i) || !addMoveIfEmpty(ctx, i))
			break;

	// Left
	for (std::uint8_t i = ctx.pieceIdx - 1; i >= ctx.pieceY * 8; --i)
		if (addMoveIfHasOpponentPiece(ctx, i) || !addMoveIfEmpty(ctx, i))
			break;

	// Top
	for (std::uint8_t i = ctx.pieceIdx - 8; i >= ctx.pieceX; i -= 8)
		if (addMoveIfHasOpponentPiece(ctx, i) || !addMoveIfEmpty(ctx, i))
			break;

	// Bottom
	for (std::uint8_t i = ctx.pieceIdx + 8; i <= 56 + ctx.pieceX; i += 8)
		if (addMoveIfHasOpponentPiece(ctx, i) || !addMoveIfEmpty(ctx, i))
			break;
}

ADD_LEGAL_MOVES_FUNC(Bishop) {
	// Top Right
	for (std::uint8_t i = ctx.pieceIdx - 7; i >= ctx.pieceIdx - (7 - ctx.pieceX) * 7; i -= 7)
		if (addMoveIfHasOpponentPiece(ctx, i) || !addMoveIfEmpty(ctx, i))
			break;

	// Bottom Right
	for (std::uint8_t i = ctx.pieceIdx + 9; i <= ctx.pieceIdx + (7 - ctx.pieceX) * 9; i += 9)
		if (addMoveIfHasOpponentPiece(ctx, i) || !addMoveIfEmpty(ctx, i))
			break;

	// Top Left
	for (std::uint8_t i = ctx.pieceIdx - 9; i >= ctx.pieceIdx - ctx.pieceX * 9; i -= 9)
		if (addMoveIfHasOpponentPiece(ctx, i) || !addMoveIfEmpty(ctx, i))
			break;

	// Bottom Left
	for (std::uint8_t i = ctx.pieceIdx + 7; i <= ctx.pieceIdx + ctx.pieceX * 7; i += 7)
		if (addMoveIfHasOpponentPiece(ctx, i) || !addMoveIfEmpty(ctx, i))
			break;
}

ADD_LEGAL_MOVES_FUNC(Knight) {
	if (ctx.pieceX < 6) {
		addMoveIfNotBlocked(ctx, ctx.pieceIdx + 10);
		addMoveIfNotBlocked(ctx, ctx.pieceIdx - 6);
	}

	if (ctx.pieceX != 7) {
		addMoveIfNotBlocked(ctx, ctx.pieceIdx + 17);
		addMoveIfNotBlocked(ctx, ctx.pieceIdx - 15);
	}

	if (ctx.pieceX > 1) {
		addMoveIfNotBlocked(ctx, ctx.pieceIdx + 6);
		addMoveIfNotBlocked(ctx, ctx.pieceIdx - 10);
	}

	if (ctx.pieceX != 0) {
		addMoveIfNotBlocked(ctx, ctx.pieceIdx - 17);
		addMoveIfNotBlocked(ctx, ctx.pieceIdx + 15);
	}
}

ADD_LEGAL_MOVES_FUNC(Queen) {
	ADD_LEGAL_MOVES_FUNC_NAME(Rook)(ctx);
	ADD_LEGAL_MOVES_FUNC_NAME(Bishop)(ctx);
}

ADD_LEGAL_MOVES_FUNC(King) {
	constexpr std::uint8_t offsets[] = {1, 8, 7, 9};
	for (const std::uint8_t of : offsets) {
		addMoveIfNotBlocked(ctx, ctx.pieceIdx + of);
		addMoveIfNotBlocked(ctx, ctx.pieceIdx - of);
	}
}

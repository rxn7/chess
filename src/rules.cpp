#include "rules.h"
#include "board.h"
#include <functional>
#include <unordered_map>

#define ADD_LEGAL_MOVES_FUNC_PARAMS std::vector<std::uint8_t> &legalMoves, const Board &board, const Piece &piece, std::uint8_t pieceIdx, std::uint8_t pieceX, std::uint8_t pieceY
using GetLegalMovesFunc = std::function<void(ADD_LEGAL_MOVES_FUNC_PARAMS)>;

#define ADD_LEGAL_MOVES_FUNC_NAME(type) getLegalMoves##type
#define ADD_LEGAL_MOVES_FUNC(type) static void ADD_LEGAL_MOVES_FUNC_NAME(type)(ADD_LEGAL_MOVES_FUNC_PARAMS)
#define ADD_LEGAL_MOVES_MAP_ENTRY(type)                                                                                                                                                                \
	{ type, ADD_LEGAL_MOVES_FUNC_NAME(type) }

ADD_LEGAL_MOVES_FUNC(Pawn);
ADD_LEGAL_MOVES_FUNC(Rook);
ADD_LEGAL_MOVES_FUNC(Bishop);
ADD_LEGAL_MOVES_FUNC(Knight);
ADD_LEGAL_MOVES_FUNC(Queen);
ADD_LEGAL_MOVES_FUNC(King);

static std::unordered_map<PieceType, GetLegalMovesFunc> s_getLegalMovesFuncMap{
	ADD_LEGAL_MOVES_MAP_ENTRY(Pawn),   ADD_LEGAL_MOVES_MAP_ENTRY(Rook),	 ADD_LEGAL_MOVES_MAP_ENTRY(Bishop),
	ADD_LEGAL_MOVES_MAP_ENTRY(Knight), ADD_LEGAL_MOVES_MAP_ENTRY(Queen), ADD_LEGAL_MOVES_MAP_ENTRY(King),
};

void Rules::addLegalMoves(std::vector<std::uint8_t> &legalMoves, const Board &board, std::uint8_t idx) {
	const Piece &piece = board.getPieces()[idx];
	legalMoves.clear();
	s_getLegalMovesFuncMap[piece.getType()](legalMoves, board, piece, idx, idx % 8, idx / 8);
}

static bool addMoveIfEmpty(std::vector<std::uint8_t> &legalMoves, const Board &board, std::uint8_t targetIdx) {
	if (!Board::isSquareIdxCorrect(targetIdx) || !board.getPiece(targetIdx).isNull())
		return false;

	legalMoves.push_back(targetIdx);
	return true;
}
#define ADD_MOVE_IF_EMPTY(idx) addMoveIfEmpty(legalMoves, board, idx)

static bool addMoveIfHasOpponentPiece(std::vector<std::uint8_t> &legalMoves, const Board &board, const Piece &piece, std::uint8_t targetIdx) {
	const Piece &targetPiece = board.getPiece(targetIdx);
	if (!Board::isSquareIdxCorrect(targetIdx) || targetPiece.isNull() || targetPiece.isColor(piece.getColor()))
		return false;

	legalMoves.push_back(targetIdx);
	return true;
}
#define ADD_MOVE_IF_HAS_OPPONENT_PIECE(idx) addMoveIfHasOpponentPiece(legalMoves, board, piece, idx)

static bool addMoveIfNotBlocked(std::vector<std::uint8_t> &legalMoves, const Board &board, const Piece &piece, std::uint8_t targetIdx) {
	if (!Board::isSquareIdxCorrect(targetIdx))
		return false;

	const Piece &targetPiece = board.getPiece(targetIdx);
	if (!targetPiece.isNull() && piece.isColor(targetPiece.getColor())) {
		return false;
	}

	legalMoves.push_back(targetIdx);
	return true;
}
#define ADD_MOVE_IF_NOT_BLOCKED(idx) addMoveIfNotBlocked(legalMoves, board, piece, idx)

ADD_LEGAL_MOVES_FUNC(Pawn) {
	if (piece.isColor(White)) {
		if (ADD_MOVE_IF_EMPTY(pieceIdx - 8))
			if (pieceY == 6)
				ADD_MOVE_IF_EMPTY(pieceIdx - 16);

		if (pieceX != 0)
			ADD_MOVE_IF_HAS_OPPONENT_PIECE(pieceIdx - 9);

		if (pieceX != 7)
			ADD_MOVE_IF_HAS_OPPONENT_PIECE(pieceIdx - 7);
	} else {
		if (ADD_MOVE_IF_EMPTY(pieceIdx + 8))
			if (pieceY == 1)
				ADD_MOVE_IF_EMPTY(pieceIdx + 16);

		if (pieceX != 7)
			ADD_MOVE_IF_HAS_OPPONENT_PIECE(pieceIdx + 9);

		if (pieceX != 0)
			ADD_MOVE_IF_HAS_OPPONENT_PIECE(pieceIdx + 7);
	}
}

ADD_LEGAL_MOVES_FUNC(Rook) {
	// Right
	for (std::uint8_t i = pieceIdx + 1; i < pieceY * 8 + 8; ++i)
		if (ADD_MOVE_IF_HAS_OPPONENT_PIECE(i) || !ADD_MOVE_IF_EMPTY(i))
			break;

	// Left
	for (std::uint8_t i = pieceIdx - 1; i >= pieceY * 8; --i)
		if (ADD_MOVE_IF_HAS_OPPONENT_PIECE(i) || !ADD_MOVE_IF_EMPTY(i))
			break;

	// Top
	for (std::uint8_t i = pieceIdx - 8; i >= pieceX; i -= 8)
		if (ADD_MOVE_IF_HAS_OPPONENT_PIECE(i) || !ADD_MOVE_IF_EMPTY(i))
			break;

	// Bottom
	for (std::uint8_t i = pieceIdx + 8; i <= 56 + pieceX; i += 8)
		if (ADD_MOVE_IF_HAS_OPPONENT_PIECE(i) || !ADD_MOVE_IF_EMPTY(i))
			break;
}

ADD_LEGAL_MOVES_FUNC(Bishop) {
	// Top Right
	for (std::uint8_t i = pieceIdx - 7; i >= pieceIdx - (7 - pieceX) * 7; i -= 7)
		if (ADD_MOVE_IF_HAS_OPPONENT_PIECE(i) || !ADD_MOVE_IF_EMPTY(i))
			break;

	// Bottom Right
	for (std::uint8_t i = pieceIdx + 9; i <= pieceIdx + (7 - pieceX) * 9; i += 9)
		if (ADD_MOVE_IF_HAS_OPPONENT_PIECE(i) || !ADD_MOVE_IF_EMPTY(i))
			break;

	// Top Left
	for (std::uint8_t i = pieceIdx - 9; i >= pieceIdx - pieceX * 9; i -= 9)
		if (ADD_MOVE_IF_HAS_OPPONENT_PIECE(i) || !ADD_MOVE_IF_EMPTY(i))
			break;

	// Bottom Left
	for (std::uint8_t i = pieceIdx + 7; i <= pieceIdx + pieceX * 7; i += 7)
		if (ADD_MOVE_IF_HAS_OPPONENT_PIECE(i) || !ADD_MOVE_IF_EMPTY(i))
			break;
}

ADD_LEGAL_MOVES_FUNC(Knight) {
	if (pieceX < 6) {
		ADD_MOVE_IF_NOT_BLOCKED(pieceIdx + 10);
		ADD_MOVE_IF_NOT_BLOCKED(pieceIdx - 6);
	}

	if (pieceX != 7) {
		ADD_MOVE_IF_NOT_BLOCKED(pieceIdx + 17);
		ADD_MOVE_IF_NOT_BLOCKED(pieceIdx - 15);
	}

	if (pieceX > 1) {
		ADD_MOVE_IF_NOT_BLOCKED(pieceIdx + 6);
		ADD_MOVE_IF_NOT_BLOCKED(pieceIdx - 10);
	}

	if (pieceX != 0) {
		ADD_MOVE_IF_NOT_BLOCKED(pieceIdx - 17);
		ADD_MOVE_IF_NOT_BLOCKED(pieceIdx + 15);
	}
}

ADD_LEGAL_MOVES_FUNC(Queen) {
	ADD_LEGAL_MOVES_FUNC_NAME(Rook)(legalMoves, board, piece, pieceIdx, pieceX, pieceY);
	ADD_LEGAL_MOVES_FUNC_NAME(Bishop)(legalMoves, board, piece, pieceIdx, pieceX, pieceY);
}

ADD_LEGAL_MOVES_FUNC(King) {
	constexpr std::uint8_t offsets[] = {1, 8, 7, 9};
	for (const std::uint8_t of : offsets) {
		ADD_MOVE_IF_NOT_BLOCKED(pieceIdx + of);
		ADD_MOVE_IF_NOT_BLOCKED(pieceIdx - of);
	}
}

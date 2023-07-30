#include "rules.h"
#include "board.h"
#include <functional>
#include <unordered_map>

#define GET_LEGAL_MOVES_FUNC_PARAMS std::vector<std::uint8_t> &legalMoves, const std::array<Piece, 64> &pieces, const Piece &piece, std::uint8_t pieceIdx, std::uint8_t pieceX, std::uint8_t pieceY
using GetLegalMovesFunc = std::function<void(GET_LEGAL_MOVES_FUNC_PARAMS)>;

#define GET_LEGAL_MOVES_FUNC_NAME(type) getLegalMoves##type
#define GET_LEGAL_MOVES_FUNC(type) static void GET_LEGAL_MOVES_FUNC_NAME(type)(GET_LEGAL_MOVES_FUNC_PARAMS)
#define GET_LEGAL_MOVES_MAP_ENTRY(type)                                                                                                                                                                \
	{ type, GET_LEGAL_MOVES_FUNC_NAME(type) }

GET_LEGAL_MOVES_FUNC(Pawn);
GET_LEGAL_MOVES_FUNC(Rook);
GET_LEGAL_MOVES_FUNC(Bishop);
GET_LEGAL_MOVES_FUNC(Knight);
GET_LEGAL_MOVES_FUNC(Queen);
GET_LEGAL_MOVES_FUNC(King);

static std::unordered_map<PieceType, GetLegalMovesFunc> s_getLegalMovesFuncMap{
	GET_LEGAL_MOVES_MAP_ENTRY(Pawn),   GET_LEGAL_MOVES_MAP_ENTRY(Rook),	 GET_LEGAL_MOVES_MAP_ENTRY(Bishop),
	GET_LEGAL_MOVES_MAP_ENTRY(Knight), GET_LEGAL_MOVES_MAP_ENTRY(Queen), GET_LEGAL_MOVES_MAP_ENTRY(King),
};

static inline bool isInRow(std::uint8_t idx, std::uint8_t row) {
	return idx / 8 == row;
}

void Rules::getLegalMoves(std::vector<std::uint8_t> &legalMoves, const Board &board, std::uint8_t idx) {
	const Piece &piece = board.getPieces()[idx];
	legalMoves.clear();
	s_getLegalMovesFuncMap[piece.getType()](legalMoves, board.getPieces(), piece, idx, idx % 8, idx / 8);
}

static bool addMoveIfEmpty(std::vector<std::uint8_t> &legalMoves, const std::array<Piece, 64> &pieces, std::uint8_t targetIdx) {
	if (!Board::isSquareIdxCorrect(targetIdx) || !pieces[targetIdx].isNull())
		return false;

	legalMoves.push_back(targetIdx);
	return true;
}
#define ADD_MOVE_IF_EMPTY(idx) addMoveIfEmpty(legalMoves, pieces, idx)

static bool addMoveIfHasOpponentPiece(std::vector<std::uint8_t> &legalMoves, const std::array<Piece, 64> &pieces, const Piece &piece, std::uint8_t targetIdx) {
	const Piece &targetPiece = pieces[targetIdx];
	if (!Board::isSquareIdxCorrect(targetIdx) || targetPiece.isNull() || targetPiece.isColor(piece.getColor()))
		return false;

	legalMoves.push_back(targetIdx);
	return true;
}
#define ADD_MOVE_IF_HAS_OPPONENT_PIECE(idx) addMoveIfHasOpponentPiece(legalMoves, pieces, piece, idx)

GET_LEGAL_MOVES_FUNC(Pawn) {
	if (piece.isColor(White)) {
		if (ADD_MOVE_IF_EMPTY(pieceIdx - 8))
			if (isInRow(pieceIdx, 6))
				ADD_MOVE_IF_EMPTY(pieceIdx - 16);

		if (pieceX != 0)
			ADD_MOVE_IF_HAS_OPPONENT_PIECE(pieceIdx - 9);

		if (pieceX != 7)
			ADD_MOVE_IF_HAS_OPPONENT_PIECE(pieceIdx - 7);
	} else {
		if (ADD_MOVE_IF_EMPTY(pieceIdx + 8))
			if (isInRow(pieceIdx, 1))
				ADD_MOVE_IF_EMPTY(pieceIdx + 16);

		if (pieceX != 7)
			ADD_MOVE_IF_HAS_OPPONENT_PIECE(pieceIdx + 9);

		if (pieceX != 0)
			ADD_MOVE_IF_HAS_OPPONENT_PIECE(pieceIdx + 7);
	}
}

GET_LEGAL_MOVES_FUNC(Rook) {
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

GET_LEGAL_MOVES_FUNC(Bishop) {}

GET_LEGAL_MOVES_FUNC(Knight) {}

GET_LEGAL_MOVES_FUNC(Queen) {}

GET_LEGAL_MOVES_FUNC(King) {}

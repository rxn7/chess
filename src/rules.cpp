#include "rules.h"
#include "board.h"
#include <functional>
#include <unordered_map>

using GetLegalMovesFunc = std::function<std::vector<std::uint8_t>(const Board &board, const Piece &piece, std::uint8_t idx)>;

#define GET_LEGAL_MOVES_FUNC_NAME(type) getLegalMoves##type
#define GET_LEGAL_MOVES_FUNC(type) static std::vector<std::uint8_t> GET_LEGAL_MOVES_FUNC_NAME(type)(const Board &board, const Piece &piece, std::uint8_t idx)
#define GET_LEGAL_MOVES_MAP_ENTRY(type) { type, GET_LEGAL_MOVES_FUNC_NAME(type) } 

GET_LEGAL_MOVES_FUNC(Pawn);
GET_LEGAL_MOVES_FUNC(Rook);
GET_LEGAL_MOVES_FUNC(Bishop);
GET_LEGAL_MOVES_FUNC(Knight);
GET_LEGAL_MOVES_FUNC(Queen);
GET_LEGAL_MOVES_FUNC(King);

static std::unordered_map<PieceType, GetLegalMovesFunc> s_getLegalMovesFuncMap{
	GET_LEGAL_MOVES_MAP_ENTRY(Pawn), 
	GET_LEGAL_MOVES_MAP_ENTRY(Rook), 
	GET_LEGAL_MOVES_MAP_ENTRY(Bishop), 
	GET_LEGAL_MOVES_MAP_ENTRY(Knight), 
	GET_LEGAL_MOVES_MAP_ENTRY(Queen), 
	GET_LEGAL_MOVES_MAP_ENTRY(King), 
};

std::vector<std::uint8_t> Rules::getLegalMoves(const Board &board, std::uint8_t idx) {
	const Piece &piece = board.getPieces()[idx];
	return s_getLegalMovesFuncMap[piece.getType()](board, piece, idx);
}

GET_LEGAL_MOVES_FUNC(Pawn) {
	return {};
}

GET_LEGAL_MOVES_FUNC(Rook) {
	return {};
}

GET_LEGAL_MOVES_FUNC(Bishop) {
	return {};
}

GET_LEGAL_MOVES_FUNC(Knight) {
	return {};
}

GET_LEGAL_MOVES_FUNC(Queen) {
	return {};
}

GET_LEGAL_MOVES_FUNC(King) {
	return {};
}

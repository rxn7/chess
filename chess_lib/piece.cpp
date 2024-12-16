#include "piece.hpp"

#include <unordered_map>
#include <string>

static const std::unordered_map<PieceType, char> PIECE_CHARS {
	{PieceType::None, ' '},
	{PieceType::King, 'k'},
	{PieceType::Queen, 'q'},
	{PieceType::Bishop, 'b'},
	{PieceType::Knight, 'n'},
	{PieceType::Rook, 'r'},
	{PieceType::Pawn, 'p'},
};

char Piece::toChar() const {
	if(isColor(White))
		return std::toupper(PIECE_CHARS.at(getType()));

	return PIECE_CHARS.at(getType());
}

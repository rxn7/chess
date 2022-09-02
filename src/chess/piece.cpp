#include "piece.h"
#include "board.h"
#include <cmath>
#include <iostream>

#define ADD_LEGAL_PIECE_MOVES_DECL(type) static void addLegal##type##Moves(uint8_t idx, std::array<PieceValue, 64> &pieces, std::vector<uint8_t> &moves)
#define ADD_LEGAL_PIECE_MOVES(type) addLegal##type##Moves(idx, pieces, moves)

#define ADD_MOVE_IF_SQUARE_EMPTY(i) _addMoveIfSquareEmpty(i, pieces, moves)
#define ADD_MOVE_IF_SQUARE_HAS_OPPONENT_PIECE(i) _addMoveIfSquareHasOpponentPiece(pieces[idx], i, pieces, moves)
#define ADD_MOVE_IF_NOT_BLOCKED(i) _addMoveIfNotBlocked(piece[idx], i, pieces, moves)

ADD_LEGAL_PIECE_MOVES_DECL(Pawn);
ADD_LEGAL_PIECE_MOVES_DECL(Rook);
ADD_LEGAL_PIECE_MOVES_DECL(Bishop);
ADD_LEGAL_PIECE_MOVES_DECL(Knight);
ADD_LEGAL_PIECE_MOVES_DECL(Queen);
ADD_LEGAL_PIECE_MOVES_DECL(King);

static bool _addMoveIfSquareEmpty(uint8_t idx, std::array<uint8_t, 64> &pieces, std::vector<uint8_t> &moves) {
	if(!Board::isSquareIdxCorrect(idx))
		return false;

	if(!Piece::isNull(pieces[idx]))
		return false;

	moves.push_back(idx);
	return true;
}

static bool _addMoveIfSquareHasOpponentPiece(PieceValue piece, uint8_t idx, std::array<uint8_t, 64> &pieces, std::vector<uint8_t> &moves) {
	if(!Board::isSquareIdxCorrect(idx))
		return false;

	if(Piece::isNull(pieces[idx]) || (piece & COLOR_MASK) == (pieces[idx] & COLOR_MASK))
		return false;

	moves.push_back(idx);
	return true;
}

static bool _addMoveIfNotblocked(PieceValue piece, uint8_t idx, std::array<uint8_t, 64> &pieces, std::vector<uint8_t> &moves) {
	if(!Board::isSquareIdxCorrect(idx))
		return false;

	if(!Board::isSquareIdxCorrect(idx))
		return false;

	if(Piece::isNull(pieces[idx]) || (piece & COLOR_MASK) != (pieces[idx] & COLOR_MASK)) {
		moves.push_back(idx);
		return true;
	}

	return false;
}

void Piece::getLegalMoves(uint8_t idx, std::array<PieceValue, 64> &pieces, std::vector<uint8_t> &moves) {
	moves.clear();

	const PieceValue &piece = pieces[idx];
	uint8_t color = piece & COLOR_MASK;

	// TODO: Would map instead of switch be faster here? 
	switch(piece & TYPE_MASK) {
		case Pawn:	ADD_LEGAL_PIECE_MOVES(Pawn);	break;
		case Rook:	ADD_LEGAL_PIECE_MOVES(Rook);	break;
		case Bishop:	ADD_LEGAL_PIECE_MOVES(Bishop);	break;
		case Knight:	ADD_LEGAL_PIECE_MOVES(Knight);	break;
		case Queen:	ADD_LEGAL_PIECE_MOVES(Queen);	break;
		case King:	ADD_LEGAL_PIECE_MOVES(King);	break;
	}
}

ADD_LEGAL_PIECE_MOVES_DECL(Pawn) {
	PieceValue piece = pieces[idx];
	uint8_t color = piece & COLOR_MASK;

	if(color == Piece::White) {
		ADD_MOVE_IF_SQUARE_EMPTY(idx-8);
		if(idx >= 48 && idx <= 55) 
			ADD_MOVE_IF_SQUARE_EMPTY(idx-16);

		ADD_MOVE_IF_SQUARE_HAS_OPPONENT_PIECE(idx-9);
		ADD_MOVE_IF_SQUARE_HAS_OPPONENT_PIECE(idx-7);
	} else {
		ADD_MOVE_IF_SQUARE_EMPTY(idx+8);
		if(idx >= 8 && idx <= 15) 
			ADD_MOVE_IF_SQUARE_EMPTY(idx+16);

		ADD_MOVE_IF_SQUARE_HAS_OPPONENT_PIECE(idx+9);
		ADD_MOVE_IF_SQUARE_HAS_OPPONENT_PIECE(idx+7);
	}
}

ADD_LEGAL_PIECE_MOVES_DECL(Rook) {
	PieceValue piece = pieces[idx];
	uint8_t color = piece & COLOR_MASK;

	// Right
	for(uint8_t i = idx+1; i<std::ceil(idx/8) * 8 + 8; ++i)
		if(ADD_MOVE_IF_SQUARE_HAS_OPPONENT_PIECE(i) || !ADD_MOVE_IF_SQUARE_EMPTY(i))
			break;

	// Left
	for(uint8_t i = idx-1; i>=std::ceil(idx/8) * 8; --i)
		if(ADD_MOVE_IF_SQUARE_HAS_OPPONENT_PIECE(i) || !ADD_MOVE_IF_SQUARE_EMPTY(i))
			break;

	// Top
	for(uint8_t i = idx-8; i>=idx%8; i-=8)
		if(ADD_MOVE_IF_SQUARE_HAS_OPPONENT_PIECE(i) || !ADD_MOVE_IF_SQUARE_EMPTY(i))
			break;

	// Bottom
	for(uint8_t i = idx+8; i<=(8*7)+(idx%8); i+=8)
		if(ADD_MOVE_IF_SQUARE_HAS_OPPONENT_PIECE(i) || !ADD_MOVE_IF_SQUARE_EMPTY(i))
			break;
}

ADD_LEGAL_PIECE_MOVES_DECL(Bishop) {
}

ADD_LEGAL_PIECE_MOVES_DECL(Knight) {
}

ADD_LEGAL_PIECE_MOVES_DECL(Queen) {
	ADD_LEGAL_PIECE_MOVES(Bishop);
	ADD_LEGAL_PIECE_MOVES(Rook);
}

ADD_LEGAL_PIECE_MOVES_DECL(King) {
}
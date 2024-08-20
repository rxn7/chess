#include "board.h"
#include "rules.h"
#include "piece.h"

#include <iostream>
#include <sstream>
#include <SFML/Graphics/RenderWindow.hpp>

enum FenRecord {
	PiecePlacement = 0,
	ActiveColor = 1,
	CastlingAvailability = 2,
	EnPassantTargetSquare = 3,
	HalfMoveClock = 4,
	FullMoveNumber = 5
};

Board::Board() {
	reset();
}

Board::Board(const Board &board) : m_pieces(board.m_pieces), m_checkResult(board.m_checkResult), m_lastMove(board.m_lastMove), m_turnColor(board.m_turnColor) {}

void Board::reset() {
	std::fill(m_pieces.begin(), m_pieces.end(), 0); // Clear the board
	m_checkResult =(CheckResult){
		.isCheck = false,
	};

	m_lastMove.reset();

	applyFen(DEFAULT_FEN);
	updateLegalMoves();
}

void Board::updateLegalMoves() {
	m_legalMoves.clear();

	for(std::uint8_t i = 0; i < 64; ++i) {
		const Piece &piece = getPiece(i);
		if(piece.isNull() || piece.getColor() != m_turnColor)
			continue;

		Rules::addLegalMoves(m_legalMoves, *this, i);
	}
}

void Board::checkPawnPromotion(const Move &move) {
	if(!move.piece.isType(Pawn))
		return;

	if(move.piece.isColor(White)) {
		if(move.toIdx / 8 == 0)
			m_pieces[move.toIdx] = Piece(White, Queen);
	} else if(move.toIdx / 8 == 7)
		m_pieces[move.toIdx] = Piece(Black, Queen);
}

void Board::applyMove(const Move &move, const bool updateMoves, const bool updateCheckResult) {
	m_pieces[move.toIdx] = move.piece;
	m_pieces[move.fromIdx] = 0;
	m_lastMove = move;

	checkPawnPromotion(move);

	if(updateMoves) {
		m_turnColor = m_turnColor == White ? Black : White;
		updateLegalMoves();
	}

	if(updateCheckResult) {
		m_checkResult = calculateCheck(m_turnColor);
	}
}

void Board::applyFen(const std::string &fen) {
	std::uint8_t file = 0, rank = 0;

	std::istringstream stream(fen);
	std::string record;

	int recordIdx = 0;

	while(stream >> record) {
		switch(recordIdx++) {
			case FenRecord::PiecePlacement:
				for(const char c : record) {
					if(c == '/') {
						file = 0;
						rank++;
					} else {
						if(isdigit(c)) {
							file += c - '0';
						} else {
							const PieceColor color = isupper(c) ? White : Black;

							PieceType type;
							switch(tolower(c)) {
							case 'q':
								type = Queen;
								break;
							case 'k':
								type = King;
								break;
							case 'n':
								type = Knight;
								break;
							case 'b':
								type = Bishop;
								break;
							case 'r':
								type = Rook;
								break;
							case 'p':
								type = Pawn;
								break;
							default:
								std::cerr << "\e[1;31mInvalid piece type: " << c << "!\e[0m\n";
							}

							m_pieces[rank * 8 + file] = Piece(color, type);
							file++;
						}
					}
				}
				break;

			case FenRecord::ActiveColor:
				m_turnColor = record[0] == 'w' ? White : Black;
				break;

			case FenRecord::CastlingAvailability:
				break;
		}
	}
}

CheckResult Board::calculateCheck(const PieceColor color) {
	std::vector<Move> legalMoves;

	// Get all legal moves of the opponent
	for(std::uint8_t i = 0; i < 64; ++i) {
		const Piece &piece = getPiece(i);

		if(piece.isNull() || piece.isColor(color))
			continue;

		Rules::addLegalMoves(legalMoves, *this, i, true);
	}
	
	// Check if any of the opponent's moves puts the king in check
	for(const Move &move : legalMoves) {
		const Piece &targetPiece = getPiece(move.toIdx);
		if(targetPiece.isType(King)) {
			return(CheckResult){.isCheck = true, .kingIdx = move.toIdx, .checkingPieceIdx = move.fromIdx};
		}
	}

	return(CheckResult){.isCheck = false};
}

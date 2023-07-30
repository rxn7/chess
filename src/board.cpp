#include "board.h"
#include "board_renderer.h"
#include "board_theme.h"
#include "legal_moves.h"
#include "piece.h"
#include "sound_system.h"
#include <cstddef>
#include <iostream>
#include <memory>
#include <SFML/Graphics/RenderWindow.hpp>

Board::Board() {
	reset();
}

Board::Board(const Board &board) : m_pieces(board.m_pieces), m_checkResult(board.m_checkResult) {}

void Board::reset() {
	std::fill(m_pieces.begin(), m_pieces.end(), 0); // Clear the board
	m_checkResult = (CheckResult){
		.isCheck = false,
	};

	m_lastMove.reset();

	applyFen(DEFAULT_FEN);
}

void Board::checkPawnPromotion(const Move &move){ 
	if (!move.piece.isType(Pawn))
		return;

	if (move.piece.isColor(White)) {
		if (move.toIdx / 8 == 0)
			m_pieces[move.toIdx] = Piece(White, Queen);
	} else if (move.toIdx / 8 == 7)
		m_pieces[move.toIdx] = Piece(Black, Queen);
}

void Board::applyMove(const Move &move, bool updateCheckResult) {
	m_pieces[move.toIdx] = move.piece;
	m_pieces[move.fromIdx] = 0;
	m_lastMove = move;

	checkPawnPromotion(move);

	if (updateCheckResult) {
		const PieceColor checkedColor = move.piece.getColor() == White ? Black : White;
		m_checkResult = getCheckResult(checkedColor);
	}
}

void Board::applyFen(const std::string &fen) {
	std::uint8_t file = 0, rank = 0;

	for (char c : fen) {
		switch (c) {
		case '/':
			file = 0;
			rank++;
			break;
		default:
			if (isdigit(c)) {
				file += c - '0';
			} else {
				const PieceColor color = isupper(c) ? White : Black;

				PieceType type;
				switch (tolower(c)) {
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
				case ' ':
					break;
				default:
					std::cerr << "\e[1;33mUnknown char '" << c << "' in fen notation '" << fen << "'!\e[0m\n";
					return;
				}

				m_pieces[rank * 8 + file] = Piece(color, type);
				file++;
			}
			break;
		}
	}
}

CheckResult Board::getCheckResult(PieceColor color) {
	std::vector<std::uint8_t> legalMoves;
	for (std::uint8_t i = 0; i < 64; ++i) {
		const Piece &piece = getPiece(i);

		if (piece.isNull() || piece.isColor(color))
			continue;

		legalMoves.clear();
		addLegalMoves(legalMoves, *this, i, false);

		for (const std::uint8_t idx : legalMoves)
			if (getPiece(idx).isType(King)) {
				return (CheckResult){.isCheck = true, .kingIdx = idx, .checkingPieceIdx = i};
			}
	}

	return (CheckResult){.isCheck = false};
}

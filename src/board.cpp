#include "board.h"
#include "board_renderer.h"
#include "board_theme.h"
#include "piece.h"
#include "rules.h"
#include "sound_system.h"
#include <cstddef>
#include <iostream>
#include <memory>
#include <SFML/Graphics/RenderWindow.hpp>

Board::Board() {
	reset();
}

Board::Board(const Board &board) : m_pieces(board.m_pieces) {}

void Board::reset() {
	std::fill(m_pieces.begin(), m_pieces.end(), 0); // Clear the board

	m_lastMove.reset();

	applyFen(DEFAULT_FEN);
}

void Board::processPawnPromotion(uint8_t idx) {
	Piece &piece = m_pieces[idx];

	if (!piece.isType(Pawn))
		return;

	if (piece.isColor(White)) {
		if (idx >= 0 && idx < 8)
			piece = Piece(White, Queen);
	} else {
		if (idx >= 56 && idx < 64)
			piece = Piece(Black, Queen);
	}
}

void Board::applyMove(const Move &move) {
	m_pieces[move.toIdx] = move.piece;
	m_pieces[move.fromIdx] = 0;
	m_lastMove = move;
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

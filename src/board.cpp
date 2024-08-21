#include "board.h"
#include "game.h"
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

Board::Board(Game &game) : m_game(game) {
	reset();
}

Board::Board(const Board &board) : m_game(board.m_game), m_pieces(board.m_pieces), m_checkResult(board.m_checkResult), m_lastMove(board.m_lastMove), m_turnColor(board.m_turnColor), m_fullMoves(board.m_fullMoves), m_halfMoveClock(board.m_halfMoveClock) {
	m_players.insert({White, {}});
	m_players.insert({Black, {}});
}

void Board::reset() {
	std::fill(m_pieces.begin(), m_pieces.end(), 0); // Clear the board
	m_checkResult =(CheckResult){
		.isCheck = false,
	};

	m_lastMove = std::nullopt;

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

void Board::applyMove(const Move &move, const bool isFake, const bool updateCheckResult) {
	m_pieces[move.toIdx] = move.piece;
	m_pieces[move.fromIdx] = 0;
	m_lastMove = move;

	handleCastling(move);
	handlePawnPromotion(move);
	handleEnPassant(move);

	if(!isFake) {
		applyMoveRules(move);

		m_turnColor = m_turnColor == White ? Black : White;
		updateLegalMoves();
	}

	if(updateCheckResult) {
		m_checkResult = calculateCheck(m_turnColor);
	}
}

void Board::applyMoveRules(const Move &move) {
	if(move.piece.isColor(Black)) {
		++m_fullMoves;
	}

	if(move.piece.isType(Pawn) || move.isCapture) {
		m_halfMoveClock = 0;
	} else {
		++m_halfMoveClock;
		if(m_halfMoveClock == 50) {
			std::cout << "50 half moves without capture or pawn move. Draw!" << std::endl;
			m_game.end(GameResult::Draw);
		}
	}

	Player &player = m_players[move.piece.getColor()];

	if(move.piece.isType(King)) {
		player.canCastleQueenSide = false;
		player.canCastleKingSide = false;
		std::cout << "Player " << (move.piece.getColor() == White ? "white" : "black") << " can no longer castle" << std::endl;
	} else if(move.piece.isType(Rook)) {
		if(move.fromIdx == 0 || move.fromIdx == 56) {
			player.canCastleQueenSide = false;
			std::cout << "Player " << (move.piece.getColor() == White ? "white" : "black") << " can no longer castle queen side" << std::endl;
		} else {
			player.canCastleKingSide = false;
			std::cout << "Player " << (move.piece.getColor() == White ? "white" : "black") << " can no longer castle king side" << std::endl;
		}
	}

	std::cout << "Half move clock: " << m_halfMoveClock << std::endl;
	std::cout << "Full moves: " << m_fullMoves << std::endl;
}

void Board::handleCastling(const Move &move) {
	if(move.isQueenSideCastling) {
		if(move.piece.getColor() == Black) {
			applyMove(Move(*this, 0, 3), true, false);
		} else {
			applyMove(Move(*this, 56, 59), true, false);
		}
	} else if(move.isKingSideCastling) {
		if(move.piece.getColor() == Black) {
			applyMove(Move(*this, 7, 5), true, false);
		} else {
			applyMove(Move(*this, 63, 61), true, false);
		}
	}
}

void Board::handlePawnPromotion(const Move &move) {
	if(move.isPawnPromotion)
		m_pieces[move.toIdx] = Piece(move.piece.getColor(), Queen);
}

void Board::handleEnPassant(const Move &move) {
	if(move.piece.getType() == Pawn) {
		// If moved 2 files
		if(move.toIdx == m_enPassantTarget) {
			if(move.piece.getColor() == White)
				m_pieces[move.toIdx + 8] = 0;
			else
				m_pieces[move.toIdx - 8] = 0;
		}

		if(std::abs(move.fromIdx - move.toIdx) == 16) {
			if(move.piece.getColor() == White)
				m_enPassantTarget = move.toIdx + 8;
			else
				m_enPassantTarget = move.toIdx - 8;

			return;
		}
	} 
	m_enPassantTarget = std::nullopt;
}

// based on: https://www.chess.com/terms/fen-chess
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
				if(record.empty() || record[0] == '-') {
					for(auto &i : m_players) {
						i.second.canCastleKingSide = false;
						i.second.canCastleQueenSide = false;
					}
					break;
				}

				for(const char c : record) {
					switch(c) {
						case 'K':
							m_players[White].canCastleKingSide = true;
							break;
						case 'Q':
							m_players[White].canCastleQueenSide = true;
							break;
						case 'k':
							m_players[Black].canCastleKingSide = true;
							break;
						case 'q':
							m_players[Black].canCastleQueenSide = true;
							break;
					}
				}
				break;

			case FenRecord::EnPassantTargetSquare:
				if(record.empty() || record[0] == '-') {
					m_enPassantTarget = std::nullopt;
					break;
				}

				m_enPassantTarget = getSquareIdx(record[0], record[1]);
				break;

			case FenRecord::HalfMoveClock:
				m_halfMoveClock = std::stoi(record);
				break;

			case FenRecord::FullMoveNumber:
				m_fullMoves = std::stoi(record);
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

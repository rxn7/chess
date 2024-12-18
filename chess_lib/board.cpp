#include "board.hpp"
#include "fen.hpp"
#include "rules.hpp"
#include "piece.hpp"

#include <iostream>
#include <algorithm>

Board::Board() {
	reset();
}

Board::Board(const Board &board) : m_pieces(board.m_pieces), m_state(board.m_state) {}

void Board::reset(bool applyDefaultFen) {
	m_pieces.fill(0);
	m_state = {};
													
	m_checkResult = (CheckResult){
		.isCheck = false,
	};

	m_status = BoardStatus::Playing;
	m_lastMove = std::nullopt;

	if(applyDefaultFen) {
		FEN::applyFen(*this, DEFAULT_FEN);
	}
}

bool Board::applyMove(const Move &move, const bool updateCheckResult) {
	if(std::find(m_legalMoves.begin(), m_legalMoves.end(), move) == m_legalMoves.end()) {
		std::cerr << "This move (" << move << ") is not legal" << std::endl;
		std::cerr << "Legal moves are: ";
		for(const Move &move : m_legalMoves) {
			std::cerr << move << std::endl;
		}

		return false;
	}

	if(!handleCastling(move)) {
		m_pieces[move.toIdx] = move.piece;
		m_pieces[move.fromIdx] = 0;
	}

	handlePawnPromotion(move);
	handleEnPassant(move, true);

	if(move.piece.isColor(Black)) {
		++m_state.fullMoves;
	}

	if(move.piece.isType(Pawn) || move.isCapture) {
		m_state.halfMoveClock = 0;
	} else {
		++m_state.halfMoveClock;
		if(m_state.halfMoveClock == 50) {
			std::cout << "50 half moves without capture or pawn move. Draw!" << std::endl;
			m_status = BoardStatus::Draw;
		}
	}

	Player &player = getPlayer(move.piece.getColor());

	if(move.piece.isType(King)) {
		player.canCastleQueenSide = false;
		player.canCastleKingSide = false;
	} else if(move.piece.isType(Rook)) {
		if(move.fromIdx == 0 || move.fromIdx == 56) {
			player.canCastleQueenSide = false;
		} else {
			player.canCastleKingSide = false;
		}
	}

	m_state.turnColor = m_state.turnColor == White ? Black : White;
	m_lastMove = move;

	if(updateCheckResult) {
		m_checkResult = calculateCheck(m_state.turnColor);
	}

	updateLegalMoves();
	updateStatus();

	return true;
}

CheckResult Board::fakeMove(const Move &move) {
	std::array<Piece, 64> oldPieces = m_pieces;

	if(!handleCastling(move)) {
		m_pieces[move.toIdx] = move.piece;
		m_pieces[move.fromIdx] = 0;
	}

	handlePawnPromotion(move);
	handleEnPassant(move, false);

	CheckResult result = calculateCheck(m_state.turnColor);
	m_pieces = oldPieces;

	return result;
}

void Board::castle(PieceColor color, bool isQueenSide) {
	uint8_t kingStartIdx, rookStartIdx, kingEndIdx, rookEndIdx;
	if(color == Black) {
		if(isQueenSide) {
			kingStartIdx = 4;
			kingEndIdx = 2;
			rookStartIdx = 0;
			rookEndIdx = 3;
		} else {
			kingStartIdx = 4;
			kingEndIdx = 6;
			rookStartIdx = 7;
			rookEndIdx = 5;
		}
	} else {
		if(isQueenSide) {
			kingStartIdx = 60;
			kingEndIdx = 58;
			rookStartIdx = 56;
			rookEndIdx = 59;
		} else {
			kingStartIdx = 60;
			kingEndIdx = 62;
			rookStartIdx = 63;
			rookEndIdx = 61;
		}
	}

	m_pieces[kingStartIdx] = 0;
	m_pieces[rookStartIdx] = 0;

	m_pieces[kingEndIdx] = Piece(color, King);
	m_pieces[rookEndIdx] = Piece(color, Rook);
}

bool Board::handleCastling(const Move &move) {
	if(move.isKingSideCastling) {
		castle(move.piece.getColor(), false);
		return true;
	} 
	if(move.isQueenSideCastling) {
		castle(move.piece.getColor(), true);
		return true;
	} 

	return false;
}

void Board::handlePawnPromotion(const Move &move) {
	if(move.isPawnPromotion)
		m_pieces[move.toIdx] = Piece(move.piece.getColor(), Queen);
}

void Board::handleEnPassant(const Move &move, const bool changeEnPassantTarget) {
	if(move.piece.getType() == Pawn) {
		// If moved 2 files
		if(move.toIdx == m_state.enPassantTarget) {
			if(move.piece.getColor() == White)
				m_pieces[move.toIdx + 8] = 0;
			else
				m_pieces[move.toIdx - 8] = 0;
		}

		if(changeEnPassantTarget && std::abs(move.fromIdx - move.toIdx) == 16) {
			if(move.piece.getColor() == White)
				m_state.enPassantTarget = move.toIdx + 8;
			else
				m_state.enPassantTarget = move.toIdx - 8;

			return;
		}
	} 

	if(changeEnPassantTarget) {
		m_state.enPassantTarget = std::nullopt;
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
		if(move.targetPiece.isType(King)) {
			return(CheckResult){.isCheck = true, .kingIdx = move.toIdx, .checkingPieceIdx = move.fromIdx};
		}
	}

	return(CheckResult){.isCheck = false};
}

void Board::updateLegalMoves() {
#ifndef NDEBUG
	auto timeStart = std::chrono::high_resolution_clock::now();
#endif

	m_legalMoves.clear();

	for(std::uint8_t i = 0; i < 64; ++i) {
		const Piece &piece = getPiece(i);
		if(piece.isNull() || piece.getColor() != m_state.turnColor)
			continue;

		Rules::addLegalMoves(m_legalMoves, *this, i);
	}

#ifndef NDEBUG
	auto timeEnd = std::chrono::high_resolution_clock::now();
	m_debugData.legalMovesCalculationTime = timeEnd - timeStart;
	m_debugData.legalMovesCount = m_legalMoves.size();
#endif
}

void Board::updateStatus() {
	if(m_checkResult.isCheck) {
		if(m_legalMoves.empty()) {
			m_status = m_state.turnColor == White ? BoardStatus::BlackWin : BoardStatus::WhiteWin; // Checkmate
		}
	} else {
		if(m_legalMoves.empty()) {
			m_status = BoardStatus::Draw; // Stalemate
		}
	}
}

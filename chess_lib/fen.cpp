#include "fen.hpp"
#include "board.hpp"

#include <sstream>

// based on: https://www.chess.com/terms/fen-chess
bool FEN::applyFen(Board &board, const std::string &fen) {
	board.reset(false);

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

							PieceType type = None;
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
								std::cerr << "\e[1;31mInvalid piece type: " << (int)c << "!\e[0m\n";
								return false;
							}

							board.m_pieces[rank * 8 + file] = Piece(color, type);
							file++;
						}
					}
				}
				break;

			case FenRecord::ActiveColor:
				switch(record[0]) {
					case 'w': board.m_state.turnColor = White; break;
					case 'b': board.m_state.turnColor = Black; break;
					default: std::cerr << "Invalid active color in fen: " << record[0] << std::endl; return false;
				}
				break;

			case FenRecord::CastlingAvailability:
				if(record.empty() || record[0] == '-') {
					const auto disableCastling = [&](Player &player) {
						player.canCastleKingSide = false;
						player.canCastleQueenSide = false;
					};

					disableCastling(board.m_state.whitePlayer);
					disableCastling(board.m_state.blackPlayer);

					break;
				}

				for(const char c : record) {
					switch(c) {
						case 'K':
							board.m_state.whitePlayer.canCastleKingSide = true;
							break;
						case 'Q':
							board.m_state.whitePlayer.canCastleQueenSide = true;
							break;
						case 'k':
							board.m_state.blackPlayer.canCastleKingSide = true;
							break;
						case 'q':
							board.m_state.blackPlayer.canCastleQueenSide = true;
							break;
						default: return false;
					}
				}
				break;

			case FenRecord::EnPassantTargetSquare:
				if(record.empty() || record[0] == '-') {
					board.m_state.enPassantTarget = std::nullopt;
					break;
				}

				board.m_state.enPassantTarget = Board::getSquareIdx(record);
				break;

			case FenRecord::HalfMoveClock:
				board.m_state.halfMoveClock = std::stoi(record);
				break;

			case FenRecord::FullMoveNumber:
				board.m_state.fullMoves = std::stoi(record);
				break;
		}
	}

	board.updateLegalMoves();
	return true;
}

std::string FEN::convertToFen(const Board &board) {
	std::ostringstream stream;

	for(std::uint8_t rank = 0; rank < 8; ++rank) {
		uint8_t emptyCount = 0;
		for(std::uint8_t file = 0; file < 8; ++file) {
			const std::uint8_t idx = rank * 8 + file;
			const Piece &piece = board.getPiece(idx);

			if(piece.isNull()) {
				++emptyCount;
				continue;
			}

			if(emptyCount != 0) {
				stream << emptyCount;
				emptyCount = 0;
			}

			std::cout << piece.getType() << piece.getColor() << std::endl;
			stream << piece.toChar();
		}

		if(emptyCount != 0) {
			stream << emptyCount;
		}

		if(rank != 7) {
			stream << '/';
		}
	}

	stream << ' ';

	bool anyCastle = false;
	if(board.m_state.whitePlayer.canCastleKingSide) { 
		anyCastle = true;
		stream << 'K';
	}
	if(board.m_state.whitePlayer.canCastleQueenSide) {
		anyCastle = true;
		stream << 'Q';
	}
	if(board.m_state.blackPlayer.canCastleKingSide) {
		anyCastle = true;
		stream << 'k';
	}
	if(board.m_state.blackPlayer.canCastleQueenSide) {
		anyCastle = true;
		stream << 'q';
	}

	if(!anyCastle) {
		stream << '-';
	}

	stream << ' ';

	if(board.m_state.enPassantTarget.has_value()) {
		stream << Move::positionToString(board.m_state.enPassantTarget.value());
	} else {
		stream << '-';
	}

	stream << ' ' << board.m_state.halfMoveClock << ' ' << board.m_state.fullMoves;

	return stream.str();
}

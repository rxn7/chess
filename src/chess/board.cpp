#include "board.h"
#include "chess/board_renderer.h"
#include "chess/board_theme.h"
#include "chess/piece.h"
#include <SFML/Graphics/RenderWindow.hpp>
#include <iostream>
#include <memory>

#define MOVE_SOUND_PATH "res/move.flac"

Board::Board(sf::RenderWindow &window, const sf::Font &font, const BoardTheme &theme) : m_window(window), m_boardRenderer(m_window, font, theme) {
	if(!m_pieceMoveSb.loadFromFile(MOVE_SOUND_PATH))
		std::cerr << "\e[1;31mFailed to load piece move sound (" MOVE_SOUND_PATH ")!\e[0m\n";

	m_pieceMoveSound.setBuffer(m_pieceMoveSb);

	reset();
}

void Board::reset() {
	std::fill(m_pieces.begin(), m_pieces.end(), Piece::None); // Clear the board
	resetHeldPiece();
	applyFen(DEFAULT_FEN);
	m_turnColor = Piece::White;
}

void Board::render() {
	m_boardRenderer.renderSquares();

	if(!m_lastMove.isNull())
		m_boardRenderer.highlightMoveSquares(m_lastMove);

	renderPieces();
	m_boardRenderer.renderCoords();
	renderHeldPiece();
}

void Board::movePiece(const Move &move) {
	m_pieces[move.toIdx] = m_pieces[move.fromIdx];
	m_pieces[move.fromIdx] = Piece::None;
	playMoveSound();

	m_turnColor = m_turnColor == Piece::White ? Piece::Black : Piece::White;
	m_lastMove = move;
}

void Board::playMoveSound() {
	m_pieceMoveSound.setPitch((90 + rand() % 20) / 100.0f);
	m_pieceMoveSound.play();
}

void Board::handlePieceDrag() {
	if(isAnyPieceHeld())
		return;

	sf::Vector2f pos = m_window.mapPixelToCoords(sf::Mouse::getPosition(m_window));
	sf::Vector2i gridPos(pos.x / 64, pos.y / 64);
	uint8_t idx = gridPos.y * 8 + gridPos.x;

	if(idx < 0 || idx >= 64)
		return;

	PieceValue piece = m_pieces[idx];
	if(!Piece::isNull(piece) && (piece & COLOR_MASK) == m_turnColor)
		m_heldPieceIdx = idx;
}

void Board::handlePieceDrop() {
	if(!isAnyPieceHeld())
		return;

	sf::Vector2i mousePos = sf::Mouse::getPosition(m_window);
	sf::Vector2f pos = m_window.mapPixelToCoords(mousePos);
	if(pos.x < 0 || pos.x > 512 || pos.y < 0 || pos.y > 512) {
		resetHeldPiece();
		return;
	}

	sf::Vector2i gridPos(pos.x / 64, pos.y / 64);
	uint8_t idx = gridPos.y * 8 + gridPos.x;

	if(idx < 0 || idx >= 64) {
		resetHeldPiece();
		return;
	}

	PieceValue piece = m_pieces[idx];
	if(Piece::isNull(piece) || (piece & COLOR_MASK) != (m_pieces[m_heldPieceIdx] & COLOR_MASK))
		movePiece({m_heldPieceIdx, idx});
	else
		resetHeldPiece();
}

void Board::renderPieces() {
	for(auto i=0; i<m_pieces.size(); ++i)
		if(i != m_heldPieceIdx) 
			m_pieceRenderer.renderPiece(m_window, m_pieces[i], i);
}

void Board::renderHeldPiece() {
	if(!isAnyPieceHeld())
		return;

	sf::Vector2f pos = m_window.mapPixelToCoords(sf::Mouse::getPosition(m_window));
	pos.x -= 32;
	pos.y -= 32;

	m_pieceRenderer.renderPiece(m_window, m_pieces[m_heldPieceIdx], pos);
}

void Board::applyFen(const std::string &fen) {
	uint8_t file = 0, rank = 0;

	for(char c : fen) {
		switch(c) {
			case '/':
				file = 0;
				rank++;
				break;
			default:
				if(isdigit(c)) {
					file += c - '0';
				} else {
					uint8_t color = isupper(c) ? Piece::White : Piece::Black;

					uint8_t type;
					switch(tolower(c)) {
						case 'q': type = Piece::Queen; break;
						case 'k': type = Piece::King; break;
						case 'n': type = Piece::Knight; break;
						case 'b': type = Piece::Bishop; break;
						case 'r': type = Piece::Rook; break;
						case 'p': type = Piece::Pawn; break;
						default: 
							std::cerr << "\e[1;31mUnknown char '" << c << "' in fen notation '" << fen << "'!\e[0m\n";
							return;
					}

					m_pieces[rank * 8 + file] = type | color;
					file++;
				}
				break;
		}
	}
}

void Board::handleEvent(const sf::Event &e) {
	switch(e.type) {
		case sf::Event::EventType::MouseButtonPressed:
			if(e.mouseButton.button == sf::Mouse::Left)
				handlePieceDrag();

			break;

		case sf::Event::EventType::MouseButtonReleased:
			if(e.mouseButton.button == sf::Mouse::Left)
				handlePieceDrop();

			break;

		default:
			break;
	}
}
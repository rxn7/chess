#include "board.h"
#include "chess/board_renderer.h"
#include "chess/board_theme.h"
#include "chess/piece.h"
#include <SFML/Graphics/RenderWindow.hpp>
#include <cctype>
#include <memory>

Board::Board(sf::RenderWindow &window, const sf::Font &font, const BoardTheme &theme) : m_window(window), m_boardRenderer(font, theme) {
	resetBoard();
}

void Board::render() {
	m_boardRenderer.renderSquares(m_window);
	renderPieces();
	m_boardRenderer.renderCoords(m_window);
	renderHeldPiece();
}

void Board::renderPieces() {
	for(auto i=0; i<m_pieces.size(); ++i)
		m_pieceRenderer.renderPiece(m_window, m_pieces[i], i);
}

void Board::renderHeldPiece() {
	if(Piece::isNull(m_heldPiece.value))
		return;

	sf::Vector2f pos = m_window.mapPixelToCoords(sf::Mouse::getPosition(m_window));
	pos.x -= 32;
	pos.y -= 32;

	m_pieceRenderer.renderPiece(m_window, m_heldPiece.value, pos);
}

void Board::resetBoard() {
	m_heldPiece.reset();
	std::fill(m_pieces.begin(), m_pieces.end(), 0);
	applyFen(DEFAULT_FEN);
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

		// TODO: Check if move is legal
		case sf::Event::EventType::MouseButtonReleased:
			if(e.mouseButton.button == sf::Mouse::Left)
				handlePieceDrop();

			break;

		default:
			break;
	}
}

void Board::handlePieceDrag() {
	if(!Piece::isNull(m_heldPiece.value))
		return;

	sf::Vector2f pos = m_window.mapPixelToCoords(sf::Mouse::getPosition(m_window));
	sf::Vector2i gridPos(pos.x / 64, pos.y / 64);
	uint8_t idx = gridPos.y * 8 + gridPos.x;

	if(idx < 0 || idx >= 64)
		return;

	PieceValue piece = m_pieces[idx];
	if(!Piece::isNull(piece)) {
		m_pieces[idx] = 0;
		m_heldPiece.value = piece;
		m_heldPiece.previousIdx = idx;
	}
}

void Board::handlePieceDrop() {
	if(Piece::isNull(m_heldPiece.value))
		return;

	sf::Vector2f pos = m_window.mapPixelToCoords(sf::Mouse::getPosition(m_window));
	sf::Vector2i gridPos(pos.x / 64, pos.y / 64);
	uint8_t idx = gridPos.y * 8 + gridPos.x;

	if(idx < 0 || idx >= 64) {
		m_pieces[m_heldPiece.previousIdx] = m_heldPiece.value;
		m_heldPiece.reset();
		return;
	}

	PieceValue piece = m_pieces[idx];
	if(Piece::isNull(piece) || (piece & COLOR_MASK) != (m_heldPiece.value & COLOR_MASK)) {
		m_pieces[idx] = m_heldPiece.value;
		m_heldPiece.reset();
	} else if(m_heldPiece.previousIdx > 0 && m_heldPiece.previousIdx < 64) {
		m_pieces[m_heldPiece.previousIdx] = m_heldPiece.value;
		m_heldPiece.reset();
	}
}
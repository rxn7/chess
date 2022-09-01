#include "board.h"
#include "chess/board_renderer.h"
#include "chess/board_theme.h"
#include "chess/piece.h"
#include <SFML/Graphics/RenderWindow.hpp>
#include <cctype>
#include <memory>

#define DEFAULT_FEN "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR"

Board::Board(const sf::Font &font, const BoardTheme &theme) : m_boardRenderer(font, theme) {
	std::fill(m_pieces.begin(), m_pieces.end(), 0);
	applyFen(DEFAULT_FEN);
}

void Board::render(sf::RenderWindow &window) {
	m_boardRenderer.renderSquares(window);
	renderPieces(window);
	m_boardRenderer.renderCoords(window);
	renderHeldPiece(window);
}

void Board::renderPieces(sf::RenderWindow &window) {
	for(auto i=0; i<m_pieces.size(); ++i)
		m_pieceRenderer.renderPiece(window, m_pieces[i], i);
}

void Board::renderHeldPiece(sf::RenderWindow &window) {
	if(Piece::isNull(m_heldPiece.value))
		return;

	sf::Vector2f pos = window.mapPixelToCoords(sf::Mouse::getPosition(window));
	pos.x -= 32;
	pos.y -= 32;

	m_pieceRenderer.renderPiece(window, m_heldPiece.value, pos);
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

void Board::handleEvent(sf::RenderWindow &window, const sf::Event &e) {
	switch(e.type) {
		case sf::Event::EventType::MouseButtonPressed:
			if(e.mouseButton.button == sf::Mouse::Left) {
				sf::Vector2f pos = window.mapPixelToCoords(sf::Mouse::getPosition(window));
				sf::Vector2i gridPos(pos.x / 64, pos.y / 64);
				uint8_t idx = gridPos.y * 8 + gridPos.x;

				PieceValue piece = m_pieces[idx];
				if(!m_heldPiece.value && piece) {
					m_pieces[idx] = 0;
					m_heldPiece.value = piece;
					m_heldPiece.previousIdx = idx;
				}
			}
			break;

		// TODO: Check if move is legal
		case sf::Event::EventType::MouseButtonReleased:
			if(e.mouseButton.button == sf::Mouse::Left) {
				if(Piece::isNull(m_heldPiece.value))
					return;

				sf::Vector2f pos = window.mapPixelToCoords(sf::Mouse::getPosition(window));
				sf::Vector2i gridPos(pos.x / 64, pos.y / 64);
				uint8_t idx = gridPos.y * 8 + gridPos.x;

				if(idx < 0 || idx >= 64) {
					m_pieces[m_heldPiece.previousIdx] = m_heldPiece.value;
					m_heldPiece.restart();
					return;
				}

				PieceValue piece = m_pieces[idx];
				if(Piece::isNull(piece) || (piece & COLOR_MASK) != (m_heldPiece.value & COLOR_MASK)) {
					m_pieces[idx] = m_heldPiece.value;
					m_heldPiece.restart();
				} else if(m_heldPiece.previousIdx > 0 && m_heldPiece.previousIdx < 64) {
					m_pieces[m_heldPiece.previousIdx] = m_heldPiece.value;
					m_heldPiece.restart();
				}
			}
			break;

		default:
			break;
	}
}
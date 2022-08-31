#include "board.h"
#include "chess/board_renderer.h"
#include "chess/board_theme.h"
#include "chess/piece.h"
#include <SFML/Graphics/RenderWindow.hpp>
#include <cctype>
#include <memory>

#define DEFAULT_FEN "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR"

Board::Board(const sf::Font &font, const BoardTheme &theme) : m_boardRenderer(font, theme) {
	applyFen(DEFAULT_FEN);
	m_heldPiece = Piece::White | Piece::Bishop;
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
	sf::Vector2f pos = window.mapPixelToCoords(sf::Mouse::getPosition(window));
	pos.x -= 32;
	pos.y -= 32;
	m_pieceRenderer.renderPiece(window, m_heldPiece, pos);
}

void Board::applyFen(const std::string &fen) {
	uint8_t file = 0, rank = 7;

	for(char c : fen) {
		switch(c) {
			case '/':
				file = 0;
				rank--;
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
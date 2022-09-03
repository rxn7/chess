#include "board.h"
#include "board_renderer.h"
#include "board_theme.h"
#include "piece.h"
#include "sound_system.h"
#include <SFML/Graphics/RenderWindow.hpp>
#include <cstddef>
#include <iostream>
#include <memory>

#define MOVE_SOUND_PATH "res/move.wav"
#define TAKE_SOUND_PATH "res/move.wav"
#define HELD_PIECE m_pieces[m_heldPieceIdx]

Board::Board(sf::RenderWindow &window, const sf::Font &font, const BoardTheme &theme) : m_window(window), m_boardRenderer(m_window, font, theme) {
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

	// Render last move
	if(!m_lastMove.isNull())
		for(uint8_t idx : m_lastMove.indices)
			m_boardRenderer.renderSquareLastMove(idx);

	// Render legal moves
	for(uint8_t idx : m_legalMoves)
		m_boardRenderer.renderSquareLegalMove(idx);

	m_boardRenderer.renderSquareOutline(m_heldPieceIdx);
	m_boardRenderer.renderSquareOutline(getHoveredSquareIdx());

	renderPieces();
	m_boardRenderer.renderCoords();

	if(isAnyPieceHeld())
		renderHeldPiece();
}

bool Board::moveHeldPiece(uint8_t toIdx) {
	if(std::find(m_legalMoves.begin(), m_legalMoves.end(), toIdx) == m_legalMoves.end())
		return false;

	if(Piece::isNull(m_pieces[toIdx]))
		SoundSystem::playSound(SoundType::Move);
	else if((m_pieces[toIdx] & COLOR_MASK) != (HELD_PIECE & COLOR_MASK))
		SoundSystem::playSound(SoundType::Take);
	else
		return false;

	m_pieces[toIdx] = HELD_PIECE;
	m_pieces[m_heldPieceIdx] = Piece::None;
	
	m_lastMove.fromIdx = m_heldPieceIdx;
	m_lastMove.toIdx = toIdx;

	processPawnPromotion(toIdx);
	resetHeldPiece();

	m_turnColor = m_turnColor == Piece::White ? Piece::Black : Piece::White;

	return true;
}

void Board::processPawnPromotion(uint8_t idx) {
	PieceValue piece = m_pieces[idx];

	if((piece & TYPE_MASK) != Piece::Pawn)
		return;

	uint8_t color = piece & COLOR_MASK;
	if(color == Piece::White) {
		if(idx >= 0 && idx < 8)
			m_pieces[idx] = Piece::Queen | Piece::White;
	} else {
		if(idx >= 8*7 && idx < 8*8)
			m_pieces[idx] = Piece::Queen | Piece::Black;
	}
}

void Board::handlePieceDrag() {
	if(isAnyPieceHeld())
		return;

	sf::Vector2f pos = m_window.mapPixelToCoords(sf::Mouse::getPosition(m_window));
	sf::Vector2i gridPos(pos.x / 64, pos.y / 64);
	uint8_t idx = gridPos.y * 8 + gridPos.x;

	if(!isSquareIdxCorrect(idx))
		return;

	PieceValue piece = m_pieces[idx];
	if(!Piece::isNull(piece) && (piece & COLOR_MASK) == m_turnColor) {
		m_heldPieceIdx = idx;
		Piece::getLegalMoves(idx, m_pieces, m_legalMoves);
	}
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

	if(!isSquareIdxCorrect(idx)) {
		resetHeldPiece();
		return;
	}

	PieceValue piece = m_pieces[idx];


	if(!moveHeldPiece(idx))
		resetHeldPiece();
}

void Board::renderPieces() {
	for(auto i=0; i<m_pieces.size(); ++i)
		if(i != m_heldPieceIdx) 
			m_pieceRenderer.renderPiece(m_window, m_pieces[i], i);
}

void Board::renderHeldPiece() {
	sf::Vector2f pos = m_window.mapPixelToCoords(sf::Mouse::getPosition(m_window));
	pos.x -= 32;
	pos.y -= 32;

	m_pieceRenderer.renderPiece(m_window, m_pieces[m_heldPieceIdx], pos);
}

uint8_t Board::getHoveredSquareIdx() const {
	sf::Vector2i mousePos = sf::Mouse::getPosition(m_window);
	sf::Vector2f pos = m_window.mapPixelToCoords(mousePos);
	if(pos.x < 0 || pos.x > 512 || pos.y < 0 || pos.y > 512) {
		return 64;
	}

	sf::Vector2i gridPos(pos.x / 64, pos.y / 64);
	return gridPos.y * 8 + gridPos.x;
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
							std::cerr << "\e[1;33mUnknown char '" << c << "' in fen notation '" << fen << "'!\e[0m\n";
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
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

Board::Board(sf::RenderWindow &window, const sf::Font &font, const BoardTheme &theme) : m_window(window), m_boardRenderer(m_window, font, theme) {
	reset();
}

void Board::reset() {
	std::fill(m_pieces.begin(), m_pieces.end(), 0); // Clear the board

	resetHeldPiece();
	m_lastMove.reset();

	applyFen(DEFAULT_FEN);
	m_turnColor = White;
}

void Board::render() {
	m_boardRenderer.renderSquares();

	if (!m_lastMove.isNull())
		for (std::uint8_t idx : m_lastMove.indices)
			m_boardRenderer.renderSquareLastMove(idx);

	for (const std::uint8_t idx : m_legalHeldPieceMoves)
		m_boardRenderer.renderSquareLegalMove(idx);

	m_boardRenderer.renderSquareOutline(m_heldPieceIdx);
	m_boardRenderer.renderSquareOutline(getIdxFromPosition(getMousePosition()));

	renderPieces();
	m_boardRenderer.renderCoords();

	if (isAnyPieceHeld())
		renderHeldPiece();
}

bool Board::moveHeldPiece(uint8_t toIdx) {
	if (std::find(m_legalHeldPieceMoves.begin(), m_legalHeldPieceMoves.end(), toIdx) == m_legalHeldPieceMoves.end())
		return false;

	Piece &targetPiece = m_pieces[toIdx];

	if (targetPiece.isNull())
		SoundSystem::playSound(Sound::Move);
	else if (!targetPiece.isColor(getHeldPiece().getColor()))
		SoundSystem::playSound(Sound::Take);
	else
		return false;

	targetPiece = getHeldPiece();
	m_pieces[m_heldPieceIdx] = 0;

	m_lastMove.fromIdx = m_heldPieceIdx;
	m_lastMove.toIdx = toIdx;

	processPawnPromotion(toIdx);
	resetHeldPiece();

	m_turnColor = m_turnColor == White ? Black : White;

	return true;
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

void Board::handlePieceDrag() {
	if (isAnyPieceHeld())
		return;

	const std::uint8_t idx = getIdxFromPosition(getMousePosition());

	if (!isSquareIdxCorrect(idx))
		return;

	Piece &piece = m_pieces[idx];
	if (!piece.isNull() && piece.isColor(m_turnColor)) {
		m_heldPieceIdx = idx;
		Rules::getLegalMoves(m_legalHeldPieceMoves, *this, idx);
	}
}

void Board::handlePieceDrop() {
	if (!isAnyPieceHeld())
		return;

	const sf::Vector2f mousePosition = getMousePosition();
	if (mousePosition.x < 0 || mousePosition.x > 512 || mousePosition.y < 0 || mousePosition.y > 512) {
		resetHeldPiece();
		return;
	}

	const std::uint8_t idx = getIdxFromPosition(mousePosition);

	if (!isSquareIdxCorrect(idx)) {
		resetHeldPiece();
		return;
	}

	if (!moveHeldPiece(idx))
		resetHeldPiece();
}

void Board::renderPieces() {
	for (std::uint8_t i = 0; i < 64; ++i)
		if (i != m_heldPieceIdx)
			m_pieceRenderer.renderPiece(m_window, m_pieces[i], i);
}

void Board::renderHeldPiece() {
	const sf::Vector2f pos = m_window.mapPixelToCoords(sf::Mouse::getPosition(m_window)) - sf::Vector2f(32.0f, 32.0f);
	m_pieceRenderer.renderPiece(m_window, m_pieces[m_heldPieceIdx], pos);
}

void Board::applyFen(const std::string &fen) {
	uint8_t file = 0, rank = 0;

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

void Board::handleEvent(const sf::Event &e) {
	switch (e.type) {
	case sf::Event::EventType::MouseButtonPressed:
		if (e.mouseButton.button == sf::Mouse::Left)
			handlePieceDrag();
		break;

	case sf::Event::EventType::MouseButtonReleased:
		if (e.mouseButton.button == sf::Mouse::Left)
			handlePieceDrop();
		break;

	default:
		break;
	}
}

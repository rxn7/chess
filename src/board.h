#pragma once

#include "board_renderer.h"
#include "board_theme.h"
#include "move.h"
#include "piece.h"
#include "piece_renderer.h"
#include <array>
#include <memory>
#include <SFML/Audio.hpp>
#include <SFML/Graphics/RenderWindow.hpp>

#define DEFAULT_FEN "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNRwKQkq-01"

class Board {
  public:
	Board(sf::RenderWindow &window, const sf::Font &font, const BoardTheme &theme = DEFAULT_BOARD_THEME);
	void render();
	void handleEvent(const sf::Event &e);
	void applyFen(const std::string &fen);
	bool moveHeldPiece(uint8_t toIdx);
	void processPawnPromotion(uint8_t idx);
	void reset();

	inline const std::array<Piece, 64> &getPieces() const {
		return m_pieces;
	}

	inline sf::Vector2f getMousePosition() const {
		return m_window.mapPixelToCoords(sf::Mouse::getPosition(m_window));
	}

	inline std::uint8_t getIdxFromPosition(const sf::Vector2f &position) const {
		if (position.x < 0 || position.x > 512 || position.y < 0 || position.y > 512)
			return 64;

		const sf::Vector2i gridPosition(position.x / 64, position.y / 64);
		return gridPosition.y * 8 + gridPosition.x;
	}

	inline Piece getHeldPiece() const {
		return m_pieces[m_heldPieceIdx];
	}

	inline BoardRenderer &getBoardRenderer() {
		return m_boardRenderer;
	}

	inline bool isAnyPieceHeld() const {
		return m_heldPieceIdx < 64 && !m_pieces[m_heldPieceIdx].isNull();
	}

	static inline bool isSquareIdxCorrect(std::uint8_t idx) {
		return idx >= 0 && idx < 64;
	}

  private:
	void renderHeldPiece();
	void renderPieces();
	void handlePieceDrag();
	void handlePieceDrop();

	inline void resetHeldPiece() {
		m_heldPieceIdx = 255; // every number above 63 and below 0 is ignored
		m_legalHeldPieceMoves.clear();
	}

  private:
	sf::RenderWindow &m_window;
	BoardRenderer m_boardRenderer;
	PieceRenderer m_pieceRenderer;

	PieceColor m_turnColor;
	Move m_lastMove;

	std::vector<std::uint8_t> m_legalHeldPieceMoves;
	std::array<Piece, 64> m_pieces;
	std::uint8_t m_heldPieceIdx;
};

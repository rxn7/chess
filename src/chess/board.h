#pragma once

#include "board_renderer.h"
#include "chess/board_theme.h"
#include "chess/piece.h"
#include "chess/piece_renderer.h"
#include <SFML/Graphics/RenderWindow.hpp>
#include <array>
#include <memory>

struct HeldPieceData {
	PieceValue value = 0;
	uint8_t previousIdx = 64;

	void restart() {
		value = 0;
		previousIdx = 64;
	}
};

class Board {
public:
	Board(const sf::Font &font, const BoardTheme &theme = DEFAULT_BOARD_THEME);
	void render(sf::RenderWindow &window);
	inline BoardRenderer &getBoardRenderer() { return m_boardRenderer; }
	void applyFen(const std::string &fen);
	void handleEvent(sf::RenderWindow &window, const sf::Event &e);

private:
	void renderHeldPiece(sf::RenderWindow &window);
	void renderPieces(sf::RenderWindow &window);
	void handlePieceDrag(sf::RenderWindow &window);
	void handlePieceDrop(sf::RenderWindow &window);

private:
	std::array<PieceValue, 64> m_pieces;
	HeldPieceData m_heldPiece;
	BoardRenderer m_boardRenderer;
	PieceRenderer m_pieceRenderer;
};
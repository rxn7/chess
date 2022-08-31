#pragma once

#include "board_renderer.h"
#include "chess/board_theme.h"
#include "chess/piece_renderer.h"
#include <array>
#include <memory>

class Board {
public:
	Board(const sf::Font &font, const BoardTheme &theme = DEFAULT_BOARD_THEME);
	void render(sf::RenderWindow &window);
	inline BoardRenderer &getBoardRenderer() { return m_boardRenderer; }
	void applyFen(const std::string &fen);

private:
	void renderPieces(sf::RenderWindow &window);

private:
	std::array<PieceValue, 64> m_pieces;
	BoardRenderer m_boardRenderer;
	PieceRenderer m_pieceRenderer;
};
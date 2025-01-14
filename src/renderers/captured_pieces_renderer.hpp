#pragma once

#include "board.hpp"
#include "piece_renderer.hpp"

#include <SFML/Graphics/Text.hpp>

class CapturedPiecesRenderer {
public:
	CapturedPiecesRenderer(sf::Font &font);
	void render(const ChessColor color, const Board &board, PieceRenderer &pieceRenderer, sf::RenderTarget &target);

private:
	sf::Text m_materialBalanceText;
};

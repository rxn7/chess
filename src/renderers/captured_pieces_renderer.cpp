#include "captured_pieces_renderer.hpp"

CapturedPiecesRenderer::CapturedPiecesRenderer(sf::Font &font) {
	m_materialBalanceText.setFont(font);
	m_materialBalanceText.setCharacterSize(12);
	m_materialBalanceText.setFillColor(sf::Color::White);
}

void CapturedPiecesRenderer::render(const ChessColor color, const Board &board, PieceRenderer &pieceRenderer, sf::RenderTarget &target) {
	float y = 0.0f;
	if(color == board.getState().turnColor)	y = 512.0f + 8;
	else									y = -32.0f + 8;

	const std::int16_t materialBalance = board.getMaterialBalance(color);
	m_materialBalanceText.setString(std::format("{}{}", materialBalance >= 0 ? "+" : "-", std::abs(materialBalance)));
	m_materialBalanceText.setPosition(0, y + 2);
	target.draw(m_materialBalanceText);

	const Player &player = board.getPlayer(color);
	float offset = 24.0f;

	static constexpr std::array<PieceType, 5> PIECE_TYPE_ORDER = {PieceType::Pawn, PieceType::Knight, PieceType::Bishop, PieceType::Rook, PieceType::Queen};
	for(const PieceType type : PIECE_TYPE_ORDER) {
		const std::uint8_t capturedCount = player.capturedPieces.at(type);
		if(capturedCount == 0)
			continue;

		for(uint8_t j = 0; j < capturedCount; ++j) {
			const Piece piece(OPPOSITE_COLOR(color), type);
			pieceRenderer.renderPiece(target, piece, sf::Vector2f(j * 4 + offset, y), 0.3f);
		}

		offset += capturedCount * 4 + 12;
	}
}

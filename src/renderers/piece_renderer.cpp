#include "piece_renderer.hpp"

#include "board.hpp"

#include <iostream>

PieceRenderer::PieceRenderer(const std::string_view piecesTexturePath) {
	if (!m_texture.loadFromFile(std::string(piecesTexturePath))) {
		std::cerr << "\e[1;31mFailed to load pieces texture!\e[0m\n";
		return;
	}

	m_texture.setSmooth(true);
	m_texture.generateMipmap();

	generateSprites();
}

void PieceRenderer::renderPiece(sf::RenderTarget &target, Piece piece, const sf::Vector2f &pos) {
	if (piece.isNull())
		return;

	const std::unordered_map<std::uint8_t, sf::Sprite>::iterator it = m_sprites.find(piece.value);

	if (it == m_sprites.end()) {
		std::cerr << "\e[1;31mSprite for PieceValue " << (int)piece.value << " (type: " << piece.getType() << ", color: " << piece.getColor() << ") not found!\e[0m\n";
		return;
	}

	sf::Sprite &sprite = it->second;
	sprite.setPosition(pos);
	target.draw(sprite);
}

void PieceRenderer::renderPiece(sf::RenderTarget &target, Piece piece, const std::uint8_t idx) {
	if (!Board::isSquareIdxCorrect(idx))
		return;

	sf::Vector2f position;
	if(m_flipped) {
		position = sf::Vector2f((7 - idx % 8) * 64, (7 - idx / 8) * 64);
	} else {
		position = sf::Vector2f((idx % 8) * 64, idx / 8 * 64);
	}

	renderPiece(target, piece, position);
}

void PieceRenderer::generateSprites() {
	constexpr std::uint8_t TEXTURE_ATLAS_COLUMNS = 6;
	constexpr std::uint8_t TEXTURE_ATLAS_ROWS = 2;

	sf::Vector2i singleTextureSize(m_texture.getSize());
	singleTextureSize.x /= TEXTURE_ATLAS_COLUMNS;
	singleTextureSize.y /= TEXTURE_ATLAS_ROWS;

	const float scale = 64.0f / singleTextureSize.x;

	// Six types of pieces
	for (std::uint8_t c = 0; c < 6; ++c) {
		Piece piece = (c + 1) | PieceColor::White;
		const int x = c * singleTextureSize.x;

		for (std::uint8_t r = 0; r < 2; ++r) {
			const int y = r * singleTextureSize.y;

			sf::Sprite sprite;
			sprite.setTexture(m_texture);
			sprite.setTextureRect({x, y, singleTextureSize.x, singleTextureSize.y});
			sprite.setScale(scale, scale);

			m_sprites.insert({piece.value, sprite});

			piece = (c + 1) | PieceColor::Black;
		}
	}
}

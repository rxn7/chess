#include "piece_renderer.h"
#include "piece.h"
#include <iostream>
#include <unordered_map>

PieceRenderer::PieceRenderer(const std::string &piecesTexturePath) {
	if(!m_texture.loadFromFile(piecesTexturePath)) {
		std::cerr << "\e[1;31mFailed to load pieces texture!\e[0m\n";
		return;
	}

	m_texture.setSmooth(true);
	m_texture.generateMipmap();

	generateSprites();
}

void PieceRenderer::renderPiece(sf::RenderWindow &window, PieceValue piece, const sf::Vector2f &pos) {
	if(!piece || !(piece & TYPE_MASK) || !(piece & COLOR_MASK))
		return;

	std::unordered_map<PieceValue, sf::Sprite>::iterator it = m_sprites.find(piece);

	if(it == m_sprites.end()) {
		std::cerr << "\e[1;31mSprite for PieceValue " << (int)piece << " (type: " << (piece & TYPE_MASK) << ", color: " << (piece & COLOR_MASK)  << ") not found!";
		return;
	}

	sf::Sprite &sprite = it->second;
	sprite.setPosition(pos);
	window.draw(sprite);
}

void PieceRenderer::renderPiece(sf::RenderWindow &window, PieceValue piece, uint8_t positionIdx) {
	renderPiece(window, piece, sf::Vector2f((positionIdx % 8) * 64, (positionIdx / 8) * 64));
}

void PieceRenderer::generateSprites() {
	sf::Vector2i singleTextureSize(m_texture.getSize());
	singleTextureSize.x /= 6;
	singleTextureSize.y /= 2;

	float scale = 64.0f / singleTextureSize.x;

	// Six types of pieces
	for(uint8_t c=0; c<6; ++c) {
		PieceValue piece = (c+1) | Piece::Black;
		int x = c*singleTextureSize.x;

		for(uint8_t r=0; r<2; ++r) {
			int y = r*singleTextureSize.y;
			sf::Sprite sprite;
			sprite.setTexture(m_texture);
			sprite.setTextureRect({x, y, singleTextureSize.x, singleTextureSize.y});
			sprite.setScale(scale, scale);
			m_sprites.insert({piece, sprite});

			piece = (c+1) | Piece::White;
		}
	}
}
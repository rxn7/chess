#pragma once

#include "piece.h"
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <unordered_map>

class PieceRenderer {
public:
	PieceRenderer(const std::string &piecesTexurePath="res/pieces.png");
	void renderPiece(sf::RenderWindow &window, PieceValue piece, uint8_t positionIdxc);

private:
	void generateSprites();

private:
	sf::Texture m_texture;
	std::unordered_map<PieceValue, sf::Sprite> m_sprites;
};
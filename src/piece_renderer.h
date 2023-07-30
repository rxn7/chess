#pragma once

#include "piece.h"
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <unordered_map>

class PieceRenderer {
  public:
	PieceRenderer(const std::string &piecesTexurePath = "assets/pieces.png");
	void renderPiece(sf::RenderWindow &window, PieceValue piece, uint8_t idx);
	void renderPiece(sf::RenderWindow &window, PieceValue piece, const sf::Vector2f &pos);

  private:
	void generateSprites();

  private:
	sf::Texture m_texture;
	std::unordered_map<PieceValue, sf::Sprite> m_sprites;
};

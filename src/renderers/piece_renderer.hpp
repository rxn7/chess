#pragma once

#include "../piece.hpp"
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <unordered_map>

class PieceRenderer {
  public:
	PieceRenderer(const std::string_view piecesTexurePath = "assets/pieces.png");
	void renderPiece(sf::RenderTarget &target, const Piece piece, const std::uint8_t idx);
	void renderPiece(sf::RenderTarget &target, const Piece piece, const sf::Vector2f &pos);

  private:
	void generateSprites();

  private:
	sf::Texture m_texture;
	std::unordered_map<std::uint8_t, sf::Sprite> m_sprites;
};

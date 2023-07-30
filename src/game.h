#pragma once

#include "board.h"
#include "piece_renderer.h"
#include <memory>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/View.hpp>
#include <SFML/Window/Event.hpp>

const sf::Color LIGHT_COLOR(0xf0d9b5ff);
const sf::Color DARK_COLOR(0xb58863ff);
const sf::Color CLEAR_COLOR(0x282828ff);

class Game {
  public:
	Game();
	void start();

	static inline Game *getInstance() {
		return s_instance;
	}

	inline float getFps() const {
		return m_fps;
	}

	inline float getFrameDelta() const {
		return m_frameDelta;
	}

  private:
	void renderHeldPiece();
	void renderPieces();
	void handlePieceDrag();
	void handlePieceDrop();
	bool moveHeldPiece(std::uint8_t toIdx);
	void handleEvent(const sf::Event &e);
	void update();
	void render();

	inline Piece getHeldPiece() const {
		return m_board.getPieces()[m_heldPieceIdx];
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

	inline bool isAnyPieceHeld() const {
		return m_heldPieceIdx < 64 && !m_board.getPieces()[m_heldPieceIdx].isNull();
	}

	inline void resetHeldPiece() {
		m_heldPieceIdx = 255;
		m_heldPieceLegalMoves.clear();
	}

  private:
	static Game *s_instance;

	sf::RenderWindow m_window;
	Board m_board;
	BoardRenderer m_boardRenderer;
	PieceRenderer m_pieceRenderer;

	PieceColor m_turnColor;

	std::vector<std::uint8_t> m_heldPieceLegalMoves;
	std::uint8_t m_heldPieceIdx;
	sf::Font m_font;
	sf::View m_view;
	float m_fps, m_frameDelta;
};

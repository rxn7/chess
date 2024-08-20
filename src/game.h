#pragma once

#include "board.h"
#include "renderers/piece_renderer.h"
#include <memory>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/View.hpp>
#include <SFML/Window/Event.hpp>

const sf::Color LIGHT_COLOR(0xf0d9b5ff);
const sf::Color DARK_COLOR(0xb58863ff);
const sf::Color CLEAR_COLOR(0x282828ff);
constexpr float END_GAME_SCREEN_DURATION = 5.0f;

enum GameState {
	Playing,
	EndScreen
};

enum GameResult {
	WhiteWin,
	BlackWin,
	Draw
};

class Game {
  public:
	Game();
	void start();
	void end(const GameResult result);

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
	void restart();
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

		return (int)position.y / 64 * 8 + (int)position.x / 64;
	}

	inline bool isAnyPieceHeld() const {
		return m_heldPieceIdx < 64 && !m_board.getPieces()[m_heldPieceIdx].isNull();
	}

	inline void resetHeldPiece() {
		m_heldPieceIdx = 255;
	}

  private:
	static Game *s_instance;

	GameState m_state;

	sf::RenderWindow m_window;
	Board m_board;
	BoardRenderer m_boardRenderer;
	PieceRenderer m_pieceRenderer;

	std::uint8_t m_heldPieceIdx;
	sf::Text m_endGameText;
	sf::Font m_font;
	sf::View m_view;
	float m_fps, m_frameDelta;
};

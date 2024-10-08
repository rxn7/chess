#pragma once

#include "board.hpp"
#include "debug_data.hpp"
#include "renderers/board_renderer.hpp"
#include "renderers/piece_renderer.hpp"

#include <SFML/Graphics/Font.hpp>
#include <SFML/System/Vector2.hpp>
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
		return m_board.getPieces()[m_heldPieceIdx.value()];
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
		return m_heldPieceIdx && !getHeldPiece().isNull();
	}

  public:
	DebugData debugData;

  private:
	static Game *s_instance;

	GameState m_state;
	Board m_board;
	BoardRenderer m_boardRenderer;
	PieceRenderer m_pieceRenderer;

	std::optional<std::uint8_t> m_heldPieceIdx;
	float m_fps, m_frameDelta;

	sf::RenderWindow m_window;
	sf::Text m_endGameText;
	sf::Font m_font;
	sf::View m_view;
};

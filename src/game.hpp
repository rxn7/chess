#pragma once

#include "board.hpp"
#include "clock.hpp"
#include "imgui_layer.hpp"
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

enum class GameState {
	Playing,
	EndScreen
};

class Game {
public:
	Game();
	void start();
	void end(const BoardStatus status);
	void restart();
	void performAutoFlip();

	static inline Game *getInstance() {
		return s_instance;
	}

	inline std::uint8_t getRealIdx(const std::uint8_t idx) {
		return m_boardRenderer.isFlipped() ? 63 - idx : idx;
	}

	inline std::uint8_t getIdxFromPosition(const sf::Vector2f &position, const bool real) {
		if (position.x < 0 || position.x > 512 || position.y < 0 || position.y > 512)
			return 64;

		const int x = (int)position.x / 64;
		const int y = (int)position.y / 64;

		return real ? getRealIdx(y * 8 + x) : y * 8 + x;
	}

	inline float getFps() const {
		return m_fps;
	}

	inline sf::Time getFrameDelta() const {
		return m_frameDelta;
	}

	inline Board &getBoard() {
		return m_board;
	}

	inline BoardRenderer &getBoardRenderer() {
		return m_boardRenderer;
	}

private:
	void renderHeldPiece();
	void renderPieces();
	void handlePieceDrag();
	void handlePieceDrop();
	bool moveHeldPiece(std::uint8_t toIdx);
	void handleEvent(const sf::Event &e);
	void render();

	void updateClocks();
	Clock &getClock(PieceColor color);

	inline Piece getHeldPiece() const {
		return m_board.getPieces()[m_heldPieceIdx.value()];
	}

	inline sf::Vector2f getMousePosition() const {
		return m_window.mapPixelToCoords(sf::Mouse::getPosition(m_window));
	}

	inline bool isAnyPieceHeld() const {
		return m_heldPieceIdx && !getHeldPiece().isNull();
	}


public:
	bool m_autoFlip = true;

private:
	static Game *s_instance;

	GameState m_state = GameState::Playing;
	Board m_board;
	BoardRenderer m_boardRenderer;
	PieceRenderer m_pieceRenderer;

	std::optional<std::uint8_t> m_heldPieceIdx;
	sf::Time m_frameDelta;
	float m_fps;

	sf::RenderWindow m_window;
	sf::Text m_endGameText;
	sf::Font m_font;
	sf::View m_view;
	std::array<Clock, 2> m_clocks;

	ImGuiLayer m_imgui;
};

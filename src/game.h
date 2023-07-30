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
	void handleEvent(const sf::Event &e);
	void update();
	void render();

  private:
	static Game *s_instance;

	std::unique_ptr<Board> mp_board;
	sf::Font m_font;
	sf::View m_view;
	sf::RenderWindow m_window;
	float m_fps, m_frameDelta;
};

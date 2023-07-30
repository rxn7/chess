#include "game.h"
#include "board_renderer.h"
#include "board_theme.h"
#include "piece.h"
#include "piece_renderer.h"
#include "sound_system.h"
#include <iostream>
#include <memory>
#include <SFML/Graphics/Color.hpp>
#include <SFML/Window/Event.hpp>
#include <SFML/Window/Keyboard.hpp>
#include <SFML/Window/VideoMode.hpp>
#include <sstream>

Game *Game::s_instance;

Game::Game() : m_window(sf::VideoMode(512, 512), "Chess by rxn") {
	s_instance = this;
	srand(time(0));
	SoundSystem::init();

	if (!m_font.loadFromFile("assets/RobotoMono-Regular.ttf")) {
		std::cerr << "\e[1;31mFailed to load font RobotoMono-Regular!\e[0m\n";
		m_window.close();
	}

	mp_board = std::make_unique<Board>(m_window, m_font, DEFAULT_BOARD_THEME);

	m_view.setCenter(256, 256);
	m_view.setSize(512, 512);

	m_window.setVerticalSyncEnabled(true);
	m_window.setKeyRepeatEnabled(false);

	std::cout << "\e[1;32mPress 'T' to generate random board theme!\e[0m" << std::endl;
	std::cout << "\e[1;32mPress 'R' to bring back the default board theme!\e[0m" << std::endl;
	std::cout << "\e[1;32mPress 'Escape' to reset the board!\e[0m" << std::endl;
}

void Game::start() {
	sf::Event e;
	sf::Clock frameClock;

	while (true) {
		m_frameDelta = frameClock.restart().asSeconds();
		m_fps = 1.0f / m_frameDelta;

		while (m_window.pollEvent(e))
			handleEvent(e);

		if (!m_window.isOpen())
			break;

		update();
		render();
	}
}

void Game::update() {}

void Game::render() {
	m_window.setView(m_view);
	m_window.clear(CLEAR_COLOR);
	mp_board->render();
	m_window.display();
}

void Game::handleEvent(const sf::Event &e) {
	mp_board->handleEvent(e);

	switch (e.type) {
	case sf::Event::Closed:
		m_window.close();
		break;

	case sf::Event::Resized: {
		const float winRatio = e.size.width / (float)e.size.height;
		const float viewRatio = m_view.getSize().x / (float)m_view.getSize().y;
		float sizeX = 1, sizeY = 1;
		float posX = 0, posY = 0;

		if (winRatio > viewRatio) {
			sizeX = viewRatio / winRatio;
			posX = (1 - sizeX) * 0.5f;
		} else {
			sizeY = winRatio / viewRatio;
			posY = (1 - sizeY) * 0.5f;
		}

		m_view.setViewport({posX, posY, sizeX, sizeY});

		break;
	}

	case sf::Event::KeyPressed:
		switch (e.key.code) {
		case sf::Keyboard::Key::T:
			mp_board->getBoardRenderer().setTheme(BoardTheme::generateRandomTheme());
			break;

		case sf::Keyboard::Key::R:
			mp_board->getBoardRenderer().setTheme(DEFAULT_BOARD_THEME);
			break;

		case sf::Keyboard::Key::Escape:
			mp_board->reset();
			break;

		default:
			break;
		}

	default:
		break;
	}
}

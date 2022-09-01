#include "game.h"
#include "chess/board_renderer.h"
#include "chess/board_theme.h"
#include "chess/piece_renderer.h"
#include "chess/piece.h"
#include <SFML/Graphics/Color.hpp>
#include <SFML/Window/Event.hpp>
#include <SFML/Window/Keyboard.hpp>
#include <SFML/Window/VideoMode.hpp>
#include <iostream>
#include <memory>

Game::Game() : m_window(sf::VideoMode(512, 512), "Chess by rxn") {
	srand(time(0));

	if(!m_font.loadFromFile("res/RobotoMono-Regular.ttf")) {
		std::cerr << "\e[1;31mFailed to load font RobotoMono-Regular!\e[0m\n";
		m_window.close();
	}

	m_board = std::make_unique<Board>(m_window, m_font, DEFAULT_BOARD_THEME);

	m_view.setCenter(256, 256);
	m_view.setSize(512, 512);
	m_window.setVerticalSyncEnabled(true);
	m_window.setKeyRepeatEnabled(false);

	std::cout << "\e[1;32mPress 'T' to generate random board theme!\e[0m" << std::endl;
}

void Game::start() {
	sf::Event e;
	for(;;) {
		while(m_window.pollEvent(e))
			handleEvent(e);

		if(!m_window.isOpen())
			break;

		m_window.setView(m_view);
		m_window.clear(CLEAR_COLOR);
		m_board->render();
		m_window.display();
	}
}


void Game::handleEvent(const sf::Event &e) {
	m_board->handleEvent(e);

	switch(e.type) {
		case sf::Event::Closed:
			m_window.close();
			break;

		case sf::Event::Resized: {
			float winRatio = e.size.width / (float)e.size.height;
			float viewRatio = m_view.getSize().x / (float)m_view.getSize().y;
			float sizeX = 1, sizeY = 1;
			float posX = 0, posY = 0;

			if(winRatio > viewRatio) {
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
			switch(e.key.code) {
				case sf::Keyboard::Key::T:
					m_board->getBoardRenderer().setTheme(BoardTheme::generateRandomTheme());
					break;

				default:
					break;
			}

		default:
			break;
	}
}
#include "game.h"
#include "chess/board_renderer.h"
#include "chess/board_theme.h"
#include <SFML/Graphics/Color.hpp>
#include <SFML/Window/Event.hpp>
#include <SFML/Window/Keyboard.hpp>
#include <SFML/Window/VideoMode.hpp>
#include <iostream>
#include <memory>

Game::Game() : m_window(sf::VideoMode(512, 512), "Chess by rxn") {
	if(!m_font.loadFromFile("res/RobotoMono-Regular.ttf")) {
		std::cerr << "Failed to load font RobotoMono-Regular!";
		m_window.close();
	}

	m_boardRenderer = std::make_unique<BoardRenderer>(m_font, BoardTheme::generateRandomTheme());

	m_view.setCenter(256, 256);
	m_window.setVerticalSyncEnabled(true);
	m_window.setKeyRepeatEnabled(false);

	std::cout << "\e[1;32mPress 'T' to generate random board theme!\e[0m" << std::endl;
}

void Game::start() {
	srand(time(0));

	sf::Event e;
	for(;;) {
		while(m_window.pollEvent(e))
			handleEvent(e);

		if(!m_window.isOpen())
			break;

		m_window.setView(m_view);
		m_window.clear(CLEAR_COLOR);
		m_boardRenderer->render(m_window);
		m_window.draw(m_infoText);
		m_window.display();
	}
}


void Game::handleEvent(const sf::Event &e) {
	switch(e.type) {
		case sf::Event::Closed:
			m_window.close();
			break;

		// TODO: Make sure the board is always visible.
		case sf::Event::Resized: {
			sf::Vector2f size(m_window.getSize());
			m_view.setSize(size);
			break;
		}

		case sf::Event::KeyPressed:
			switch(e.key.code) {
				case sf::Keyboard::Key::T:
					m_boardRenderer->setTheme(BoardTheme::generateRandomTheme());
					break;

				default:
					break;
			}

		default:
			break;
	}
}

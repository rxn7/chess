#include "game.h"
#include "board.h"
#include "renderers/board_renderer.h"
#include "renderers/piece_renderer.h"
#include "board_theme.h"
#include "rules.h"
#include "piece.h"
#include "audio.h"

#include <iostream>
#include <SFML/Graphics/Color.hpp>
#include <SFML/Window/Event.hpp>
#include <SFML/Window/Keyboard.hpp>
#include <SFML/Window/VideoMode.hpp>
#include <sstream>

Game *Game::s_instance;

Game::Game() : m_window(sf::VideoMode(512, 512), "Chess by rxn"), m_heldPieceIdx(255) {
	s_instance = this;
	srand(time(0));
	Audio::init();

	if(!m_font.loadFromFile("assets/RobotoMono-Regular.ttf")) {
		std::cout << "\e[1;31mFailed to load font!\e[0m" << std::endl;
		return;
	}

	m_view.setCenter(256, 256);
	m_view.setSize(512, 512);

	m_window.setVerticalSyncEnabled(true);
	m_window.setKeyRepeatEnabled(false);

	m_endGameText.setFillColor(sf::Color::Black);
	m_endGameText.setOutlineColor(sf::Color::White);
	m_endGameText.setOutlineThickness(2);
	m_endGameText.setPosition(256, 256);
	m_endGameText.setCharacterSize(32);
	m_endGameText.setFont(m_font);

	std::cout << "\e[1;32mPress 'T' to generate random board theme!\e[0m" << std::endl;
	std::cout << "\e[1;32mPress 'R' to bring back the default board theme!\e[0m" << std::endl;
	std::cout << "\e[1;32mPress 'Escape' to reset the board!\e[0m" << std::endl;

	restart();
}

void Game::start() {
	sf::Event e;
	sf::Clock frameClock;

	while(true) {
		m_frameDelta = frameClock.restart().asSeconds();
		m_fps = 1.0f / m_frameDelta;

		while(m_window.pollEvent(e))
			handleEvent(e);

		if(!m_window.isOpen())
			break;

		render();
	}
}

void Game::end(const GameResult result) {
	m_state = GameState::EndScreen;

	std::ostringstream ss;

	switch(result) {
		case GameResult::WhiteWin:
			ss << "White wins";
			break;
		case GameResult::BlackWin:
			ss << "Black wins";
			break;
		case GameResult::Draw:
			ss << "Draw";
			break;
	}

	ss << "\nPress any key to restart";
	m_endGameText.setString(ss.str());
	
	m_endGameText.setOrigin(m_endGameText.getLocalBounds().width * 0.5f, m_endGameText.getLocalBounds().height * 0.5f);
}

void Game::restart() {
	m_state = GameState::Playing;
	m_board.reset();
}

void Game::render() {
	m_window.setView(m_view);
	m_window.clear(CLEAR_COLOR);

	m_boardRenderer.renderSquares(m_window);

	const CheckResult &checkResult = m_board.getCheckResult();
	if(checkResult.isCheck) {
		m_boardRenderer.renderSquareCheck(m_window, checkResult.kingIdx);
		m_boardRenderer.renderSquareCheck(m_window, checkResult.checkingPieceIdx);
	}

	if(!m_board.getLastMove().isNull())
		for(std::uint8_t idx : m_board.getLastMove().indices)
			m_boardRenderer.renderSquareLastMove(m_window, idx);

	for(const Move &move : m_board.getLegalMoves())
		if(move.fromIdx == m_heldPieceIdx)
			m_boardRenderer.renderSquareLegalMove(m_window, move.toIdx);

	m_boardRenderer.renderSquareOutline(m_window, m_heldPieceIdx);
	m_boardRenderer.renderSquareOutline(m_window, getIdxFromPosition(getMousePosition()));

	renderPieces();
	m_boardRenderer.renderCoords(m_window);

	if(isAnyPieceHeld())
		renderHeldPiece();

	if(m_state == GameState::EndScreen)
		m_window.draw(m_endGameText);

	m_window.display();
}

bool Game::moveHeldPiece(std::uint8_t toIdx) {
	if(!Rules::isMoveLegal(m_board.getLegalMoves(), Move(getHeldPiece(), m_heldPieceIdx, toIdx)))
		return false;

	const Piece &targetPiece = m_board.getPieces()[toIdx];
	const bool capture = !targetPiece.isNull();

	m_board.applyMove(Move(getHeldPiece(), m_heldPieceIdx, toIdx), true, true);

	if(capture) {
		Audio::playSound(Sound::Capture);
	} else {
		Audio::playSound(Sound::Move);
	}

	if(m_board.getCheckResult().isCheck) {
		if(m_board.getLegalMoves().empty()) {
			std::cout << "\e[1;32mCheckmate!\e[0m" << std::endl;
			Audio::playSound(Sound::Checkmate);
			end(m_board.getTurnColor() == White ? GameResult::BlackWin : GameResult::WhiteWin);
		} else {
			Audio::playSound(Sound::Check);
		}
	} else {
		if(m_board.getLegalMoves().empty()) {
			std::cout << "\e[1;32mStalemate!\e[0m" << std::endl;
			Audio::playSound(Sound::Stalemate);
			end(GameResult::Draw);
		}
	}

	resetHeldPiece();
	return true;
}

void Game::handlePieceDrag() {
	const std::uint8_t idx = getIdxFromPosition(getMousePosition());
	if(!Board::isSquareIdxCorrect(idx))
		return;

	const Piece &piece = m_board.getPieces()[idx];

	if(piece.isNull() || piece.isNotColor(m_board.getTurnColor())) {
		return;
	}

	m_heldPieceIdx = idx;
}

void Game::handlePieceDrop() {
	if(!isAnyPieceHeld())
		return;

	const sf::Vector2f mousePosition = getMousePosition();
	if(mousePosition.x < 0 || mousePosition.x > 512 || mousePosition.y < 0 || mousePosition.y > 512) {
		resetHeldPiece();
		return;
	}

	const std::uint8_t idx = getIdxFromPosition(mousePosition);

	if(!Board::isSquareIdxCorrect(idx)) {
		resetHeldPiece();
		return;
	}

	if(!moveHeldPiece(idx))
		resetHeldPiece();
}

void Game::renderPieces() {
	for(std::uint8_t i = 0; i < 64; ++i)
		if(i != m_heldPieceIdx)
			m_pieceRenderer.renderPiece(m_window, m_board.getPieces()[i], i);
}

void Game::renderHeldPiece() {
	const sf::Vector2f pos = m_window.mapPixelToCoords(sf::Mouse::getPosition(m_window)) - sf::Vector2f(32.0f, 32.0f);
	m_pieceRenderer.renderPiece(m_window, m_board.getPieces()[m_heldPieceIdx], pos);
}

void Game::handleEvent(const sf::Event &e) {
	switch(e.type) {
		default: break;

		case sf::Event::Closed:
			m_window.close();
			break;

		case sf::Event::Resized: {
			const float winRatio = e.size.width /(float)e.size.height;
			const float viewRatio = m_view.getSize().x /(float)m_view.getSize().y;
			float sizeX = 1, sizeY = 1;
			float posX = 0, posY = 0;

			if(winRatio > viewRatio) {
				sizeX = viewRatio / winRatio;
				posX =(1 - sizeX) * 0.5f;
			} else {
				sizeY = winRatio / viewRatio;
				posY =(1 - sizeY) * 0.5f;
			}

			m_view.setViewport({posX, posY, sizeX, sizeY});

			break;
		}
	}

		switch(e.type) {
			case sf::Event::EventType::MouseButtonPressed:
				if(e.mouseButton.button == sf::Mouse::Left)
					handlePieceDrag();
				break;

			case sf::Event::EventType::MouseButtonReleased:
				if(e.mouseButton.button == sf::Mouse::Left)
					handlePieceDrop();
				break;

			case sf::Event::KeyPressed:
				if(m_state == GameState::EndScreen) {
					restart();
					break;
				}
				
				switch(e.key.code) {
					case sf::Keyboard::Key::T:
						m_boardRenderer.setTheme(BoardTheme::generateRandomTheme());
						break;

					case sf::Keyboard::Key::R:
						m_boardRenderer.setTheme(DEFAULT_BOARD_THEME);
						break;

					case sf::Keyboard::Key::Escape:
						restart();
						break;

					default:
						break;
					}
				break;

			default: break;
	}
}

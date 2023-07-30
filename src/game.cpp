#include "game.h"
#include "board_renderer.h"
#include "board_theme.h"
#include "legal_moves.h"
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

Game::Game() : m_window(sf::VideoMode(512, 512), "Chess by rxn"), m_turnColor(White) {
	s_instance = this;
	srand(time(0));
	SoundSystem::init();

	if (!m_font.loadFromFile("assets/RobotoMono-Regular.ttf")) {
		std::cerr << "\e[1;31mFailed to load font RobotoMono-Regular!\e[0m\n";
		m_window.close();
	}

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

		render();
	}
}

void Game::render() {
	m_window.setView(m_view);
	m_window.clear(CLEAR_COLOR);

	m_boardRenderer.renderSquares(m_window);

	if (!m_board.m_lastMove.isNull())
		for (std::uint8_t idx : m_board.m_lastMove.indices)
			m_boardRenderer.renderSquareLastMove(m_window, idx);

	for (const std::uint8_t idx : m_heldPieceLegalMoves)
		m_boardRenderer.renderSquareLegalMove(m_window, idx);

	m_boardRenderer.renderSquareOutline(m_window, m_heldPieceIdx);
	m_boardRenderer.renderSquareOutline(m_window, getIdxFromPosition(getMousePosition()));

	const std::pair<bool, std::uint8_t> isInCheckResult = m_board.isInCheck(m_turnColor);
	if (isInCheckResult.first) {
		m_boardRenderer.renderSquareCheck(m_window, isInCheckResult.second);
	}

	renderPieces();
	m_boardRenderer.renderCoords(m_window);

	if (isAnyPieceHeld())
		renderHeldPiece();

	m_window.display();
}

bool Game::moveHeldPiece(std::uint8_t toIdx) {
	if (std::find(m_heldPieceLegalMoves.begin(), m_heldPieceLegalMoves.end(), toIdx) == m_heldPieceLegalMoves.end())
		return false;

	const Piece &targetPiece = m_board.getPieces()[toIdx];

	if (targetPiece.isNull())
		SoundSystem::playSound(Sound::Move);
	else if (!targetPiece.isColor(getHeldPiece().getColor()))
		SoundSystem::playSound(Sound::Take);
	else
		return false;

	m_board.applyMove(Move(getHeldPiece(), m_heldPieceIdx, toIdx));

	resetHeldPiece();

	m_turnColor = m_turnColor == White ? Black : White;

	return true;
}

void Game::handlePieceDrag() {
	if (isAnyPieceHeld())
		return;

	const std::uint8_t idx = getIdxFromPosition(getMousePosition());

	if (!Board::isSquareIdxCorrect(idx))
		return;

	const Piece &piece = m_board.getPieces()[idx];

	if (!piece.isNull() && piece.isColor(m_turnColor)) {
		m_heldPieceIdx = idx;
		m_heldPieceLegalMoves.clear();
		addLegalMoves(m_heldPieceLegalMoves, m_board, idx);
	}
}

void Game::handlePieceDrop() {
	if (!isAnyPieceHeld())
		return;

	const sf::Vector2f mousePosition = getMousePosition();
	if (mousePosition.x < 0 || mousePosition.x > 512 || mousePosition.y < 0 || mousePosition.y > 512) {
		resetHeldPiece();
		return;
	}

	const std::uint8_t idx = getIdxFromPosition(mousePosition);

	if (!Board::isSquareIdxCorrect(idx)) {
		resetHeldPiece();
		return;
	}

	if (!moveHeldPiece(idx))
		resetHeldPiece();
}

void Game::renderPieces() {
	for (std::uint8_t i = 0; i < 64; ++i)
		if (i != m_heldPieceIdx)
			m_pieceRenderer.renderPiece(m_window, m_board.getPieces()[i], i);
}

void Game::renderHeldPiece() {
	const sf::Vector2f pos = m_window.mapPixelToCoords(sf::Mouse::getPosition(m_window)) - sf::Vector2f(32.0f, 32.0f);
	m_pieceRenderer.renderPiece(m_window, m_board.getPieces()[m_heldPieceIdx], pos);
}

void Game::handleEvent(const sf::Event &e) {
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

	case sf::Event::EventType::MouseButtonPressed:
		if (e.mouseButton.button == sf::Mouse::Left)
			handlePieceDrag();
		break;

	case sf::Event::EventType::MouseButtonReleased:
		if (e.mouseButton.button == sf::Mouse::Left)
			handlePieceDrop();
		break;

	case sf::Event::KeyPressed:
		switch (e.key.code) {
		case sf::Keyboard::Key::T:
			m_boardRenderer.setTheme(BoardTheme::generateRandomTheme());
			break;

		case sf::Keyboard::Key::R:
			m_boardRenderer.setTheme(DEFAULT_BOARD_THEME);
			break;

		case sf::Keyboard::Key::Escape:
			m_board.reset();
			break;

		default:
			break;
		}
		break;

	default:
		break;
	}
}

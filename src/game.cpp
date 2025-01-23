#include "game.hpp"
#include "board.hpp"
#include "clock.hpp"
#include "renderers/board_renderer.hpp"
#include "renderers/piece_renderer.hpp"
#include "board_theme.hpp"
#include "rules.hpp"
#include "piece.hpp"
#include "audio.hpp"

#include <iostream>
#include <SFML/Graphics/Color.hpp>
#include <SFML/System/Time.hpp>
#include <SFML/Window/Event.hpp>
#include <SFML/Window/Keyboard.hpp>
#include <SFML/Window/VideoMode.hpp>
#include <sstream>

const sf::Time CLOCK_START_TIME = sf::seconds(60 * 3);

Game *Game::s_instance;

Game::Game() : 
m_capturedPiecesRenderer(m_font),
m_heldPieceIdx(std::nullopt),
m_window(sf::VideoMode(512, 512), "Chess by rxn"),
m_clocks({Clock(ChessColor::White, m_font, sf::Vector2f()), Clock(ChessColor::Black, m_font, sf::Vector2f())}),
m_imgui(m_window) {
	s_instance = this;
	srand(time(0));
	Audio::init();

	if(!m_font.loadFromFile("assets/RobotoMono-Regular.ttf")) {
		std::cout << "\e[1;31mFailed to load font!\e[0m" << std::endl;
		return;
	}
	
	m_boardRenderer.init(m_font);
	restart();

	m_view.setCenter(256, 256);
	m_view.setSize(512, 512 + 64);
	handleResize();

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
}

void Game::start() {
	sf::Event e;
	sf::Clock frameClock;

	while(true) {
		m_frameDelta = frameClock.restart();
		m_fps = 1.0f / m_frameDelta.asSeconds();

		while(m_window.pollEvent(e))
			handleEvent(e);

		if(!m_window.isOpen())
			break;

		updateClocks();
		m_imgui.update(m_window, m_frameDelta);

		render();
	}
}

void Game::end(const BoardStatus status) {
	m_heldPieceIdx = std::nullopt;

	m_state = GameState::EndScreen;
	std::ostringstream ss;

	if(status == BoardStatus::BlackWin || status == BoardStatus::WhiteWin) {
		Audio::playSound(Sound::Checkmate);
	}

	switch(status) {
		case BoardStatus::WhiteWin:
			ss << "White wins";
			break;
		case BoardStatus::BlackWin:
			ss << "Black wins";
			break;
		case BoardStatus::Draw:
			ss << "Draw";
			break;

		default: break;
	}

	ss << "\nPress any key to restart";
	m_endGameText.setString(ss.str());
	
	m_endGameText.setOrigin(m_endGameText.getLocalBounds().width * 0.5f, m_endGameText.getLocalBounds().height * 0.5f);
}

void Game::performAutoFlip() {
	m_boardRenderer.setFlipped(m_board.getState().turnColor == ChessColor::Black);
}

void Game::restart() {
	for(Clock &clock : m_clocks) {
		clock.restart(CLOCK_START_TIME);
	}

	m_state = GameState::Playing;
	m_board.reset();
	m_heldPieceIdx = std::nullopt;
	m_boardRenderer.setFlipped(false);
}

void Game::render() {
	m_window.setView(m_view);
	m_window.clear(CLEAR_COLOR);

	renderBoard();

	if(isAnyPieceHeld())
		renderHeldPiece();

	for(Clock &clock : m_clocks) {
		clock.render(m_window, m_frameDelta.asSeconds(), clock.getColor() == m_board.getState().turnColor);
	}

	m_capturedPiecesRenderer.render(ChessColor::White, m_board, m_pieceRenderer, m_window);
	m_capturedPiecesRenderer.render(ChessColor::Black, m_board, m_pieceRenderer, m_window);

	if(m_state == GameState::EndScreen)
		m_window.draw(m_endGameText);

	m_imgui.render(m_window, *this);
	m_window.display();
}

void Game::renderBoard() {
	m_boardRenderer.renderSquares(m_window);
	const CheckResult &checkResult = m_board.getCheckResult();
	if(checkResult.isCheck) {
		m_boardRenderer.renderSquareCheck(m_window, getRealIdx(checkResult.kingIdx));
		m_boardRenderer.renderSquareCheck(m_window, getRealIdx(checkResult.checkingPieceIdx));
	}

	if(m_board.getLastMove())
		for(std::uint8_t idx : m_board.getLastMove()->indices)
			m_boardRenderer.renderSquareLastMove(m_window, getRealIdx(idx));

	for(const Move &move : m_board.getLegalMoves())
		if(move.fromIdx == m_heldPieceIdx)
			m_boardRenderer.renderSquareLegalMove(m_window, getRealIdx(move.toIdx));

	if(isAnyPieceHeld())
		m_boardRenderer.renderSquareOutline(m_window, Game::getRealIdx(m_heldPieceIdx.value()));

	m_boardRenderer.renderSquareOutline(m_window, getIdxFromPosition(getMousePosition(), false));

	renderPieces();
	m_boardRenderer.renderCoords(m_window);
}

void Game::renderCapturedPieces(ChessColor color) {
	sf::Vector2f position;

	if(m_board.getState().turnColor == color) {
	}
}

bool Game::moveHeldPiece(std::uint8_t toIdx) {
	if(!isAnyPieceHeld())
		return false;

	const Move move = Move(m_board, m_heldPieceIdx.value(), toIdx);
	if(!Rules::isMoveLegal(m_board.getLegalMoves(), move))
		return false;

	std::cout << move << std::endl;

	if(move.isCapture) {
		Audio::playSound(Sound::Capture);
	} else {
		Audio::playSound(Sound::Move);
	}

	m_board.applyMove(move, true);

	if(autoFlip) {
		performAutoFlip();
	}

	if(m_board.getStatus() != BoardStatus::Playing) {
		end(m_board.getStatus());
	} else {
		if(m_board.getCheckResult().isCheck) {
			Audio::playSound(Sound::Check);
		}
	}

	m_heldPieceIdx = std::nullopt;

	return true;
}

void Game::handlePieceDrag() {
	const std::uint8_t idx = getIdxFromPosition(getMousePosition(), true);
	if(!Board::isSquareIdxCorrect(idx))
		return;

	const Piece &piece = m_board.getPiece(idx);

	if(piece.isNull() || piece.isNotColor(m_board.getState().turnColor)) {
		return;
	}

	m_heldPieceIdx = idx;
}

void Game::handlePieceDrop() {
	if(!isAnyPieceHeld())
		return;

	const sf::Vector2f mousePosition = getMousePosition();
	if(mousePosition.x < 0 || mousePosition.x > 512 || mousePosition.y < 0 || mousePosition.y > 512) {
		m_heldPieceIdx = std::nullopt;
		return;
	}

	const std::uint8_t idx = getIdxFromPosition(mousePosition, true);

	if(!Board::isSquareIdxCorrect(idx)) {
		m_heldPieceIdx = std::nullopt;
		return;
	}

	if(!moveHeldPiece(idx))
		m_heldPieceIdx = std::nullopt;
}

void Game::renderPieces() {
	m_pieceRenderer.flipped = m_boardRenderer.isFlipped();

	for(std::uint8_t i = 0; i < 64; ++i)
		if(i != m_heldPieceIdx)
			m_pieceRenderer.renderPiece(m_window, m_board.getPiece(i), i);
}

void Game::renderHeldPiece() {
	if(!isAnyPieceHeld())
		return;

	const sf::Vector2f pos = m_window.mapPixelToCoords(sf::Mouse::getPosition(m_window)) - sf::Vector2f(32.0f, 32.0f);
	m_pieceRenderer.renderPiece(m_window, m_board.getPiece(m_heldPieceIdx.value()), pos);
}

void Game::handleEvent(const sf::Event &e) {
	m_imgui.handleEvent(m_window, e);

	switch(e.type) {
		case sf::Event::Closed:
			m_window.close();
			break;

		case sf::Event::Resized: {
			handleResize();
			break;
		}

		case sf::Event::EventType::MouseButtonPressed:
			if(m_state == GameState::Playing) {
				if(e.mouseButton.button == sf::Mouse::Left)
					handlePieceDrag();
			}
			break;

		case sf::Event::EventType::MouseButtonReleased:
			if(m_state == GameState::Playing) {
				if(e.mouseButton.button == sf::Mouse::Left)
					handlePieceDrop();
			}
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

				case sf::Keyboard::Key::Left: {
					break;
				}

				default:
					break;
				}
			break;

		default: break;
	}
}

void Game::handleResize() {
	const float winRatio = m_window.getSize().x /(float)m_window.getSize().y;
	const float viewRatio = m_view.getSize().x /(float)m_view.getSize().y;
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
}

void Game::updateClocks() {
	Clock &currentClock = getClock(m_board.getState().turnColor);

	// NOTE: Full move counter starts at 1 for some reason: "The number of the full moves in a game. It starts at 1, and is incremented after each Black's move. "
	const bool isFirstMove = m_board.getState().fullMoves == 1;
	if(!isFirstMove) { 
		currentClock.decrement(m_frameDelta);
	}

	if(m_state == GameState::Playing && currentClock.hasFinished()) {
		end(currentClock.getColor() == ChessColor::White ? BoardStatus::BlackWin : BoardStatus::WhiteWin);
	}
}

Clock &Game::getClock(ChessColor color) {
	switch(color) {
		case ChessColor::White: return m_clocks[0];
		case ChessColor::Black: return m_clocks[1];
	}
}

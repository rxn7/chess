#include "clock.hpp"
#include "SFML/Graphics/Rect.hpp"
#include <format>

Clock::Clock(ChessColor color, const sf::Font &font, const sf::Vector2f &position) : m_color(color){
	m_text.setFont(font);
	m_text.setCharacterSize(32);
	m_text.setPosition(position);
	m_text.setFillColor(color == ChessColor::White ? sf::Color::White : sf::Color::Black);
	m_text.setOutlineColor(color == ChessColor::White ? sf::Color::Black : sf::Color::White);
	m_text.setOutlineThickness(1);
}

void Clock::render(sf::RenderTarget &renderTarget, float dt, bool active) {
	int minutes = m_timeLeftSeconds / 60;
	int seconds = m_timeLeftSeconds - minutes * 60;

	std::string text = std::format("{:02}:{:02}", (int)minutes, (int)seconds);

	m_text.setString(text);
	m_text.setPosition(active ? sf::Vector2f(256, 512 + 8) : sf::Vector2f(256, -32 + 8));

	const sf::FloatRect bounds = m_text.getLocalBounds();
	m_text.move(-bounds.width / 2, -bounds.height / 2);

	renderTarget.draw(m_text);
}

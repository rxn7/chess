#pragma once

#include "SFML/Graphics/Font.hpp"
#include "SFML/Graphics/RenderTarget.hpp"
#include "SFML/Graphics/Text.hpp"
#include "SFML/System/Time.hpp"
#include "piece.hpp"
#include <algorithm>

class Clock {
public:
	Clock(PieceColor color, const sf::Font &font, const sf::Vector2f &position);
	void render(sf::RenderTarget &renderTarget, float dt, bool active);

	inline bool hasFinished() const {
		return m_timeLeftSeconds <= 0.0f;
	}

	inline void restart(sf::Time timeLeft) {
		m_timeLeftSeconds = timeLeft.asSeconds();
	}

	inline void decrement(sf::Time dt) {
		m_timeLeftSeconds = std::max(0.0f, m_timeLeftSeconds - dt.asSeconds());
	}
	
	inline PieceColor getColor() const {
		return m_color;
	}

private:
	PieceColor m_color;
	float m_timeLeftSeconds;
	sf::Text m_text;
};

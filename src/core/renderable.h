#pragma once
#pragma once

#include <SFML/Graphics/RenderWindow.hpp>

class Renderable {
public:
	virtual void render(sf::RenderWindow &window) = 0;
};
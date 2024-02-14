#include "../include/UIElement.h"

UIElement::UIElement(float x, float y)
{
	this->x = x;
	this->y = y;
}

bool UIElement::collidesMouse()
{
	// Mouse collision
	sf::Vector2i position = sf::Mouse::getPosition(Game::Window);
	return ((y <= position.y)
			   && (y >= position.y - height)
			   && (x <= position.x)
			   && (x >= position.x - width))
		|| ((position.y <= y + height) && (position.y >= y) && (position.x <= x + width) && (position.x >= x));
}
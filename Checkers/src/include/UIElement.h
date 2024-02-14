#pragma once

#include "../include/Game.h"

class UIElement
{
public:
	// Public member variables
	float x;
	float y;

	size_t width;
	size_t height;

	// Constructor/Destructor
	UIElement() = default;
	UIElement(float x, float y);
	virtual ~UIElement() = default;

	// Load resources method
	virtual void load() = 0;

	// Draw/Update
	virtual void draw() = 0;
	virtual void update() = 0;

protected:
	// Protected variables
	sf::Texture p_Texture;

	// Protected methods
	bool collidesMouse();
};
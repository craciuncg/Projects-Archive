#pragma once

class GameState
{
public:
	// Default constructor/destructor
	GameState() = default;
	virtual ~GameState() = default;

	// Draw/Update

	virtual void draw() = 0;
	virtual void update() = 0;
};

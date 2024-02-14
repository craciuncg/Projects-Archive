#pragma once

#include "../Constants.h"
#include "UIElement.h"

class UIButton : public UIElement
{
public:
	// Constructor/Destructor
	UIButton();
	UIButton(float x, float y);
	virtual ~UIButton();

	// Draw/Update
	void draw();
	void update();

	// Load resources
	void load();

	void setText(std::string text);

	// Events

	void onClick(void (*f)());

protected:
	sf::RectangleShape p_Rectangle;

	sf::Text m_Text;

	void (*click_function)();
};
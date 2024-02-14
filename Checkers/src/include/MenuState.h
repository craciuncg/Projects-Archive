#pragma once

#include "../Constants.h"
#include "GameState.h"

class MenuState : public GameState
{
public:
	MenuState();
	virtual ~MenuState();

	virtual void draw();
	virtual void update();

	friend void blackButton();
	friend void redButton();

	static bool blackClicked;
	static bool redClicked;

private:
	UIButton* m_Button;
	UIButton* m_Button1;
};
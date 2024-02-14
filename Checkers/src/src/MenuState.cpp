#include "../include/MenuState.h"
#include "../include/UIButton.h"

bool MenuState::blackClicked = false;
bool MenuState::redClicked = false;

void blackButton()
{
	MenuState::blackClicked = true;
}

void redButton()
{
	MenuState::redClicked = true;
}

MenuState::MenuState()
{
	m_Button = new UIButton((WIDTH - BUTTON_WIDTH) / 2, (HEIGHT - BUTTON_HEIGHT) / 2);
	m_Button->onClick(blackButton);
	m_Button->setText("Black");

	m_Button1 = new UIButton(m_Button->x, m_Button->y + m_Button->height + 10);
	m_Button1->onClick(redButton);
	m_Button1->setText("Red");
}

MenuState::~MenuState()
{
	delete m_Button;
	delete m_Button1;
}

void MenuState::draw()
{
	m_Button->draw();
	m_Button1->draw();
}

void MenuState::update()
{
	m_Button->update();
	m_Button1->update();
}
#include "../include/UIButton.h"

// Constructor

UIButton::UIButton()
{
	width = BUTTON_WIDTH;
	height = BUTTON_HEIGHT;
}

UIButton::UIButton(float x, float y) :
	UIElement(x, y)
{
	width = BUTTON_WIDTH;
	height = BUTTON_HEIGHT;
	load();
	m_Text.setFont(Game::font);
	m_Text.setCharacterSize(24);
	m_Text.setFillColor(sf::Color::White);
}

// Destructor

UIButton::~UIButton()
{
}

// Load Resources

void UIButton::load()
{
	if (!p_Texture.loadFromFile(BUTTON_PATH))
	{
		std::cout << "Failed to load button sprite\n";
	}
	else
	{
		p_Rectangle.setPosition(x, y);
		p_Rectangle.setTexture(&p_Texture);
		p_Rectangle.setSize(sf::Vector2f(width, height));
	}
}

void UIButton::setText(std::string text)
{
	m_Text.setString(text);
	m_Text.setPosition(x - 10 + (BUTTON_WIDTH - m_Text.getLocalBounds().width) / 2,
		y - 15 + (BUTTON_HEIGHT - m_Text.getLocalBounds().height) / 2);
}

// Draw

void UIButton::draw()
{
	Game::Window.draw(p_Rectangle);
	Game::Window.draw(m_Text);
}

// Update

void UIButton::update()
{
	if (Game::isMousePressed
		&& Game::mouseEvent.mouseButton.button == sf::Mouse::Left
		&& collidesMouse())
	{
		click_function();
	}
}

// Events

void UIButton::onClick(void (*f)())
{
	click_function = f;
}
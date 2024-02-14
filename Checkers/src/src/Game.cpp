#include "../include/Game.h"
#include "../include/Board.h"
#include "../include/MenuState.h"
#include "../include/PlayState.h"

sf::RenderWindow Game::Window(sf::VideoMode(WIDTH, HEIGHT), TITLE, sf::Style::Close);
std::chrono::duration<double> Game::deltatime;
bool Game::isKeyReleased = false;

bool Game::isMousePressed = false;
sf::Event Game::keyEvent;
sf::Event Game::mouseEvent;

sf::Font Game::font;

void _help();

/// Initialization
Game::Game()
{
	srand((unsigned)time(0)); // Random init
	Window.setFramerateLimit(60);

	m_Menu = new MenuState();
	m_Play = new PlayState();

	fontLoaded = font.loadFromFile("content/SF Atarian System.ttf");
}

// Game leaving
Game::~Game()
{
	delete m_Menu;
	delete m_Play;
}

// Draw game
void Game::draw()
{
	/// Window clear
	Window.clear();

	/// Drawing stuff
	if (MenuState::blackClicked || MenuState::redClicked)
	{
		m_Play->draw();
	}
	else
	{
		m_Menu->draw();
		if (MenuState::blackClicked)
		{
			m_Play->setPieceColor(BLACK);
		}
		if (MenuState::redClicked)
		{
			m_Play->setPieceColor(RED);
		}
	}

	//m_PlayAIState->draw();
	Window.display();
}

/// Update game
void Game::update()
{
	if (MenuState::blackClicked || MenuState::redClicked)
	{
		m_Play->update();
	}
	else
	{
		m_Menu->update();
		if (MenuState::blackClicked)
		{
			m_Play->setPieceColor(BLACK);
		}
		if (MenuState::redClicked)
		{
			m_Play->setPieceColor(RED);
		}
	}
}

/// Game loop
void Game::run()
{
	auto start = std::chrono::high_resolution_clock::now();
	while (Game::Window.isOpen())
	{
		auto en = std::chrono::high_resolution_clock::now();
		deltatime = std::chrono::duration_cast<std::chrono::duration<double>>(en - start);
		start = en;

		update();
		draw();
		sf::Event event;
		Game::isKeyReleased = false;
		Game::isMousePressed = false;
		while (Game::Window.pollEvent(event))
		{
			switch (event.type)
			{
				case sf::Event::Closed: {
					Game::Window.close();
					break;
				}

				case sf::Event::KeyReleased: {
					Game::isKeyReleased = true;
					Game::keyEvent = event;
					break;
				}

				case sf::Event::MouseButtonPressed: {
					Game::isMousePressed = true;
					Game::mouseEvent = event;
					break;
				}

				default: {
					break;
				}
			}
		}
	}
}

void _help()
{
	Game game;
	if (!game.fontLoaded)
	{
		std::cout << "Error occured loading font!"
				  << "\n";
		std::cin.get();
		return;
	}
	game.run();
}

void Game::start()
{
	Game::Window.setActive(false);
	Window.setFramerateLimit(60);
	_help();

	/// Check for close/exit
	while (Game::Window.isOpen())
	{
		sf::Event event;
		while (Game::Window.pollEvent(event))
		{
			switch (event.type)
			{
				case sf::Event::Closed: {
					Game::Window.close();
					break;
				}

				case sf::Event::KeyReleased: {
					Game::isKeyReleased = true;
					Game::keyEvent = event;
					break;
				}

				case sf::Event::MouseButtonPressed: {
					Game::isMousePressed = true;
					Game::mouseEvent = event;
					break;
				}

				default: {
					break;
				}
			}
		}
		Game::isKeyReleased = false;
		Game::isMousePressed = false;
	}
}

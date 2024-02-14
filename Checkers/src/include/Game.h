#ifndef GAME_H
#define GAME_H

#include "../Constants.h"

class Game
{
public:
	Game();
	virtual ~Game();
	void update();
	void draw();
	void run();
	static void start();

	static sf::RenderWindow Window;
	static std::chrono::duration<double> deltatime;

	static bool isKeyReleased;
	static bool isMousePressed;

	static sf::Event mouseEvent;
	static sf::Event keyEvent;

	bool fontLoaded = true;

	static sf::Font font;

protected:
private:
	// Private Game Objects

	// Fonts and text

	//sf::Text gameOverText;

	MenuState* m_Menu;
	PlayState* m_Play;
};

#endif // GAME_H

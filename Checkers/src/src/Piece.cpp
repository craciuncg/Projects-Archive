#include "../include/Piece.h"
#include "../include/Game.h"

Piece::Piece(PieceType type) :
	type(type)
{
	std::string path = "";
	switch (type)
	{
		case RED: {
			path = RED_PATH;
			break;
		}
		case BLACK: {
			path = BLACK_PATH;
			break;
		}

		case RED_QUEEN: {
			path = REDQUEEN_PATH;
			break;
		}

		case BLACK_QUEEN: {
			path = BLACKQUEEN_PATH;
			break;
		}

		default:
			break;
	}

	if (!m_Texture.loadFromFile(path))
	{
		std::cout << "Error loading piece texture!\n";
	}
	else
	{
		m_Rectangle.setTexture(&m_Texture);
		m_Rectangle.setSize(sf::Vector2f(PIECE_SIZE, PIECE_SIZE));
	}
}

void Piece::setPosition(int px, int py)
{
	m_Rectangle.setPosition(px * SQUARE_SIZE + PIECE_OFFSET - 2, py * SQUARE_SIZE + PIECE_OFFSET - 2);
}

void Piece::draw()
{
	Game::Window.draw(m_Rectangle);
}

void Piece::update()
{
}
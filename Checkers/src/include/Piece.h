#pragma once

#include "../Constants.h"
#include "Entity.h"

class Piece : public Entity
{
public:
	PieceType type;

	Piece(PieceType type);
	virtual ~Piece() = default;

	virtual void draw();
	virtual void update();

	void setPosition(int px, int py);

private:
	sf::Texture m_Texture;
	sf::RectangleShape m_Rectangle;
};
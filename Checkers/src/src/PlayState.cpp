#include "../include/PlayState.h"
#include "../include/Board.h"
#include "../include/MenuState.h"

PlayState::PlayState()
{
}

PlayState::~PlayState()
{
	if (m_Board != nullptr)
	{
		delete m_Board;
	}
}

void PlayState::setPieceColor(PieceType type)
{
	m_Board = new Board(type);
}

void PlayState::draw()
{
	m_Board->draw();
}

void PlayState::update()
{
	m_Board->update();
}
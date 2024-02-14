#pragma once

#include "../Constants.h"
#include "GameState.h"

class PlayState : public GameState
{
public:
	PlayState();
	virtual ~PlayState();

	virtual void draw();
	virtual void update();

	void setPieceColor(PieceType type);

private:
	Board* m_Board = nullptr;
};
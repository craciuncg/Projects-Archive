#pragma once

#include "../Constants.h"
#include "Pair.h"

class Move
{
public:
	MoveType type = NO_MOVE;
	std::vector<Pair> capturePath;
	Pair position;
	Pair toGo;

	Move() :
		position(0, 0),
		toGo(0, 0)
	{}
	virtual ~Move() = default;

	bool operator==(const Move& other) const
	{
		if (type == NORMAL && other.type == NORMAL)
			return (position == other.position && toGo == other.toGo);
		if (type == CAPTURE && other.type == CAPTURE)
			return capturePath == other.capturePath;
		return false;
	}

	Move& operator=(const Move& other)
	{
		type = other.type;
		capturePath = other.capturePath;
		position = other.position;
		toGo = other.toGo;

		return *this;
	}
};
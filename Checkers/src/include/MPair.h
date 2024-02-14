#pragma once

#include "Move.h"

class MPair
{
public:
	int priority;
	Move move;

	MPair(int priority, Move move)
	{
		this->priority = priority;
		this->move = move;
	}

	virtual ~MPair() = default;

	bool operator==(const MPair& other) const
	{
		return priority == other.priority;
	}

	bool operator<(const MPair& other) const
	{
		return priority > other.priority;
	}
};
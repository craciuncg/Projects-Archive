#pragma once

class Pair
{
public:
	Pair(int x, int y) :
		x(x),
		y(y) {};

	int x;
	int y;

	bool operator==(const Pair& other) const
	{
		return x == other.x && y == other.y;
	}

	Pair& operator=(const Pair& other)
	{
		x = other.x;
		y = other.y;

		return *this;
	}

	bool operator<(const Pair& other) const
	{
		return x < other.x && y < other.y;
	}
};
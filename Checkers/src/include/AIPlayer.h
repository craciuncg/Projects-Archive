#pragma once

#include "../Constants.h"
#include "MPair.h"

class AIPlayer
{
public:
	PieceType color;
	PieceType playerColor;

	AIPlayer() = default;
	virtual ~AIPlayer() = default;

	double evaluate(std::array<std::array<char, 8>, 8>& State);
	double evaluate2(std::array<std::array<char, 8>, 8>& State);

	void bestMove(std::array<std::array<char, 8>, 8>& State, Move& best);
	bool terminal(std::array<std::array<char, 8>, 8>& State);

private:
	double minmax(std::array<std::array<char, 8>, 8>& State, PieceType level, Move& best, int depth, int& alpha, int& beta);
	double minmax_captures(std::array<std::array<char, 8>, 8>& State, PieceType level);
	double minmax_captures2(std::array<std::array<char, 8>, 8>& State, PieceType level);

	MPair move_priority(std::array<std::array<char, 8>, 8>& State, Move& move, PieceType piece);
	std::vector<Move> relevantMoves(std::array<std::array<char, 8>, 8>& State, PieceType piece);

	double controlled(std::array<std::array<char, 8>, 8>& State, PieceType piece);
};
#include "../include/AIPlayer.h"
#include "../include/Utils.h"

bool AIPlayer::terminal(std::array<std::array<char, 8>, 8>& State)
{
	int redCount = 0;
	int blackCount = 0;

	for (int i = 0; i <= 6; i += 2)
	{
		for (int j = 1; j <= 7; j += 2)
		{
			switch (State[i][j])
			{
				case C_RED: {
					redCount++;
					break;
				}

				case C_RED_Q: {
					redCount++;
					break;
				}

				case C_BLACK: {
					blackCount++;
					break;
				}

				case C_BLACK_Q: {
					blackCount++;
					break;
				}

				default:
					break;
			}

			switch (State[i + 1][j - 1])
			{
				case C_RED: {
					redCount++;
					break;
				}

				case C_RED_Q: {
					redCount++;
					break;
				}

				case C_BLACK: {
					blackCount++;
					break;
				}

				case C_BLACK_Q: {
					blackCount++;
					break;
				}

				default:
					break;
			}
		}
	}

	if (redCount == 0 || blackCount == 0)
	{
		return true;
	}
	return false;
}

// Improve...
double AIPlayer::evaluate(std::array<std::array<char, 8>, 8>& State)
{
	double redCount = 0;
	double blackCount = 0;

	for (int i = 0; i <= 6; i += 2)
	{
		for (int j = 1; j <= 7; j += 2)
		{
			switch (State[i][j])
			{
				case C_RED: {
					redCount++;
					break;
				}

				case C_RED_Q: {
					redCount += 2;
					break;
				}

				case C_BLACK: {
					blackCount++;
					break;
				}

				case C_BLACK_Q: {
					blackCount += 2;
					break;
				}

				default:
					break;
			}

			switch (State[i + 1][j - 1])
			{
				case C_RED: {
					redCount++;
					break;
				}

				case C_RED_Q: {
					redCount += 2;
					break;
				}

				case C_BLACK: {
					blackCount++;
					break;
				}

				case C_BLACK_Q: {
					blackCount += 2;
					break;
				}

				default:
					break;
			}
		}
	}

	if (color == RED)
	{
		if (redCount == 0)
		{
			return -INT_MAX;
		}
		if (blackCount == 0)
		{
			return INT_MAX;
		}
		return redCount - blackCount;
	}

	if (color == BLACK)
	{
		if (blackCount == 0)
		{
			return -INT_MAX;
		}
		if (redCount == 0)
		{
			return INT_MAX;
		}
		return blackCount - redCount;
	}

	return 0;
}

double AIPlayer::evaluate2(std::array<std::array<char, 8>, 8>& State)
{
	double redCount = 0;
	double blackCount = 0;

	for (int i = 0; i <= 6; i += 2)
	{
		for (int j = 1; j <= 7; j += 2)
		{
			switch (State[i][j])
			{
				case C_RED: {
					redCount++;
					break;
				}

				case C_RED_Q: {
					redCount += 2;
					break;
				}

				case C_BLACK: {
					blackCount++;
					break;
				}

				case C_BLACK_Q: {
					blackCount += 2;
					break;
				}

				default:
					break;
			}

			switch (State[i + 1][j - 1])
			{
				case C_RED: {
					redCount++;
					break;
				}

				case C_RED_Q: {
					redCount += 2;
					break;
				}

				case C_BLACK: {
					blackCount++;
					break;
				}

				case C_BLACK_Q: {
					blackCount += 2;
					break;
				}

				default:
					break;
			}
		}
	}

	return evaluate(State) + (controlled(State, color) / (redCount + blackCount));
}

double AIPlayer::controlled(std::array<std::array<char, 8>, 8>& State, PieceType piece)
{
	double num_controlled = 0;

	std::vector<Move> blackMoves = Utils::legalMoves(State, playerColor, BLACK);
	std::vector<Move> redMoves = Utils::legalMoves(State, playerColor, RED);

	std::array<std::array<bool, 8>, 8> checked;

	for (int i = 0; i < 8; i++)
	{
		for (int j = 0; j < 8; j++)
		{
			checked[i][j] = false;
		}
	}

	switch (piece)
	{
		case RED: {
			for (Move& redMove : redMoves)
			{
				for (Move& blackMove : blackMoves)
				{
					if (redMove.toGo == blackMove.toGo && !checked[redMove.toGo.x][redMove.toGo.y])
					{
						num_controlled++;
						checked[redMove.toGo.x][redMove.toGo.y] = true;
					}
				}
			}
			break;
		}

		case BLACK: {
			for (Move& blackMove : blackMoves)
			{
				for (Move& redMove : redMoves)
				{
					if (blackMove.toGo == redMove.toGo && !checked[blackMove.toGo.x][blackMove.toGo.y])
					{
						num_controlled++;
						checked[blackMove.toGo.x][blackMove.toGo.y] = true;
					}
				}
			}
			break;
		}

		default:
			break;
	}

	return num_controlled;
}

MPair AIPlayer::move_priority(std::array<std::array<char, 8>, 8>& State, Move& move, PieceType piece)
{
	auto State_cpy = State;
	double e1 = evaluate2(State);
	Utils::performMove(State_cpy, playerColor, move);
	Pair final_pos(0, 0);

	if (move.type == NORMAL)
	{
		final_pos = move.toGo;
	}
	if (move.type == CAPTURE)
	{
		final_pos = move.capturePath[move.capturePath.size() - 1];
	}

	/*// Orice mutare care castiga jocul are prioritate infinit
	if (evaluate(State_cpy) == INT_MAX)
	{
		return MPair(INT_MAX, move);
	}

	// Orice mutare care pierde jocul are prioritate -infinit
	if (evaluate(State_cpy) == -INT_MAX)
	{
		return MPair(-INT_MAX, move);
	}
	*/

	// Mutari care aduc/pierd piese
	switch (piece)
	{
		case RED: {
			std::vector<Move> captures = Utils::captureMoves(State_cpy, playerColor, BLACK);
			if (captures.empty())
			{
				//int controlled = this->controlled(State_cpy, piece);
				return MPair(1, move);
			}
			else
			{
				int eval = minmax_captures2(State_cpy, BLACK);
				if (piece == color)
				{
					eval = eval - e1;
				}
				else
				{
					eval = e1 - eval;
				}
				if (eval > 0)
				{
					if (eval == INT_MAX)
					{
						return MPair(INT_MAX, move);
					}
					return MPair(eval + 10, move);
				}
				else if (eval < 0)
				{
					if (eval == -INT_MAX)
					{
						return MPair(-INT_MAX, move);
					}
					return MPair(eval - 1, move);
				}
				else
				{
					return MPair(0, move);
				}
			}
			break;
		}

		case BLACK: {
			std::vector<Move> captures = Utils::captureMoves(State_cpy, playerColor, RED);
			if (captures.empty())
			{
				//int controlled = this->controlled(State_cpy, piece);
				return MPair(1, move);
			}
			else
			{
				int eval = minmax_captures2(State_cpy, RED);
				if (piece == color)
				{
					eval = eval - e1;
				}
				else
				{
					eval = e1 - eval;
				}
				if (eval > 0)
				{
					if (eval == INT_MAX)
					{
						return MPair(INT_MAX, move);
					}
					return MPair(eval + 10, move);
				}
				else if (eval < 0)
				{
					if (eval == -INT_MAX)
					{
						return MPair(-INT_MAX, move);
					}
					return MPair(eval - 1, move);
				}
				else
				{
					return MPair(0, move);
				}
			}
			break;
		}

		default:
			break;
	}
	return MPair(0, move);
}

std::vector<Move> AIPlayer::relevantMoves(std::array<std::array<char, 8>, 8>& State, PieceType piece)
{
	std::vector<Move> allMoves = Utils::legalMoves(State, playerColor, piece);
	std::vector<MPair> priMoves;
	std::vector<Move> relevant;

	for (Move& move : allMoves)
	{
		priMoves.push_back(move_priority(State, move, piece));
	}

	if (priMoves.empty())
	{
		return relevant;
	}

	std::sort(priMoves.begin(), priMoves.end());
	int val = priMoves[0].priority;

	for (size_t i = 0; i < 11 && i < priMoves.size(); i++)
	{
		if (priMoves[i].priority != val)
		{
			break;
		}
		val = priMoves[i].priority;
		//std::cout << "pri: " << priMoves[i].priority << "; ";
		//Utils::printMove(priMoves[i].move);
		relevant.push_back(priMoves[i].move);
	}

	return relevant;
}

void AIPlayer::bestMove(std::array<std::array<char, 8>, 8>& State, Move& best)
{
	int alpha = -INT_MAX;
	int beta = INT_MAX;
	double eval = minmax(State, color, best, 11, alpha, beta);
	std::cout << "Evaluation: " << eval << "\n";
}

double AIPlayer::minmax_captures(std::array<std::array<char, 8>, 8>& State, PieceType level)
{
	std::vector<Move> captures = Utils::captureMoves(State, playerColor, level);

	if (captures.empty() || terminal(State))
	{
		return evaluate(State);
	}

	if (level == color)
	{
		double eval = 0;
		double max = -INT_MAX;
		for (Move& move : captures)
		{
			auto State_cpy = State;
			//Utils::printState(State_cpy);
			Utils::performMove(State_cpy, playerColor, move);
			//Utils::printMove(move);
			//Utils::printState(State_cpy);
			switch (level)
			{
				case RED: {
					//std::cout << "------------------RED---------------------\n";
					eval = minmax_captures(State_cpy, BLACK);
					break;
				}

				case BLACK: {
					//std::cout << "------------------BLACK---------------------\n";
					eval = minmax_captures(State_cpy, RED);
					break;
				}

				default:
					break;
			}

			if (eval > max)
			{
				max = eval;
			}
		}
		return max;
	}
	else
	{
		double eval = 0;
		double min = INT_MAX;
		for (Move& move : captures)
		{
			auto State_cpy = State;
			//Utils::printState(State_cpy);
			Utils::performMove(State_cpy, playerColor, move);
			//Utils::printMove(move);
			//Utils::printState(State_cpy);
			switch (level)
			{
				case RED: {
					//std::cout << "------------------RED---------------------\n";
					eval = minmax_captures(State_cpy, BLACK);
					break;
				}

				case BLACK: {
					//std::cout << "------------------BLACK---------------------\n";
					eval = minmax_captures(State_cpy, RED);
					break;
				}

				default:
					break;
			}

			if (eval < min)
			{
				min = eval;
			}
		}
		return min;
	}
}

double AIPlayer::minmax_captures2(std::array<std::array<char, 8>, 8>& State, PieceType level)
{
	std::vector<Move> captures = Utils::captureMoves(State, playerColor, level);

	if (captures.empty() || terminal(State))
	{
		return evaluate2(State);
	}

	if (level == color)
	{
		double eval = 0;
		double max = -INT_MAX;
		for (Move& move : captures)
		{
			auto State_cpy = State;
			//Utils::printState(State_cpy);
			Utils::performMove(State_cpy, playerColor, move);
			//Utils::printMove(move);
			//Utils::printState(State_cpy);
			switch (level)
			{
				case RED: {
					//std::cout << "------------------RED---------------------\n";
					eval = minmax_captures(State_cpy, BLACK);
					break;
				}

				case BLACK: {
					//std::cout << "------------------BLACK---------------------\n";
					eval = minmax_captures(State_cpy, RED);
					break;
				}

				default:
					break;
			}

			if (eval > max)
			{
				max = eval;
			}
		}
		return max;
	}
	else
	{
		double eval = 0;
		double min = INT_MAX;
		for (Move& move : captures)
		{
			auto State_cpy = State;
			//Utils::printState(State_cpy);
			Utils::performMove(State_cpy, playerColor, move);
			//Utils::printMove(move);
			//Utils::printState(State_cpy);
			switch (level)
			{
				case RED: {
					//std::cout << "------------------RED---------------------\n";
					eval = minmax_captures(State_cpy, BLACK);
					break;
				}

				case BLACK: {
					//std::cout << "------------------BLACK---------------------\n";
					eval = minmax_captures(State_cpy, RED);
					break;
				}

				default:
					break;
			}

			if (eval < min)
			{
				min = eval;
			}
		}
		return min;
	}
}

double AIPlayer::minmax(std::array<std::array<char, 8>, 8>& State, PieceType level, Move& best, int depth, int& alpha, int& beta)
{
	if (terminal(State))
	{
		return evaluate(State);
	}
	if (depth == 0)
	{
		return minmax_captures2(State, level);
	}

	//std::vector<Move> relevantMoves = this->relevantMoves(State, level);

	std::vector<Move> allMoves = this->relevantMoves(State, level);
	if (level == color)
	{
		double eval = 0;
		double max = -INT_MAX;
		for (Move& move : allMoves)
		{
			auto State_cpy = State;
			Utils::performMove(State_cpy, playerColor, move);
			best = move;
			switch (level)
			{
				case RED: {
					Move cpy_mov;
					eval = minmax(State_cpy, BLACK, cpy_mov, depth - 1, alpha, beta);
					break;
				}

				case BLACK: {
					Move cpy_mov;
					eval = minmax(State_cpy, RED, cpy_mov, depth - 1, alpha, beta);
					break;
				}

				default:
					break;
			}

			if (eval > max)
			{
				max = eval;
				best = move;
			}
			if (max >= beta)
			{
				return max;
			}
			alpha = Utils::max(alpha, max);
		}
		return max;
	}
	else
	{
		double eval = 0;
		double min = INT_MAX;
		for (Move& move : allMoves)
		{
			auto State_cpy = State;
			Utils::performMove(State_cpy, playerColor, move);
			best = move;
			switch (level)
			{
				case RED: {
					Move cpy_mov;
					eval = minmax(State_cpy, BLACK, cpy_mov, depth - 1, alpha, beta);
					break;
				}

				case BLACK: {
					Move cpy_mov;
					eval = minmax(State_cpy, RED, cpy_mov, depth - 1, alpha, beta);
					break;
				}

				default:
					break;
			}

			if (eval < min)
			{
				min = eval;
				best = move;
			}
			if (min <= alpha)
			{
				return min;
			}
			beta = Utils::min(beta, min);
		}
		return min;
	}
}
#include "../include/Utils.h"
#include "../include/Board.h"
#include "../include/Pair.h"

Utils::Utils()
{
	//ctor
}

Utils::~Utils()
{
	//dtor
}

int Utils::min(int a, int b)
{
	return (a > b) ? b : a;
}

int Utils::max(int a, int b)
{
	return (a > b) ? a : b;
}

int Utils::random_number(int l, int u)
{
	return rand() % (l - u) + l;
}

float Utils::frandom_number(float l, float u)
{
	return l + static_cast<float>(rand()) / (static_cast<float>(RAND_MAX / (u - l)));
}

void Utils::printMove(Move& move)
{
	if (move.type == NORMAL)
	{
		std::cout << "[(" << move.position.x << ", " << move.position.y << "), (" << move.toGo.x << ", " << move.toGo.y << ")]\n";
	}
	if (move.type == CAPTURE)
	{
		for (Pair& p : move.capturePath)
		{
			std::cout << "(" << p.x << ", " << p.y << "); ";
		}
		std::cout << "\n";
	}
}

bool Utils::isCapture(std::array<std::array<char, 8>, 8>& State, Direction dir, PieceType color, Pair pos)
{
	switch (dir)
	{
		case UP_LEFT: {
			if (pos.x - 2 < 0 || pos.y - 2 < 0)
			{
				return false;
			}
			return ((color == RED && (State[pos.y - 1][pos.x - 1] == C_BLACK || State[pos.y - 1][pos.x - 1] == C_BLACK_Q))
					   || (color == BLACK && (State[pos.y - 1][pos.x - 1] == C_RED || State[pos.y - 1][pos.x - 1] == C_RED_Q)))
				&& (State[pos.y - 2][pos.x - 2] == C_EMPTY);
		}
		case UP_RIGHT: {
			if (pos.x + 2 > 7 || pos.y - 2 < 0)
			{
				return false;
			}
			return ((color == RED && (State[pos.y - 1][pos.x + 1] == C_BLACK || State[pos.y - 1][pos.x + 1] == C_BLACK_Q))
					   || (color == BLACK && (State[pos.y - 1][pos.x + 1] == C_RED || State[pos.y - 1][pos.x + 1] == C_RED_Q)))
				&& (State[pos.y - 2][pos.x + 2] == C_EMPTY);
		}
		case DOWN_LEFT: {
			if (pos.x - 2 < 0 || pos.y + 2 > 7)
			{
				return false;
			}
			return ((color == RED && (State[pos.y + 1][pos.x - 1] == C_BLACK || State[pos.y + 1][pos.x - 1] == C_BLACK_Q))
					   || (color == BLACK && (State[pos.y + 1][pos.x - 1] == C_RED || State[pos.y + 1][pos.x - 1] == C_RED_Q)))
				&& (State[pos.y + 2][pos.x - 2] == C_EMPTY);
		}
		case DOWN_RIGHT: {
			if (pos.x + 2 > 7 || pos.y + 2 > 7)
			{
				return false;
			}
			return ((color == RED && (State[pos.y + 1][pos.x + 1] == C_BLACK || State[pos.y + 1][pos.x + 1] == C_BLACK_Q))
					   || (color == BLACK && (State[pos.y + 1][pos.x + 1] == C_RED || State[pos.y + 1][pos.x + 1] == C_RED_Q)))
				&& (State[pos.y + 2][pos.x + 2] == C_EMPTY);
		}

		default:
			break;
	}
	return false;
}

std::array<std::array<char, 8>, 8> Utils::readState(std::string filename)
{
	std::array<std::array<char, 8>, 8> State;
	std::ifstream fin(filename);

	for (int i = 0; i < 8; i++)
	{
		for (int j = 0; j < 8; j++)
		{
			fin >> State[i][j];
		}
	}

	return State;
}

void Utils::printState(std::array<std::array<char, 8>, 8>& State)
{
	for (int i = 0; i < 8; i++)
	{
		for (int j = 0; j < 8; j++)
		{
			std::cout << State[i][j] << " ";
		}
		std::cout << "\n";
	}
}

/*
	TODO: Check for promotion in makeMove and makeCapture, add playerColor to check
*/
void Utils::makeMove(std::array<std::array<char, 8>, 8>& State, Pair& pos, Pair& mov, PieceType playerColor)
{
	if (pos == mov)
	{
		return;
	}
	char piece = State[pos.y][pos.x];
	State[mov.y][mov.x] = piece;
	State[pos.y][pos.x] = C_EMPTY;

	switch (playerColor)
	{
		case RED: {
			if (mov.y == 7 && State[mov.y][mov.x] == C_RED)
			{
				State[mov.y][mov.x] = C_RED_Q;
			}

			if (mov.y == 0 && State[mov.y][mov.x] == C_BLACK)
			{
				State[mov.y][mov.x] = C_BLACK_Q;
			}
			break;
		}

		case BLACK: {
			if (mov.y == 0 && State[mov.y][mov.x] == C_RED)
			{
				State[mov.y][mov.x] = C_RED_Q;
			}

			if (mov.y == 7 && State[mov.y][mov.x] == C_BLACK)
			{
				State[mov.y][mov.x] = C_BLACK_Q;
			}
			break;
		}

		default:
			break;
	}
}

void Utils::makeCapture(std::array<std::array<char, 8>, 8>& State, Pair& pos, Pair& cap, PieceType playerColor)
{
	if (pos == cap)
	{
		return;
	}
	char piece = State[pos.y][pos.x];
	int replaceX = (pos.x + cap.x) / 2;
	int replaceY = (pos.y + cap.y) / 2;
	State[pos.y][pos.x] = C_EMPTY;
	State[replaceY][replaceX] = C_EMPTY;
	State[cap.y][cap.x] = piece;

	switch (playerColor)
	{
		case RED: {
			if (cap.y == 7 && State[cap.y][cap.x] == C_RED)
			{
				State[cap.y][cap.x] = C_RED_Q;
			}

			if (cap.y == 0 && State[cap.y][cap.x] == C_BLACK)
			{
				State[cap.y][cap.x] = C_BLACK_Q;
			}
			break;
		}

		case BLACK: {
			if (cap.y == 0 && State[cap.y][cap.x] == C_RED)
			{
				State[cap.y][cap.x] = C_RED_Q;
			}

			if (cap.y == 7 && State[cap.y][cap.x] == C_BLACK)
			{
				State[cap.y][cap.x] = C_BLACK_Q;
			}
			break;
		}

		default:
			break;
	}
}

void Utils::capturesPaths(MySet& paths, std::array<std::array<char, 8>, 8>& State, Pair pos, PieceType playerColor, std::vector<Pair>& cur_path)
{
	cur_path.push_back(pos);
	paths.add(cur_path);
	std::vector<Pair> captures = capturesSquare(State, playerColor, pos);
	if (captures.empty())
	{
		return;
	}
	for (Pair& cap : captures)
	{
		auto State_cpy = State;
		makeCapture(State_cpy, pos, cap, playerColor);
		auto path = cur_path;
		capturesPaths(paths, State_cpy, cap, playerColor, path);
	}
	paths.remove(cur_path);
}

std::vector<Pair> Utils::capturesSquare(std::array<std::array<char, 8>, 8>& State, PieceType color, Pair pos)
{
	std::vector<Pair> captures;

	switch (color)
	{
		case RED: {
			switch (State[pos.y][pos.x])
			{
				case C_RED: {
					if (isCapture(State, DOWN_LEFT, RED, pos))
					{
						captures.push_back(Pair(pos.x - 2, pos.y + 2));
					}
					if (isCapture(State, DOWN_RIGHT, RED, pos))
					{
						captures.push_back(Pair(pos.x + 2, pos.y + 2));
					}
					break;
				}

				case C_RED_Q: {
					if (isCapture(State, DOWN_LEFT, RED, pos))
					{
						captures.push_back(Pair(pos.x - 2, pos.y + 2));
					}
					if (isCapture(State, DOWN_RIGHT, RED, pos))
					{
						captures.push_back(Pair(pos.x + 2, pos.y + 2));
					}
					if (isCapture(State, UP_LEFT, RED, pos))
					{
						captures.push_back(Pair(pos.x - 2, pos.y - 2));
					}
					if (isCapture(State, UP_RIGHT, RED, pos))
					{
						captures.push_back(Pair(pos.x + 2, pos.y - 2));
					}
					break;
				}

				case C_BLACK: {
					if (isCapture(State, UP_LEFT, BLACK, pos))
					{
						captures.push_back(Pair(pos.x - 2, pos.y - 2));
					}
					if (isCapture(State, UP_RIGHT, BLACK, pos))
					{
						captures.push_back(Pair(pos.x + 2, pos.y - 2));
					}
					break;
				}

				case C_BLACK_Q: {
					if (isCapture(State, DOWN_LEFT, BLACK, pos))
					{
						captures.push_back(Pair(pos.x - 2, pos.y + 2));
					}
					if (isCapture(State, DOWN_RIGHT, BLACK, pos))
					{
						captures.push_back(Pair(pos.x + 2, pos.y + 2));
					}
					if (isCapture(State, UP_LEFT, BLACK, pos))
					{
						captures.push_back(Pair(pos.x - 2, pos.y - 2));
					}
					if (isCapture(State, UP_RIGHT, BLACK, pos))
					{
						captures.push_back(Pair(pos.x + 2, pos.y - 2));
					}
					break;
				}

				default:
					break;
			}
			break;
		}

		case BLACK: {
			switch (State[pos.y][pos.x])
			{
				case C_BLACK: {
					if (isCapture(State, DOWN_LEFT, BLACK, pos))
					{
						captures.push_back(Pair(pos.x - 2, pos.y + 2));
					}
					if (isCapture(State, DOWN_RIGHT, BLACK, pos))
					{
						captures.push_back(Pair(pos.x + 2, pos.y + 2));
					}
					break;
				}

				case C_RED: {
					if (isCapture(State, UP_LEFT, RED, pos))
					{
						captures.push_back(Pair(pos.x - 2, pos.y - 2));
					}
					if (isCapture(State, UP_RIGHT, RED, pos))
					{
						captures.push_back(Pair(pos.x + 2, pos.y - 2));
					}
					break;
				}

				case C_BLACK_Q: {
					if (isCapture(State, DOWN_LEFT, BLACK, pos))
					{
						captures.push_back(Pair(pos.x - 2, pos.y + 2));
					}
					if (isCapture(State, DOWN_RIGHT, BLACK, pos))
					{
						captures.push_back(Pair(pos.x + 2, pos.y + 2));
					}
					if (isCapture(State, UP_LEFT, BLACK, pos))
					{
						captures.push_back(Pair(pos.x - 2, pos.y - 2));
					}
					if (isCapture(State, UP_RIGHT, BLACK, pos))
					{
						captures.push_back(Pair(pos.x + 2, pos.y - 2));
					}
					break;
				}

				case C_RED_Q: {
					if (isCapture(State, DOWN_LEFT, RED, pos))
					{
						captures.push_back(Pair(pos.x - 2, pos.y + 2));
					}
					if (isCapture(State, DOWN_RIGHT, RED, pos))
					{
						captures.push_back(Pair(pos.x + 2, pos.y + 2));
					}
					if (isCapture(State, UP_LEFT, RED, pos))
					{
						captures.push_back(Pair(pos.x - 2, pos.y - 2));
					}
					if (isCapture(State, UP_RIGHT, RED, pos))
					{
						captures.push_back(Pair(pos.x + 2, pos.y - 2));
					}
					break;
				}

				default:
					break;
			}

			break;
		}

		default:
			break;
	}

	return captures;
}

std::vector<Pair> Utils::capturesPositions(std::array<std::array<char, 8>, 8>& State, PieceType color, PieceType piece)
{
	std::vector<Pair> positions;

	for (int i = 0; i <= 6; i += 2)
	{
		for (int j = 1; j <= 7; j += 2)
		{
			switch (piece)
			{
				case RED: {
					if (State[i][j] == C_RED || State[i][j] == C_RED_Q)
					{
						std::vector<Pair> caps = capturesSquare(State, color, Pair(j, i));
						//captures.insert(captures.end(), caps.begin(), caps.end());
						if (caps.size() > 0)
						{
							positions.push_back(Pair(j, i));
						}
					}

					if (State[i + 1][j - 1] == C_RED || State[i + 1][j - 1] == C_RED_Q)
					{
						std::vector<Pair> caps = capturesSquare(State, color, Pair(j - 1, i + 1));
						if (caps.size() > 0)
						{
							positions.push_back(Pair(j - 1, i + 1));
						}
					}

					break;
				}

				case BLACK: {
					if (State[i][j] == C_BLACK || State[i][j] == C_BLACK_Q)
					{
						std::vector<Pair> caps = capturesSquare(State, color, Pair(j, i));
						if (caps.size() > 0)
						{
							positions.push_back(Pair(j, i));
						}
					}

					if (State[i + 1][j - 1] == C_BLACK || State[i + 1][j - 1] == C_BLACK_Q)
					{
						std::vector<Pair> caps = capturesSquare(State, color, Pair(j - 1, i + 1));
						if (caps.size() > 0)
						{
							positions.push_back(Pair(j - 1, i + 1));
						}
					}
					break;
				}

				default:
					break;
			}
		}
	}

	return positions;
}

std::vector<Pair> Utils::legalMovesSquare(std::array<std::array<char, 8>, 8>& State, PieceType color, Pair pos)
{
	std::vector<Pair> moves;

	switch (color)
	{
		case RED: {
			if (State[pos.y][pos.x] == C_RED)
			{
				if (pos.y + 1 < 8 && pos.x - 1 >= 0 && State[pos.y + 1][pos.x - 1] == C_EMPTY)
				{
					moves.push_back(Pair(pos.x - 1, pos.y + 1));
				}
				if (pos.y + 1 < 8 && pos.x + 1 < 8 && State[pos.y + 1][pos.x + 1] == C_EMPTY)
				{
					moves.push_back(Pair(pos.x + 1, pos.y + 1));
				}
			}

			if (State[pos.y][pos.x] == C_BLACK)
			{
				if (pos.y - 1 >= 0 && pos.x + 1 < 8 && State[pos.y - 1][pos.x + 1] == C_EMPTY)
				{
					moves.push_back(Pair(pos.x + 1, pos.y - 1));
				}
				if (pos.x - 1 >= 0 && pos.y - 1 >= 0 && State[pos.y - 1][pos.x - 1] == C_EMPTY)
				{
					moves.push_back(Pair(pos.x - 1, pos.y - 1));
				}
			}

			if (State[pos.y][pos.x] == C_RED_Q || State[pos.y][pos.x] == C_BLACK_Q)
			{
				if (pos.y - 1 >= 0 && pos.x + 1 < 8 && State[pos.y - 1][pos.x + 1] == C_EMPTY)
				{
					moves.push_back(Pair(pos.x + 1, pos.y - 1));
				}
				if (pos.x - 1 >= 0 && pos.y - 1 >= 0 && State[pos.y - 1][pos.x - 1] == C_EMPTY)
				{
					moves.push_back(Pair(pos.x - 1, pos.y - 1));
				}
				if (pos.y + 1 < 8 && pos.x - 1 >= 0 && State[pos.y + 1][pos.x - 1] == C_EMPTY)
				{
					moves.push_back(Pair(pos.x - 1, pos.y + 1));
				}
				if (pos.y + 1 < 8 && pos.x + 1 < 8 && State[pos.y + 1][pos.x + 1] == C_EMPTY)
				{
					moves.push_back(Pair(pos.x + 1, pos.y + 1));
				}
			}
			break;
		}

		case BLACK: {
			if (State[pos.y][pos.x] == C_BLACK)
			{
				if (pos.y + 1 < 8 && pos.x - 1 >= 0 && State[pos.y + 1][pos.x - 1] == C_EMPTY)
				{
					moves.push_back(Pair(pos.x - 1, pos.y + 1));
				}
				if (pos.y + 1 < 8 && pos.x + 1 < 8 && State[pos.y + 1][pos.x + 1] == C_EMPTY)
				{
					moves.push_back(Pair(pos.x + 1, pos.y + 1));
				}
			}

			if (State[pos.y][pos.x] == C_RED)
			{
				if (pos.y - 1 >= 0 && pos.x + 1 < 8 && State[pos.y - 1][pos.x + 1] == C_EMPTY)
				{
					moves.push_back(Pair(pos.x + 1, pos.y - 1));
				}
				if (pos.x - 1 >= 0 && pos.y - 1 >= 0 && State[pos.y - 1][pos.x - 1] == C_EMPTY)
				{
					moves.push_back(Pair(pos.x - 1, pos.y - 1));
				}
			}

			if (State[pos.y][pos.x] == C_BLACK_Q || State[pos.y][pos.x] == C_RED_Q)
			{
				if (pos.y - 1 >= 0 && pos.x + 1 < 8 && State[pos.y - 1][pos.x + 1] == C_EMPTY)
				{
					moves.push_back(Pair(pos.x + 1, pos.y - 1));
				}
				if (pos.x - 1 >= 0 && pos.y - 1 >= 0 && State[pos.y - 1][pos.x - 1] == C_EMPTY)
				{
					moves.push_back(Pair(pos.x - 1, pos.y - 1));
				}
				if (pos.y + 1 < 8 && pos.x - 1 >= 0 && State[pos.y + 1][pos.x - 1] == C_EMPTY)
				{
					moves.push_back(Pair(pos.x - 1, pos.y + 1));
				}
				if (pos.y + 1 < 8 && pos.x + 1 < 8 && State[pos.y + 1][pos.x + 1] == C_EMPTY)
				{
					moves.push_back(Pair(pos.x + 1, pos.y + 1));
				}
			}
		}

		default:
			break;
	}

	return moves;
}

std::vector<Move> Utils::legalMoves(std::array<std::array<char, 8>, 8>& State, PieceType playerColor, PieceType piece)
{
	std::vector<Move> moves;

	std::vector<Pair> positionsCaptures = capturesPositions(State, playerColor, piece);

	if (positionsCaptures.empty())
	{
		for (int i = 0; i <= 6; i += 2)
		{
			for (int j = 1; j <= 7; j += 2)
			{
				switch (piece)
				{
					case RED: {
						if (State[i][j] == C_RED || State[i][j] == C_RED_Q)
						{
							std::vector<Pair> movesSquare = legalMovesSquare(State, playerColor, Pair(j, i));
							for (Pair& p : movesSquare)
							{
								Move move;
								move.type = NORMAL;
								move.position = Pair(j, i);
								move.toGo = p;
								moves.push_back(move);
							}
						}

						if (State[i + 1][j - 1] == C_RED || State[i + 1][j - 1] == C_RED_Q)
						{
							std::vector<Pair> movesSquare = legalMovesSquare(State, playerColor, Pair(j - 1, i + 1));
							for (Pair& p : movesSquare)
							{
								Move move;
								move.type = NORMAL;
								move.position = Pair(j - 1, i + 1);
								move.toGo = p;
								moves.push_back(move);
							}
						}
						break;
					}

					case BLACK: {
						if (State[i][j] == C_BLACK || State[i][j] == C_BLACK_Q)
						{
							std::vector<Pair> movesSquare = legalMovesSquare(State, playerColor, Pair(j, i));
							for (Pair& p : movesSquare)
							{
								Move move;
								move.type = NORMAL;
								move.position = Pair(j, i);
								move.toGo = p;
								moves.push_back(move);
							}
						}

						if (State[i + 1][j - 1] == C_BLACK || State[i + 1][j - 1] == C_BLACK_Q)
						{
							std::vector<Pair> movesSquare = legalMovesSquare(State, playerColor, Pair(j - 1, i + 1));
							for (Pair& p : movesSquare)
							{
								Move move;
								move.type = NORMAL;
								move.position = Pair(j - 1, i + 1);
								move.toGo = p;
								moves.push_back(move);
							}
						}
						break;
					}

					default:
						break;
				}
			}
		}
	}
	else
	{
		for (Pair& pair : positionsCaptures)
		{
			MySet paths;
			std::vector<Pair> tmp_path;
			Utils::capturesPaths(paths, State, pair, playerColor, tmp_path);

			for (auto& path : paths.elements())
			{
				Move move;
				move.type = CAPTURE;
				move.capturePath = path;
				moves.push_back(move);
			}
		}
	}

	return moves;
}

std::vector<Move> Utils::captureMoves(std::array<std::array<char, 8>, 8>& State, PieceType playerColor, PieceType piece)
{
	std::vector<Move> moves;

	std::vector<Pair> positionsCaptures = capturesPositions(State, playerColor, piece);

	for (Pair& pair : positionsCaptures)
	{
		MySet paths;
		std::vector<Pair> tmp_path;
		Utils::capturesPaths(paths, State, pair, playerColor, tmp_path);

		for (auto& path : paths.elements())
		{
			Move move;
			move.type = CAPTURE;
			move.capturePath = path;
			moves.push_back(move);
		}
	}

	return moves;
}

void Utils::performMove(std::array<std::array<char, 8>, 8>& State, PieceType playerColor, Move& move)
{
	if (move.type == NORMAL)
	{
		makeMove(State, move.position, move.toGo, playerColor);

		switch (playerColor)
		{
			case RED: {
				if (move.toGo.y == 7 && State[move.toGo.y][move.toGo.x] == C_RED)
				{
					State[move.toGo.y][move.toGo.x] = C_RED_Q;
				}

				if (move.toGo.y == 0 && State[move.toGo.y][move.toGo.x] == C_BLACK)
				{
					State[move.toGo.y][move.toGo.x] = C_BLACK_Q;
				}
				break;
			}

			case BLACK: {
				if (move.toGo.y == 0 && State[move.toGo.y][move.toGo.x] == C_RED)
				{
					State[move.toGo.y][move.toGo.x] = C_RED_Q;
				}

				if (move.toGo.y == 7 && State[move.toGo.y][move.toGo.x] == C_BLACK)
				{
					State[move.toGo.y][move.toGo.x] = C_BLACK_Q;
				}
				break;
			}

			default:
				break;
		}
	}
	if (move.type == CAPTURE)
	{
		std::vector<Pair> path = move.capturePath;
		for (size_t i = 0; i < path.size() - 1; i++)
		{
			makeCapture(State, path[i], path[i + 1], playerColor);
		}
	}
}
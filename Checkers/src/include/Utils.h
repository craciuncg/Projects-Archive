#ifndef UTILS_H
#define UTILS_H

#include "../Constants.h"
#include "Move.h"
#include "MySet.h"

enum Direction
{
	UP_LEFT,
	UP_RIGHT,
	DOWN_LEFT,
	DOWN_RIGHT
};

class Utils
{
public:
	Utils();
	virtual ~Utils();

	static int random_number(int l, int u);
	static float frandom_number(float l, float u);

	static int min(int a, int b);
	static int max(int a, int b);

	static void printMove(Move& move);

	static bool isCapture(std::array<std::array<char, 8>, 8>& State, Direction dir, PieceType color, Pair pos);

	static std::vector<Pair> legalMovesSquare(std::array<std::array<char, 8>, 8>& State, PieceType color, Pair pos);
	static std::vector<Pair> capturesSquare(std::array<std::array<char, 8>, 8>& State, PieceType color, Pair pos);

	static void capturesPaths(MySet& paths, std::array<std::array<char, 8>, 8>& State, Pair pos, PieceType playerColor, std::vector<Pair>& cur_path);
	static void makeCapture(std::array<std::array<char, 8>, 8>& State, Pair& pos, Pair& cap, PieceType playerColor);

	static void printState(std::array<std::array<char, 8>, 8>& State);
	static std::array<std::array<char, 8>, 8> readState(std::string filename);

	static void makeMove(std::array<std::array<char, 8>, 8>& State, Pair& pos, Pair& mov, PieceType playerColor);
	static std::vector<Pair> capturesPositions(std::array<std::array<char, 8>, 8>& State, PieceType color, PieceType piece);

	static std::vector<Move> legalMoves(std::array<std::array<char, 8>, 8>& State, PieceType playerColor, PieceType piece);
	static std::vector<Move> captureMoves(std::array<std::array<char, 8>, 8>& State, PieceType playerColor, PieceType piece);

	static void performMove(std::array<std::array<char, 8>, 8>& State, PieceType playerColor, Move& move);

protected:
private:
};

#endif // UTILS_H

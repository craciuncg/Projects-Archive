#pragma once

#include<opencv2/opencv.hpp>
#include <vector>
#include <algorithm>
#include <math.h>
#include <unordered_set>
#include <stack>
#include <unordered_map>

#include "Pair.h"

#define H_HALF_RECT 8
#define V_HALF_RECT 100
#define S_HALF_RECT 100

#define V_HALF_RECT1 28
#define S_HALF_RECT1 28

#define NEXT_HIERARCHY 0
#define PREV_HIERARCHY 1
#define CHILD_HIERARCHY 2
#define PARENT_HIERARCHY 3

#define NIL_VAL -1

#define MIN_SIZE 30

using namespace std;
using namespace cv;

typedef vector<Point> Contour;

// (Color, Position) pair
typedef Pair<Vec3b, Point> AreaPixel;
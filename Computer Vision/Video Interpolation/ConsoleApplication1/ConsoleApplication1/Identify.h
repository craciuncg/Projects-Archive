#pragma once

#include "Constants.h"
#include "Utils.h"

vector<int> identifyContours(const vector<Contour>&, const vector<Contour>&);
vector<int> identify(const Mat& img, const vector<Area>& A, const vector<Area>& B, const int K, const bool largeImage);
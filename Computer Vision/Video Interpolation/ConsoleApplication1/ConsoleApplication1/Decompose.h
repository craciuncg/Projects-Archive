#pragma once

#include "Constants.h"
#include "Utils.h"

vector<Contour> decompose(Mat& imgHSV, const Vec3b& color);
void decomposeHelper(int root, Mat& img, vector<Area>& areas, vector<Contour>& contours, vector<Vec4i>& hierarchy, int min_size);
vector<Area> decompose2(Mat& img, Mat& imgHSV, const Vec3b& color, const Vec3b& bounds, Mat& imgFiltered, vector<Contour>& contours, int min_size);
vector<vector<Area>> decomposeAll(vector<vector<Area>>& areasByColor, vector<vector<Contour>>& contoursByColor, Mat& img, Mat& imgHSV, const bool largeImage);

vector<vector<Area>> organize(const Mat& img, const vector<Area>& A, const int K);
vector<Area> restAreas(Mat& img, const Mat& it);
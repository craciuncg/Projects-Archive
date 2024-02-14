#pragma once

#include "Area.h"

/*
    I) Identify
    II) Union
    III) Interpolate
*/

class Utils
{
public:
    static void drawContour(const Contour& contour, const Size& size, const char* title);

    static double distance(const Point& p1, const Point& p2);
    static Rect rectanglefy(const vector<Point>& poly);
    static double distance(const Rect& r1, const Rect& r2);
    static double distance(const Contour& p1, const Contour& p2);
    static double delta(const Rect& r1, const Rect& r2);
    static double delta(const Contour& p1, const Contour& p2);
    static double delta(const Area& a1, const Area& a2);
    static int rootHelper(int node, vector<Contour>& contours, vector<Vec4i>& hierarchy);
    static bool compareDeltaPairs(Pair<Pair<int, int>, double> a, Pair<Pair<int, int>, double> b);
    static bool comparePairs(Pair<Pair<Area, Area>, double> a, Pair<Pair<Area, Area>, double> b);
};

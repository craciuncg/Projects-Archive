#include "Utils.h"

/// TODO: Get all pixels inside contour for later interpolation, store them(RGB value and position) in
/// Area type objects along with the contour and then do the identification.

double Utils::distance(const Point& p1, const Point& p2)
{
    return sqrt(pow((p1.x - p2.x), 2) + pow((p1.y - p2.y), 2));
}

Rect Utils::rectanglefy(const vector<Point>& poly)
{
    Rect rect;
    int maxX, maxY;
    int minX, minY;
    size_t size = poly.size();

    maxX = minX = poly[0].x;
    maxY = minY = poly[0].y;

    for (size_t i = 1; i < size; i++) {
        if (maxX < poly[i].x)
            maxX = poly[i].x;
        if (maxY < poly[i].y)
            maxY = poly[i].y;
        if (minX > poly[i].x)
            minX = poly[i].x;
        if (minY > poly[i].y)
            minY = poly[i].y;
    }

    rect.x = minX;
    rect.y = minY;
    rect.width = maxX - minX;
    rect.height = maxY - minY;

    return rect;
}

double Utils::distance(const Rect& r1, const Rect& r2)
{
    return distance(Point(r1.x, r1.y), Point(r2.x, r2.y));
}

double Utils::distance(const Contour& p1, const Contour& p2)
{
    Rect r1 = rectanglefy(p1);
    Rect r2 = rectanglefy(p2);

    return distance(r1, r2);
}

//  Delta rectangles
double Utils::delta(const Rect& r1, const Rect& r2)
{
    return sqrt(pow(r1.width - r2.width, 2) + pow(r1.height - r2.height, 2));
}

// Delta countours =: Metric on Contours
double Utils::delta(const Contour& p1, const Contour& p2) {
    Rect r1 = rectanglefy(p1);
    Rect r2 = rectanglefy(p2);

	double area1 = contourArea(p1);
	double area2 = contourArea(p2);

    return 1.75 * distance(r1, r2) + 0.01 * abs(area1 - area2);
}

/// TODO: Improve area calculation, take innerBounds into account
/// look for other improvements of delta

double Utils::delta(const Area& a1, const Area& a2)
{
    return delta(a1.contour, a2.contour);
}

void Utils::drawContour(const Contour& contour, const Size& size, const char* title)
{
    Mat mask = Mat::zeros(size, CV_8UC1);
    vector<Contour> contours;
    contours.push_back(contour);
    fillPoly(mask, contour, Scalar(255, 0, 0));

    namedWindow(title, WINDOW_AUTOSIZE);
    imshow(title, mask);
    waitKey(0);
}

int Utils::rootHelper(int node, vector<Contour>& contours, vector<Vec4i>& hierarchy)
{
    if (node == -1)
        return -1;

    int index = node;

    while (hierarchy[index][PREV_HIERARCHY] != -1)
        index = hierarchy[index][PREV_HIERARCHY];

    return index;
}

bool insideChildren(int index, Point& p, vector<Contour>& contours, vector<Vec4i>& hierarchy)
{
    int i = Utils::rootHelper(hierarchy[index][CHILD_HIERARCHY], contours, hierarchy);
    while (i != -1)
    {
        if (pointPolygonTest(contours[i], p, false) == 1)
        {
            return true;
        }
        i = hierarchy[i][NEXT_HIERARCHY];
    }
    return false;
}

bool Utils::compareDeltaPairs(Pair<Pair<int, int>, double> a, Pair<Pair<int, int>, double> b)
{
    return (a.second < b.second);
}

bool Utils::comparePairs(Pair<Pair<Area, Area>, double> a, Pair<Pair<Area, Area>, double> b)
{
    return (a.second < b.second);
}
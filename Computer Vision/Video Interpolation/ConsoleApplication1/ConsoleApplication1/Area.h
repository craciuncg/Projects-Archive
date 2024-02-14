#pragma once

#include "Constants.h"


class Area {
private:
public:
    Mat& img;
    Vec3b color;
    Contour contour;
    vector<Contour> innerBounds;
	Contour matchContour;

    // Store area pixels for interpolation and union
    vector<AreaPixel> areaPixels;
private:
    
    //bool contains(Point);
    //bool isColor(Vec3b, Vec3b);
public:
    Area(Mat&);
    void identifyContour(Point&);
    //void overlapWith(Area& other);

    static int getRoot(vector<Contour>&, vector<Vec4i>&);
    vector<int> getInnerBounds(int, vector<Contour>&, vector<Vec4i>&);

    Area& operator=(const Area& other);

    double areaValue() const;

    void getMask(Mat& init);
    ~Area();
};

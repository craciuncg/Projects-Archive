#include "Area.h"
#include "Utils.h"

//bool Area::contains(Point p) {
//    int size = area.size();
//    for (int i = 0; i < size; i++) {
//        if (area[i] == p)
//            return true;
//    }
//    return false;
//}
//
//bool Area::isColor(Vec3b refp, Vec3b p) {
//    double refpH = ((double)refp[0]);
//    double refpS = ((double)refp[1]);
//    double refpV = ((double)refp[2]);
//
//    double pH = ((double)p[0]);
//    double pS = ((double)p[1]);
//    double pV = ((double)p[2]);
//
//    return (pH >= refpH - H_HALF_RECT
//        && pS >= S_LOWER
//        && pV >= refpV - V_HALF_RECT
//        && pH <= refpH + H_HALF_RECT
//        && pS <= S_UPPER
//        && pV <= refpV + V_HALF_RECT);
//}

bool inContour(Point p, Contour contour) {
    return pointPolygonTest(contour, p, false) >= 0;
}

bool inContours(Point p, vector<Contour> Contours) {
    size_t size = Contours.size();
    for (size_t i = 0; i < size; i++) {
        if (inContour(p, Contours[i]))
            return true;
    }
    return false;
}

int Area::getRoot(vector<Contour>& contours, vector<Vec4i>& hierarchy) {
    int index = 0;

    while (hierarchy[index][PARENT_HIERARCHY] != -1)
        index = hierarchy[index][PARENT_HIERARCHY];
    while (hierarchy[index][PREV_HIERARCHY] != -1)
        index = hierarchy[index][PREV_HIERARCHY];
    /*while (hierarchy[index][NEXT_HIERARCHY] != -1) {
        if (contour == contours[index])
            return index;
        index = hierarchy[index][NEXT_HIERARCHY];
    }*/
    return index;
}

vector<int> Area::getInnerBounds(int c_index, vector<Contour>& contours, vector<Vec4i>& hierarchy)
{
    vector<int> inner;

    int index = hierarchy[c_index][CHILD_HIERARCHY];
    if (index != -1) {
        while (hierarchy[index][PREV_HIERARCHY] != -1) {
            index = hierarchy[index][PREV_HIERARCHY];
        }
        while (index != -1) {
            Rect r = Utils::rectanglefy(contours[index]);
            if (r.width >= MIN_SIZE || r.height >= MIN_SIZE) {
                inner.push_back(index);
                innerBounds.push_back(contours[index]);
            }
            index = hierarchy[index][NEXT_HIERARCHY];
        }
    }
    innerBounds.shrink_to_fit();

    return inner;
}

int getBeginChildren(vector<Vec4i>& hierarchy, int root) {
    int index = root;
    if (hierarchy[index][CHILD_HIERARCHY] != -1) {
        index = hierarchy[index][CHILD_HIERARCHY];
        while (hierarchy[index][PREV_HIERARCHY] != -1) {
            index = hierarchy[index][PREV_HIERARCHY];
        }
    }
    return index;
}

void Area::identifyContour(Point& p) {
   /* Mat imgHSV;
    Mat imgFiltered;
    vector<vector<Point>> contours;
    vector<Vec4i> hierarchy;
    cvtColor(img, imgHSV, COLOR_BGR2HSV);
    Vec3b pix = imgHSV.at<Vec3b>(p.y, p.x);
    int pH = ((int)pix[0]);
    int pS = ((int)pix[1]);
    int pV = ((int)pix[2]);
    inRange(imgHSV,
        Scalar(pH - H_HALF_RECT, pS - S_HALF_RECT, pV - V_HALF_RECT),
        Scalar(pH + H_HALF_RECT, pS + S_HALF_RECT, pV + V_HALF_RECT),
        imgFiltered);
    imgHSV.release();
    findContours(imgFiltered, contours, hierarchy, RETR_TREE, CHAIN_APPROX_NONE);
    /*const char* test = "test2";
    namedWindow(test, WINDOW_AUTOSIZE);
    imshow(test, imgFiltered);
    imgFiltered.release();*/

   /* size_t size = contours.size();
    size_t index = 0;
    bool found = true;
    for (size_t i = 0; i < size; i++) {
        if (inContour(p, contours[i])) {
            index = i;
            break;
        }
    }

    /*while (found && hierarchy[index][CHILD_HIERARCHY] != -1) {
        index = hierarchy[index][CHILD_HIERARCHY];
        while (true) {
            if (hierarchy[index][NEXT_HIERARCHY] == -1) {
                found = false;
                index = hierarchy[index][PARENT_HIERARCHY];
                break;
            }
            if (inContour(p, contours[index])) {
                found = true;
                break;
            }
            index = hierarchy[index][NEXT_HIERARCHY];
        }
    }
    contour = &contours[index];
    (*contour).shrink_to_fit(); // free unused memory
    index = hierarchy[index][CHILD_HIERARCHY];
    if (index != -1) {
        while (hierarchy[index][PREV_HIERARCHY] != -1) {
            index = hierarchy[index][PREV_HIERARCHY];
        }
        while (index != -1) {
            innerBounds.push_back(contours[index]);
            index = hierarchy[index][NEXT_HIERARCHY];
        }
    }
    innerBounds.shrink_to_fit(); // free unused memory
	*/
}

double Area::areaValue() const
{
    double val = contourArea(contour);

    return val;
}

Area& Area::operator=(const Area& other)
{
    Area a(other.img);
    a.areaPixels = other.areaPixels;
    a.color = other.color;
    a.contour = other.contour;
    a.innerBounds = other.innerBounds;
    return a;
}

void Area::getMask(Mat& init) {
    vector<vector<Point>> contours;
    contours.push_back(contour);
    for (auto& in : innerBounds)
    {
        contours.push_back(in);
    }
    fillPoly(init, contours, Scalar(255, 0, 0), LINE_4);
    //fillPoly(mask, innerBounds, Scalar(0, 0, 0), LINE_4);
}

// Remove common area of "this" with "other"
/*void Area::overlapWith(Area& other) {
    Mat mask = getMask();
    Mat omask = other.getMask();
    vector<vector<Point>> contours;
    vector<Vec4i> hierarchy;
    size_t size = innerBounds.size();
    int root = 0;
    int index = 0;

    mask = (mask & omask) ^ mask; // remove common area from this
    contour.clear(); // free old contour
    findContours(mask, contours, hierarchy, RETR_TREE, CHAIN_APPROX_SIMPLE);

    /*const char* test = "test2";
    namedWindow(test, WINDOW_AUTOSIZE);
    imshow(test, mask);*/

   /* mask.release();
    omask.release();
    root = getRoot(contours, hierarchy);
    contour = contours[root]; // new contour
    contour.shrink_to_fit(); // free unused memory

    cout << contour.size() << endl;

    //for (int i = 0; i < size; i++)
    //    innerBounds[i].clear();
    //innerBounds.clear(); // clear old inner children

    //index = getBeginChildren(hierarchy, root);
    //while (index != -1) {
    //    innerBounds.push_back(contours[index]); // push new inner children
    //    index = hierarchy[index][NEXT_HIERARCHY];
    //}
    //innerBounds.shrink_to_fit(); // free unused memory
}*/

Area::Area(Mat& img)
    : img(img)
{
}

Area::~Area()
{
}

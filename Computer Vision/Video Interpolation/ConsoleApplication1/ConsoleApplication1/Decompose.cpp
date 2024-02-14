#include "Decompose.h"

vector<Contour> decompose(Mat& imgHSV, const Vec3b& color)
{
    Mat imgFiltered;
    vector<Contour> contours;

    int pH = ((int)color[0]);
    int pS = ((int)color[1]);
    int pV = ((int)color[2]);
    inRange(imgHSV,
        Scalar(pH - H_HALF_RECT, pS - S_HALF_RECT, pV - V_HALF_RECT),
        Scalar(pH + H_HALF_RECT, pS + S_HALF_RECT, pV + V_HALF_RECT),
        imgFiltered);
    imgHSV.release();
    findContours(imgFiltered, contours, RETR_TREE, CHAIN_APPROX_NONE);

    return contours;
}

//TODO: Use recursion to get all areas from the hierarchy of the same color
// Write a function
void decomposeHelper(int root, Mat& img, vector<Area>& areas, vector<Contour>& contours, vector<Vec4i>& hierarchy, int min_size)
{
    //std::cout << "Root: " << root << "\n";
    stack<int> s;
    unordered_set<int> inners;

    s.push(root);

    while (!s.empty())
    {
        int n = s.top();
        s.pop();


        if (n < 0)
        {
            continue;
        }
        if (inners.find(n) == inners.end())
        {
            //s.push(rootHelper(hierarchy[n][CHILD_HIERARCHY], contours, hierarchy));
            //decomposeHelper(hierarchy[root][CHILD_HIERARCHY], img, areas, inners, contours, hierarchy);
            //continue;

            //Utils::drawContour(contours[n], img.size(), "Area");
            Rect r = Utils::rectanglefy(contours[n]);
            if (r.width >= min_size || r.height >= min_size) {

                Area area(img);
                area.contour = contours[n];
                vector<int> innerBounds = area.getInnerBounds(n, contours, hierarchy);
                for (int in : innerBounds)
                {
                    inners.insert(in);
                }
                areas.push_back(area);
            }
        }


        s.push(Utils::rootHelper(hierarchy[n][CHILD_HIERARCHY], contours, hierarchy));
        s.push(hierarchy[n][NEXT_HIERARCHY]);

    }
}

vector<Area> decompose2(Mat& img, Mat& imgHSV, const Vec3b& color, const Vec3b& bounds, Mat& imgFiltered, vector<Contour>& contours, int min_size)
{
    // Mat imgFiltered;
    vector<Vec4i> hierarchy;
    vector<Area> areas;

    int pH = ((int)color[0]);
    int pS = ((int)color[1]);
    int pV = ((int)color[2]);
    inRange(imgHSV,
        Scalar(pH - bounds[0], pS - bounds[1], pV - bounds[2]),
        Scalar(pH + bounds[0] - 1, pS + bounds[1], pV + bounds[2]),
        imgFiltered);
    findContours(imgFiltered, contours, hierarchy, RETR_TREE, CHAIN_APPROX_NONE);

    if (contours.size() == 0)
        return areas;

    int root = Area::getRoot(contours, hierarchy);
    decomposeHelper(root, img, areas, contours, hierarchy, min_size);

    return areas;
}

/// TODO: create decomposeAll function to decompose an image by all colors in areas

/// Split S and V values

vector<vector<Area>> decomposeAll(vector<vector<Area>>& areasByColor, vector<vector<Contour>>& contoursByColor, Mat& img, Mat& imgHSV, const bool largeImage)
{
    Mat imgFiltered;

    vector<Contour> contours;
    int min_size = (largeImage) ? MIN_SIZE : 0;

    for (int H = H_HALF_RECT; H < 256; H += 2 * H_HALF_RECT)
    {

        //cout << H << "\n";
        //decompose2(img, imgHSV, Vec3b(H, 255 - S_HALF_RECT, V_HALF_RECT), Vec3b(H_HALF_RECT, S_HALF_RECT, V_HALF_RECT));

        imgFiltered = Mat::zeros(img.size(), CV_8UC1);
        contours.clear();
        areasByColor.emplace_back(decompose2(img, imgHSV, Vec3b(H, 255 - S_HALF_RECT, 128), Vec3b(H_HALF_RECT, S_HALF_RECT, 128), imgFiltered, contours, min_size));
        contoursByColor.push_back(contours);

        //imgFiltered = Mat::zeros(img.size(), CV_8UC1);
        //contours.clear();
        //decompose2(img, imgHSV, Vec3b(H, 255 - S_HALF_RECT, V_HALF_RECT1 + 2 * V_HALF_RECT), Vec3b(H_HALF_RECT, S_HALF_RECT, V_HALF_RECT1));
        //areasByColor.emplace_back(decompose2(img, imgHSV, Vec3b(H, 255 - S_HALF_RECT, V_HALF_RECT1 + 2 * V_HALF_RECT), Vec3b(H_HALF_RECT, S_HALF_RECT, V_HALF_RECT1), imgFiltered, contours));
        //contoursByColor.push_back(contours);
        //decompose2(img, imgHSV, Vec3b(H, S_HALF_RECT1, V_HALF_RECT), Vec3b(H_HALF_RECT, S_HALF_RECT1, V_HALF_RECT));
        //areasByColor.emplace_back(decompose2(img, imgHSV, Vec3b(H, S_HALF_RECT1, V_HALF_RECT), Vec3b(H_HALF_RECT, S_HALF_RECT1, V_HALF_RECT)));

        //decompose2(img, imgHSV, Vec3b(H, S_HALF_RECT1, V_HALF_RECT1 + 2 * V_HALF_RECT), Vec3b(H_HALF_RECT, S_HALF_RECT1, V_HALF_RECT));
        //areasByColor.emplace_back(decompose2(img, imgHSV, Vec3b(H, S_HALF_RECT1, V_HALF_RECT1 + 2 * V_HALF_RECT), Vec3b(H_HALF_RECT, S_HALF_RECT1, V_HALF_RECT)));
    }

    imgFiltered = Mat::zeros(img.size(), CV_8UC1);
    contours.clear();
    areasByColor.emplace_back(decompose2(img, imgHSV, Vec3b(128, S_HALF_RECT1, V_HALF_RECT), Vec3b(128, S_HALF_RECT1, V_HALF_RECT), imgFiltered, contours, MIN_SIZE));
    contoursByColor.push_back(contours);
    imgFiltered = Mat::zeros(img.size(), CV_8UC1);
    areasByColor.emplace_back(decompose2(img, imgHSV, Vec3b(128, S_HALF_RECT1, V_HALF_RECT1 + 2 * V_HALF_RECT), Vec3b(128, S_HALF_RECT1, V_HALF_RECT), imgFiltered, contours, MIN_SIZE));
    contoursByColor.push_back(contours);

    std::cout << "Done!" << endl;

    return areasByColor;
}

vector<vector<Area>> organize(const Mat& img, const vector<Area>& A, const int K)
{
    int cols = ceil((double)img.cols / K);
    vector<vector<Area>> cat(ceil((double)cols * (double)(img.rows + 2 * K) / K));
    for (int i = 0; i < A.size(); i++) {
        Rect r = Utils::rectanglefy(A[i].contour);
        //cout << floor((double)r.y / K) * cols + floor((double)r.x / K) << " " << cols * img.rows / K << endl;
        cat[floor((double)r.y / K) * cols + floor((double)r.x / K)].push_back(A[i]);
    }
    return cat;
}

vector<Area> restAreas(Mat& img, const Mat& it)
{
    Mat imgFiltered = ~it;
    vector<Contour> contours;

    vector<Vec4i> hierarchy;
    vector<Area> areas;

    findContours(imgFiltered, contours, hierarchy, RETR_TREE, CHAIN_APPROX_NONE);

    if (contours.size() == 0)
        return areas;

    int root = Area::getRoot(contours, hierarchy);
    decomposeHelper(root, img, areas, contours, hierarchy, 0);

    return areas;
}


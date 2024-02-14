#include <iostream>

#include "Utils.h"
#include "Area.h"
#include "Decompose.h"
#include "Identify.h"
#include "Interpolate.h"

using namespace std::chrono;

double distance2(Point p1, Point p2) {
    return (p1.y - p2.y) * (p1.y - p2.y)
        + (p1.x - p2.x) * (p1.x - p2.x);
}

double distance2(vector<Point>& p1, vector<Point>& p2) {
    return distance2(p1[0], p2[0]);
}

bool cmp(Area a, Area b)
{
    Rect r1 = Utils::rectanglefy(a.contour);
    Rect r2 = Utils::rectanglefy(b.contour);

    int a1 = r1.width * r1.height;
    int a2 = r2.width * r2.height;

    return a1 > a2;
}

int main() {
    string sample1 = "C:\\Users\\Cristi\\Desktop\\working\\VideoInterpolation\\test1.jpg";
    string sample2 = "C:\\Users\\Cristi\\Desktop\\working\\VideoInterpolation\\sample.png";

    Mat imgA = imread(sample2);
    bool largeImage = false;
    //Mat imgB = imread("C:\\Users\\craci\\Desktop\\VideoInterpolation\\test3.png");
    Mat imgHSVA;
    
    cvtColor(imgA, imgHSVA, COLOR_BGR2HSV);

    Mat it = Mat::zeros(imgA.size(), CV_8UC1);
	Mat zeros = Mat::zeros(imgA.size(), CV_8UC1);
    Mat mask = Mat::zeros(imgA.size(), CV_8UC1);

    Mat out;
    Mat blackA;

	vector<vector<Contour>> contoursAll;
    Mat imgFiltered = Mat::zeros(imgA.size(), CV_8UC1);


	vector<vector<Area>> areasAll(256 / H_HALF_RECT + 2);
    decomposeAll(areasAll, contoursAll, imgA, imgHSVA, largeImage);

    //cout << "Debug\n";
    const char* nameA = "testA";
    cvtColor(it, out, COLOR_GRAY2BGR);

    auto start = high_resolution_clock::now();
    for (auto& areas : areasAll)
    {
        vector<int> pairsA_B = identify(imgA, areas, areas, 0, largeImage);
        for (auto& area : areas)
        {
            //cout << area.innerBounds.size() << endl;
            area.getMask(mask);
            it |= mask;
            mask = Mat::zeros(imgA.size(), CV_8UC1);
        }
        /*cout << areas.size() << endl;
        cvtColor(it, out, COLOR_GRAY2BGR);
        const char* nameA = "testA";
        namedWindow(nameA, WINDOW_AUTOSIZE);
        imshow(nameA, out & imgA);
        waitKey(0);*/
        //i++;
    }

    vector<Area> rest = restAreas(imgA, it);
    vector<int> pairsRest = identify(imgA, rest, rest, 0, largeImage);
    for (auto& area : rest) {
        area.getMask(mask);
        it |= mask;
        mask = Mat::zeros(imgA.size(), CV_8UC1);
    }

	auto stop = high_resolution_clock::now();
	auto duration = duration_cast<seconds>(stop - start);
	cout << duration.count() << endl;
    //cvtColor(it, out, COLOR_GRAY2BGR);
    Mat finalImg;
    cvtColor(it, finalImg, COLOR_GRAY2BGR);
    namedWindow(nameA, WINDOW_FULLSCREEN);
    imshow(nameA, finalImg & imgA);
    waitKey(0);
    //cvtColor(it, mask, COLOR_GRAY2BGR);
    /*const char* nameA = "testA";
    namedWindow(nameA, WINDOW_AUTOSIZE);
    imshow(nameA, it);
   // waitKey(0);*/


    //cvtColor(imgB, imgHSVB, COLOR_BGR2HSV);
    /*Vec3b pix = imgHSVA.at<Vec3b>(500, 200);
    cout << pix << "\n";
    vector<Area> areasA = Utils::decompose2(imgA, imgHSVA, pix);
    Mat mask = Mat::zeros(imgA.size(), CV_8UC1);

    for (Area& area : areasA)
    {
        mask = (area.getMask() | mask);
    }
    cvtColor(mask, mask, COLOR_GRAY2BGR);
    mask = mask & imgA;
    const char* nameA = "testA";
    namedWindow(nameA, WINDOW_AUTOSIZE);
    imshow(nameA, mask);
    waitKey(0);
    //vector<Area> areasB = Utils::decompose2(imgB, imgHSVB, pix);

    //std::cout << "SizeA: " << areasA.size() << "  SizeB: " << areasB.size() << "\n";

    //vector<int> pairsA_B = Utils::identify(areasA, areasB);

   /* for (int i = 0; i < pairsA_B.size(); i++)
    {
        if (pairsA_B[i] == -1)
            continue;
        const char* nameA = "testA";
        namedWindow(nameA, WINDOW_AUTOSIZE);
        imshow(nameA, areasA[i].getMask());

        const char* nameB = "testB";
        namedWindow(nameB, WINDOW_AUTOSIZE);
        imshow(nameB, areasB[pairsA_B[i]].getMask());
        waitKey(0);
    }

    /*std::cout << "Size: " << areas.size() << "\n";
    Mat mask = Mat::zeros(img.size(), CV_8UC1);

    for (int i = 0; i < areas.size(); i++)
    {
        mask = mask | areas[i].getMask();
    }
    const char* name = "test";
    namedWindow(name, WINDOW_AUTOSIZE);
    imshow(name, mask);
    waitKey(0);
    /*Area area(img);
    area.identifyContour(Point(120, 500));
    Mat mask = area.getMask();

    Rect rect = Utils::rectanglefy(area.contour);
    cout << rect << endl;


    /*const char* name = "test";
    namedWindow(name, WINDOW_AUTOSIZE);
    imshow(name, mask);

    rectangle(mask, rect, Scalar(255, 0, 0));

    const char* test = "test1";
    namedWindow(test, WINDOW_AUTOSIZE);
    imshow(test, mask);
    waitKey(0);*/
    //cin.get();
    return 0;
}
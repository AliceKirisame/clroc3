#pragma once
#include <core.hpp>
#include <opencv2\imgproc\imgproc.hpp>
#include <opencv2\highgui\highgui.hpp>

#include <iostream>
#include <vector>

using namespace cv;
using namespace std;

class ContoursFinder
{
public:
	ContoursFinder();
	~ContoursFinder();

	void findRect(Mat & src, std::vector<Rect>& rects, float rmin = 1.8, float rmax = 5.5, int minArea = 900);

	void findRect(std::vector<Mat> &src, std::vector<Rect>& rects, float rmin = 1.8, float rmax = 5.5);
	void findRect(std::vector<Mat> &src, std::vector<vector<Rect>>& rects, float rmin = 1.8, float rmax = 5.5);
	
	void countDist(Point pt, Rect rc, double a, double b, std::vector<double> &dists);

private:
	vector<vector<Point>> m_contours;
	vector<Vec4i> m_hie;
	vector<int> m_rectContoursNum;

	vector<vector<Point>> g_hull;
	vector<vector<Point>> contours_poly;
	vector<Rect> boundRect;
	vector<Point2f> center;
	vector<float> radius;
	vector<RotatedRect> minRect;

	const double ELLIPSE_A_DIV, ELLIPSE_B_DIV, MIN_PLATE_AREA;
};


#pragma once

#include <iostream>

#include <core.hpp>
#include <opencv2\imgproc\imgproc.hpp>
#include <opencv2\highgui\highgui.hpp>

using namespace std;
using namespace cv;

class ImageImfor
{
public:
	ImageImfor();
	~ImageImfor();

	void init(Mat &src);
	
	Mat blurM, gradX, gradY, absX,absY, threshXM, threshXTZ, threshYM, threshYTZ, cannyM, cannyMX;
};


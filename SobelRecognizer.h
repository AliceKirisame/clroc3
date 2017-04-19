#pragma once
#include <core.hpp>
#include <opencv2\imgproc\imgproc.hpp>
#include <opencv2\highgui\highgui.hpp>

#include <iostream>
#include <sstream>

using namespace std;
using namespace cv;

class SobelRecognizer
{
public:
	SobelRecognizer() {}
	~SobelRecognizer() {};

	void SobelPlate(const Mat& src, Mat& dst, bool showProcess = false, int core_size = -1);

	Mat getMorgh() { return morph; }
	Mat getThresh() { return threshM; }
	Mat getGradX() { return gradX; }
	Mat getGradY() { return gradY; }

private:
	Mat blurM, gradX, gradY, absX, morph, threshM;
};
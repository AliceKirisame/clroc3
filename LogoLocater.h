#pragma once
#include <core.hpp>
#include <opencv2\imgproc\imgproc.hpp>
#include <opencv2\highgui\highgui.hpp>

#include <iostream>
#include <vector>
#include <io.h>
#include <stdlib.h>

#include "ImageImfor.h"
#include "ContoursFinder.h"

using namespace cv;
using namespace std;

class LogoLocater
{
public:
	LogoLocater(bool debug = false);
	~LogoLocater();

	vector<Rect> logoLocate(const ImageImfor& ii, const Rect& plateRc);

	void mapXY(Mat & src, vector<int>& X, vector<int>& Y);

private:
	bool m_bShowImfor;
};
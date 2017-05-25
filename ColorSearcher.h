#pragma once
#include <core.hpp>
#include <opencv2\imgproc\imgproc.hpp>
#include <opencv2\highgui\highgui.hpp>

#include <iostream>
#include <sstream>
#include <vector>

using namespace cv;
using namespace std;

enum MODE {USE_HSV, USE_RGB};

class ColorSearcher
{
public:
	ColorSearcher();
	ColorSearcher(int color, int hup = 20, int hdown = 20, int smin = 90, int vmin = 90, int smax = 255, int vmax = 255);
	~ColorSearcher();

	void setColor(int color) { m_color = color; }
	void searchColor(const Mat& src, Mat& colorArea, bool showProcess = false);

	static const int BLUE = 120;

private:
	enum MODE m_mode;

	int m_color;

	int m_iHUpDiff;
	int m_iHDownDiff;

	int m_iSMax;
	int m_iSMin;

	
	int m_iVMax;
	int m_iVMin;

	static const int DEF_H_UP_DIFF = 20;
	static const int DEF_H_DOWN_DIFF = 20;

	static const int DEF_S_MAX = 255;
	static const int DEF_S_MIN = 55;

	static const int DEF_V_MAX = 255;
	static const int DEF_V_MIN = 120;
};
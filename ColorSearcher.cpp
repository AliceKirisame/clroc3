#include "ColorSearcher.h"

/*
ColorSearcher::ColorSearcher() :m_iHDownDiff(DEF_H_DOWN_DIFF),
m_iHUpDiff(DEF_H_UP_DIFF),
m_iSMax(DEF_S_MAX),
m_iSMin(DEF_S_MIN),
m_iVMax(DEF_V_MAX),
m_iVMin(DEF_V_MIN),
m_color(BLUE)
{

}
*/
//不能初始化？？

ColorSearcher::ColorSearcher()
{
	m_iHDownDiff = DEF_H_DOWN_DIFF;
	m_iHUpDiff = DEF_H_UP_DIFF;
	m_iSMax = DEF_S_MAX;
	m_iSMin = DEF_S_MIN; 
	m_iVMax = DEF_V_MAX;
	m_iVMin = DEF_V_MIN;
	m_color = BLUE;
}

ColorSearcher::ColorSearcher(int color, int hup, int hdown, int smin, int vmin, int smax, int vmax) :m_iHDownDiff(DEF_H_DOWN_DIFF),
																								m_iHUpDiff(DEF_H_UP_DIFF),
																								m_iSMax(DEF_S_MAX),
																								m_iSMin(DEF_S_MIN),
																								m_iVMax(DEF_V_MAX),
																								m_iVMin(DEF_V_MIN),
																								m_color(color)
{
}



ColorSearcher::~ColorSearcher()
{
}

void ColorSearcher::searchColor(const Mat & src, Mat & colorArea, bool showProcess)
{
	Mat hsv;
	int core_size;

	if (src.cols < 500)
	{
		core_size = 20;
	}
	else if (src.cols >= 500 && src.cols < 1000)
	{
		core_size = 45;
	}
	else if (src.cols >= 1000 && src.cols < 1500)
	{
		core_size = 60;
	}
	else
	{
		core_size = 80;
	}

	cvtColor(src, hsv, COLOR_BGR2HSV);

	colorArea = Mat::zeros(src.rows, src.cols, CV_8UC1);

	int rowN = hsv.rows;
	int colN = hsv.cols * hsv.channels();

	for (int i = 0; i < rowN; i++)
	{
		uchar *data = hsv.ptr<uchar>(i);
		uchar *result = colorArea.ptr<uchar>(i);
		
		int h, s, v;

		for (int j = 0,k=0; j < colN; j+=3,k++)
		{
			h = data[j];
			s = data[j + 1];
			v = data[j + 2];

			if (h <= m_color + m_iHUpDiff && h >= m_color - m_iHDownDiff)
			{
				if (s <= m_iSMax && s >= m_iSMin && v <= m_iVMax && v >= m_iVMin)
				{
					result[k] = 255;
				}
				else
				{
					result[k] = 0;
				}
			}
		}
	}

	if (showProcess)
		imshow("color1",colorArea);

	morphologyEx(colorArea, colorArea, MORPH_CLOSE, getStructuringElement(MORPH_RECT, Size(core_size, core_size/10)));

	erode(colorArea, colorArea, getStructuringElement(MORPH_RECT, Size(1, core_size/5)));
	dilate(colorArea, colorArea, getStructuringElement(MORPH_RECT, Size(1, core_size/5)));

	//morphologyEx(colorArea, colorArea, MORPH_CLOSE, getStructuringElement(MORPH_RECT, Size(1, core_size+2/2)));

	if (showProcess)
		imshow("color2", colorArea);

	//morphologyEx(colorArea, colorArea, MORPH_CLOSE, getStructuringElement(MORPH_RECT, Size(core_size, 1)));
	//erode(colorArea, colorArea, getStructuringElement(MORPH_RECT, Size(core_size / 2, 1)));
	//dilate(colorArea, colorArea, getStructuringElement(MORPH_RECT, Size(core_size / 2, 1)));

	//morphologyEx(colorArea, colorArea, MORPH_CLOSE, getStructuringElement(MORPH_RECT, Size(core_size, 1)));
}

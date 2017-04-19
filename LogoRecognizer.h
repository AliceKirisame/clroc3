#pragma once

#include <iostream>
#include <sstream>

#include <opencv2\opencv.hpp>
#include <opencv2\xfeatures2d\nonfree.hpp>

#include "ImageReader.h"
#include "Paths.h"

using namespace cv;

struct matchedInfo
{
	int total;
	int matched;
	int matched1;
	int matched2;
	int matched3;

	double mRate;
	double m1Rate;
	double m2Rate;
	double m3Rate;

	double score;

	int num;

	matchedInfo()
	{
		total = 0;
		matched = 0;
		matched1 = 0;
		matched2 = 0;
		matched3 = 0;

		mRate = 0;
		m1Rate = 0;
		m2Rate = 0;
		m3Rate = 0;
	}
};

class LogoRecognizer
{
public:
	LogoRecognizer(bool debug = false);
	~LogoRecognizer();

	void matchLogo(const Mat & src);

	inline double getSignificanceScore(){return m_dSignificanceScore; }
	inline int getFittestNum() { return m_iFittestNum; }

private:
	matchedInfo mI[3];
	int m_iFittestNum;
	int m_iSecondFittestNum;

	double m_dBestScore;
	double m_dSecondBestScore;

	double m_dSignificanceScore;

	int m_iLogoCategories;
	ImageReader m_ir;
	static const int LOGO_CATEGORIES = 3;

	bool m_bShowImfor;
};


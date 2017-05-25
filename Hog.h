#pragma once

#include <iostream>
#include <fstream>
#include <opencv.hpp>
#include <highgui.h>
#include <cxcore.h>
#include <cstring>
#include <ml.h>
#include <vector>
#include <iomanip>

#include "ImageReader.h"

using namespace std;
using namespace cv;


class Hog
{
public:
	Hog();
	~Hog();

	void Init(int nd, int nc, String path);
	Mat GetTrainDate() { return m_matTrainData; }
	Mat GetTrainLabel() { return m_matTrainLabel; }
	Mat GetSvmLabel() { return m_matSvmLabel; }
	int GetNumOfSamples() { return m_iNumSamples; }
	void Compute(Mat img, Mat & hogMat);

private:
	int m_iNumDimension;
	int m_iNumSamples;
	int m_iNumClasses;

	ImageReader m_ir;

	Mat m_matTrainData;
	Mat m_matTrainLabel;
	Mat m_matSvmLabel;

	vector<Mat> *m_vecTempMats;
	HOGDescriptor * m_pHog;
};
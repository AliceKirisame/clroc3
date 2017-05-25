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

using namespace std;
using namespace cv;
using namespace ml;

class Svm
{
public:
	Svm();
	~Svm();

	void Train(Mat trainingDataMat, Mat labelsMat);
	float Predict(Mat sampleMat);

private:
	Ptr<SVM> m_svm;
};


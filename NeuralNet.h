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

class NeuralNet
{
public:
	NeuralNet();
	~NeuralNet();

	void Create(int in, int on, int hn);
	void Train(Mat trainData, Mat labelData);
	void Predict(Mat sampleMat, Mat &responseMat);

private:
	Ptr<ANN_MLP> m_bp ;
	int m_iNumInputs;
	int m_iNumNeuronsPerLayer;
	int m_iNumOuputs;
};


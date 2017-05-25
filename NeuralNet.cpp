#include "NeuralNet.h"



NeuralNet::NeuralNet()
{
	
}


NeuralNet::~NeuralNet()
{
}

void NeuralNet::Create(int in, int on, int hn)
{
	m_iNumInputs = in;
	m_iNumNeuronsPerLayer = hn;
	m_iNumOuputs = on;

	Mat layerSizes = (Mat_<int>(1, 5) << in, 1000, 500, hn, on);

	m_bp = ANN_MLP::create();
	m_bp->setLayerSizes(layerSizes);
	m_bp->setActivationFunction(ANN_MLP::SIGMOID_SYM);
	m_bp->setTermCriteria(TermCriteria(TermCriteria::MAX_ITER + TermCriteria::EPS, 20000, 0.00001));
	m_bp->setTrainMethod(ANN_MLP::BACKPROP, 0.1, 0.9);
}

void NeuralNet::Train(Mat trainData, Mat trainLabel)
{
	Ptr<TrainData> tData = TrainData::create(trainData, ROW_SAMPLE, trainLabel);

	m_bp->train(tData);
}

void NeuralNet::Predict(Mat sampleMat, Mat & responseMat)
{
	m_bp->predict(sampleMat, responseMat);
}

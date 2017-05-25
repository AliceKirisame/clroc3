#include "Svm.h"



Svm::Svm()
{
	m_svm = SVM::create();
	m_svm->setType(SVM::C_SVC);
	m_svm->setKernel(SVM::POLY);
	m_svm->setDegree(1.0);
	m_svm->setTermCriteria(TermCriteria(TermCriteria::MAX_ITER, 100, 1e-6));
}


Svm::~Svm()
{
}

void Svm::Train(Mat trainingDataMat, Mat labelsMat)
{
	m_svm->train(trainingDataMat, ROW_SAMPLE, labelsMat);
}

float Svm::Predict(Mat sampleMat)
{
	return m_svm->predict(sampleMat);
}


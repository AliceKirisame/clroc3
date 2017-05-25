#include "Hog.h"



Hog::Hog()
{
	m_pHog = new HOGDescriptor(
		Size(64, 64),
		Size(16, 16),
		Size(8, 8),
		Size(8, 8),
		9
		);
}


Hog::~Hog()
{
	if (m_pHog)
		delete m_pHog;
	m_pHog = NULL;

	if (m_vecTempMats)
		delete[] m_vecTempMats;
	m_vecTempMats = NULL;
}

void Hog::Init(int nd, int nc, String tempPath)
{
	String imgName;
	m_iNumDimension = nd;
	m_iNumClasses = nc;
	m_iNumSamples = 0;

	m_vecTempMats = new vector<Mat>[nc];

	

	for (int i = 0; i < m_iNumClasses; ++i)
	{
		stringstream ss;

		ss << i;

		m_ir.init(tempPath + "\\" + ss.str());

		while (m_ir.next(imgName))
		{
			Mat img = imread(tempPath + "\\" + ss.str() + "\\" + imgName);

			if (img.channels() != 1)
			{
				cvtColor(img, img, COLOR_BGR2GRAY);
			}


			GaussianBlur(img, img, Size(3, 3), 0, 0);
			//medianBlur(img, img, 3);
			resize(img, img, Size(64, 64));

			m_vecTempMats[i].push_back(img);
			m_iNumSamples++;
		}
	}

	m_matTrainData = Mat::zeros(m_iNumSamples, m_iNumDimension, CV_32FC1);
	m_matTrainLabel = Mat::zeros(m_iNumSamples, m_iNumClasses, CV_32FC1);
	m_matSvmLabel = Mat::zeros(m_iNumSamples, 1, CV_32SC1);

	int iCountor = 0;
	
	for (int i = 0; i < m_iNumClasses; i++)
	{
		vector<Mat>::iterator it = m_vecTempMats[i].begin();

		while (it != m_vecTempMats[i].end())
		{
			vector<float> descriptors;
			m_pHog->compute(*it, descriptors, Size(64,64), Size(0,0));
			
			for (int j = 0; j < descriptors.size(); j++)
			{
				m_matTrainData.at<float>(iCountor, j) = descriptors[j];
			}
			m_matTrainLabel.at<float>(iCountor, i) = 1.0;
			m_matSvmLabel.at<int>(iCountor, 0) = i ;
			++iCountor;
			++it;
		}
	}
	cout << m_matSvmLabel;
}

void Hog::Compute(Mat img, Mat & hogMat)
{
	vector<float> descriptors;

	if (img.channels() != 1)
	{
		cvtColor(img, img, COLOR_BGR2GRAY);
	}

	resize(img, img, Size(64, 64));

	m_pHog->compute(img, descriptors, Size(64, 64), Size(0, 0));

	hogMat.create(1, descriptors.size(), CV_32FC1);

	for (int i = 0; i < descriptors.size(); ++i)
	{
		hogMat.at<float>(0, i) = descriptors[i];
	}
}

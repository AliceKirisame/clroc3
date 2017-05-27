#include "LogoLocater.h"



LogoLocater::LogoLocater(bool debug)
{
	m_bShowImfor = debug;
}


LogoLocater::~LogoLocater()
{
}


vector<Rect> LogoLocater::logoLocate(const ImageImfor & ii, const Rect & plateRc)
{
	stringstream ss;
	stringstream ss1;
	Rect rc;

	rc.x = plateRc.x;
	rc.width = plateRc.width;

	rc.height = plateRc.height * 3.5;
	rc.y = plateRc.y - rc.height;

	if (rc.y < 0)
	{
		rc.height += rc.y;
		rc.y = 0;

		if (rc.height < plateRc.height)
		{
			return vector<Rect>();
		}
	}

	vector<int> xX;
	vector<int> xY;

	Mat imgROI = ii.threshXM(rc);

	mapXY(imgROI, xX, xY);

	imshow("x", imgROI);
	
	cvRound(1);
	vector<int> yX;
	vector<int> yY;

	imgROI = ii.threshYM(rc);

	mapXY(imgROI, yX, yY);

	imshow("y", imgROI);

	vector<int> usedVec;
	bool usedFlag;
	
	(xY[xY.size() - 1] < yX[yX.size() - 1]) ? (usedVec = xY, imgROI = ii.cannyMX(rc),usedFlag=true) : (usedVec = yX, usedFlag = false);

	static int c = 0;

	ss1 << ".\\locateProcess\\canny sobel before\\";
	ss1 << c << " m";

	imwrite(ss1.str() + ii.name, imgROI);

	for (int i = 0; i < imgROI.rows; i++)
	{
		for (int j = 0; j < imgROI.cols; j++)
		{
			if (imgROI.at<uchar>(i, j) != 0)
			{
				int nearCount = 0;

				for (int k = -2; k < 3; k++)
				{
					for (int n = -2; n < 3; n++)
					{
						if (i + k >= 0 && i + k < imgROI.rows && j + n >= 0 && j + n < imgROI.cols)
						{
							nearCount += int(imgROI.at<uchar>(i + k, j + n) > 30);
						}
					}
				}

				if (nearCount <= 4)
				{
					imgROI.at<uchar>(i, j) = 0;
				}
			}
		}
	}

	ss1 << "c";
	imwrite(ss1.str() + ii.name, imgROI);
	
	if (usedFlag)
	{
		morphologyEx(imgROI, imgROI, MORPH_CLOSE, getStructuringElement(MORPH_RECT, Size(15, 1)));

		erode(imgROI, imgROI, getStructuringElement(MORPH_RECT, Size(1, 3)));
		dilate(imgROI, imgROI, getStructuringElement(MORPH_RECT, Size(1, 3)));
		
		morphologyEx(imgROI, imgROI, MORPH_CLOSE, getStructuringElement(MORPH_RECT, Size(10, 1))); 
		
		erode(imgROI, imgROI, getStructuringElement(MORPH_RECT, Size(1, 5)));
		dilate(imgROI, imgROI, getStructuringElement(MORPH_RECT, Size(1, 5)));

		//morphologyEx(imgROI, imgROI, MORPH_CLOSE, getStructuringElement(MORPH_RECT, Size(1, 5)));
	}
	else
	{
		morphologyEx(imgROI, imgROI, MORPH_CLOSE, getStructuringElement(MORPH_RECT, Size(15, 1)));

		erode(imgROI, imgROI, getStructuringElement(MORPH_RECT, Size(3, 1)));
		dilate(imgROI, imgROI, getStructuringElement(MORPH_RECT, Size(3, 1)));

		morphologyEx(imgROI, imgROI, MORPH_CLOSE, getStructuringElement(MORPH_RECT, Size(10, 1)));

		erode(imgROI, imgROI, getStructuringElement(MORPH_RECT, Size(5, 1)));
		dilate(imgROI, imgROI, getStructuringElement(MORPH_RECT, Size(5, 1)));
	}

	ss << ".\\locateProcess\\close\\";
	ss << c << " " << usedFlag << " ";
	ss << ii.name;

	c++;

	imwrite(ss.str(), imgROI);

	ContoursFinder cf;
	vector<Rect> logos;

	Mat tmpRoi;

	imgROI.copyTo(tmpRoi);

	cf.findRect(tmpRoi, logos, 0.5, 7, 100);

	for (int i = 0 ; i < logos.size() ; ++i)
	{
		logos[i].x += rc.x;
		logos[i].y += rc.y;

		logos[i].x -= 4;
		if (logos[i].x < 0)
			logos[i].x = 0;

		logos[i].y -= 4;
		if (logos[i].y < 0)
			logos[i].y = 0;

		logos[i].width += 6;
		if (logos[i].width > ii.blurM.size().width)
			logos[i].y = ii.blurM.size().width;

		logos[i].height += 6;
		if (logos[i].height > ii.blurM.size().height)
			logos[i].y = ii.blurM.size().height;
	}

	imshow("close", imgROI);
	//waitKey();

	int s=0, e=0, nSum=0, tSum=0;
	int ns = 0;

	//int avg = direPtr[direPtr.size() - 1] / direPtr.size() - 1;
	/*
	for (int i = 0; i < direPtr.size(); ++i)
	{
		direPtr[i] -= avg;

		nSum += direPtr[i];

		if (nSum > tSum)
		{
			tSum = nSum;
			s = ns;
			e = i;
		}
		if (nSum <= 0)
		{
			s = i + 1;
			nSum = 0;
		}
	}
	*/

	return logos;
}

void LogoLocater::mapXY(Mat & src, vector<int> &X, vector<int> &Y)
{
	//XΪx
	X.clear();
	Y.clear();
	int colN = src.cols*src.channels();       
	int rowN = src.rows;

	int *cX, *cY;
	int countX = 0, countY = 0;
	int totalCount = 0;

	cX = (int *)malloc(sizeof(int) * colN);
	memset(cX, 0, sizeof(int) * colN);

	cY = (int *)malloc(sizeof(int) * rowN);
	memset(cY, 0, sizeof(int) * rowN);

	for (int i = 0; i < rowN; i++)
	{
		uchar *data = src.ptr<uchar>(i);
		for (int j = 0; j < colN; j++)
		{
			if (data[j] > 0)
			{
				totalCount++;
			}

			cX[j] += data[j];
			cY[i] += data[j];
		}
	}
	if(m_bShowImfor)
		printf("Y::");
	for (int i = 0; i < rowN; i++)
	{
		cY[i] /= 50;
		
		Y.push_back(cY[i]);

		if (cY[i] > 0)
		{
			countY++;
		}
		if (m_bShowImfor)
			printf("%d ", cY[i]);
	}
	Y.push_back(countY);
	Y.push_back(totalCount);
	
	if (m_bShowImfor)
		printf("total %d\n", totalCount);
	
	if (m_bShowImfor)
		printf("X::");
	for (int i = 0; i < colN; i++)
	{
		cX[i] /= 50;
		X.push_back(cX[i]);

		if (cX[i] > 0)
		{
			countX++;
		}
		if (m_bShowImfor)
			printf("%d ", cX[i]);
	}
	X.push_back(countX);
	X.push_back(totalCount);
	if (m_bShowImfor)
		printf("total %d\n****************************************\n", totalCount);
}
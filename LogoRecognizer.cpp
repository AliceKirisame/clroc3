#include "LogoRecognizer.h"



LogoRecognizer::LogoRecognizer(bool debug)
{
	m_pvecDescriptors = NULL;
	m_pvecKeyPoints = NULL;

	m_bShowImfor = debug;

	m_iLogoCategories = LOGO_CATEGORIES;
	m_dBestScore = 0;
	m_dSecondBestScore = 0;
	m_iFittestNum = 0;
	m_iSecondFittestNum = 0;

	m_pvecDescriptors = new vector<Mat>[LOGO_CATEGORIES];
	m_pvecKeyPoints = new vector<vector<KeyPoint>>[LOGO_CATEGORIES];

	init();
}


LogoRecognizer::~LogoRecognizer()
{
	if (m_pvecDescriptors != NULL)
	{
		delete[] m_pvecDescriptors;
		m_pvecDescriptors = NULL;
	}

	if (m_pvecKeyPoints != NULL)
	{
		delete[] m_pvecKeyPoints;
		m_pvecKeyPoints = NULL;
	}
}

void LogoRecognizer::reset()
{
	m_iLogoCategories = LOGO_CATEGORIES;
	m_dBestScore = 0;
	m_dSecondBestScore = 0;
	m_iFittestNum = 0;
	m_iSecondFittestNum = 0;

	for (int i = 0; i < LOGO_CATEGORIES; i++)
	{
		m_mI[i].reset();
	}
}

void LogoRecognizer::init()
{
	String imgName;
	Ptr<Feature2D> f2d = xfeatures2d::SIFT::create();
	
	Mat img;
	vector<KeyPoint> keypoints;
	Mat descriptors;
	
	reset();

	for (int i = 0; i < m_iLogoCategories; ++i)
	{
		stringstream ss;

		ss << i;

		m_ir.init(Paths::s_strTemplatePath + "\\" + ss.str());

		while (m_ir.next(imgName))
		{
			img = imread(Paths::s_strTemplatePath + "\\" + ss.str() + "\\" + imgName);

			if (img.channels() != 1)
			{
				cvtColor(img, img, COLOR_BGR2GRAY);
			}
		
			f2d->detect(img, keypoints);
			f2d->compute(img, keypoints, descriptors);

			m_pvecKeyPoints[i].push_back(keypoints);
			m_pvecDescriptors[i].push_back(descriptors);
		}
	}
}

void LogoRecognizer::matchLogo(const Mat & src)
{
	reset();

	String imgName;

	Mat img1 = src;
	Mat img2;
	vector<KeyPoint> keypoints1, keypoints2;
	Ptr<Feature2D> f2d = xfeatures2d::SIFT::create();
	Mat descriptors1, descriptors2;


	if (img1.channels() != 1)
	{
		cvtColor(img1, img1, COLOR_BGR2GRAY);
	}
	f2d->detect(img1, keypoints1);
	f2d->compute(img1, keypoints1, descriptors1);


	for (int i = 0; i < m_iLogoCategories; ++i)
	{

		vector<Mat> vecDescriptors = m_pvecDescriptors[i];
		vector<vector<KeyPoint>> vecKeyPoints = m_pvecKeyPoints[i];

		vector<Mat>::iterator itDescriptor = vecDescriptors.begin();
		vector<vector<KeyPoint>>::iterator itKeyPoints = vecKeyPoints.begin();

		while (itDescriptor != vecDescriptors.end() && itKeyPoints != vecKeyPoints.end())
		{
			descriptors2 = *itDescriptor;
			keypoints2 = *itKeyPoints;

			m_mI[i].total++;
			
			/*
			img2 = imread(Paths::s_strTemplatePath + "\\" + ss.str()+ "\\" + imgName);
			
			if (img2.channels() != 1)
			{
				cvtColor(img2, img2, COLOR_BGR2GRAY);
			}

			imshow("r1", img1);
			imshow("r2", img2);
			*/

			/*
			if (img1.size().width > img2.size().width)
			{
				double dScale = (double)img1.size().width / img2.size().width ;

				resize(img2, img2, Size(img1.size().width, img2.size().height * dScale));
			}
			else
			{
				double dScale = (double)img2.size().width / img1.size().width;

				resize(img1, img1, Size(img2.size().width, img1.size().height * dScale));
			}*/

			//waitKey();

			/*
			double t = getTickCount();                               //当前滴答数  
			
			f2d->detect(img2, keypoints2);
			f2d->compute(img2, keypoints2, descriptors2);

			t = ((double)getTickCount() - t) / getTickFrequency();
			*/

			//cout << "SIFT算法用时：" << t << "秒" << endl;
			//cout << "图像1特征描述矩阵大小：" << descriptors1.size()
			//	<< "，特征向量个数：" << descriptors1.rows << "，维数：" << descriptors1.cols << endl;
			//cout << "图像2特征描述矩阵大小：" << descriptors2.size()
			//	<< "，特征向量个数：" << descriptors2.rows << "，维数：" << descriptors2.cols << endl;

			Mat img_keypoints1, img_keypoints2;
			//drawKeypoints(img1, keypoints1, img_keypoints1, Scalar::all(-1), 0);
			//drawKeypoints(img2, keypoints2, img_keypoints2, Scalar::all(-1), 0);
			//imshow("Src1",img_keypoints1);  
			//imshow("Src2",img_keypoints2);  
			
			BFMatcher matcher;
			vector<DMatch> matches;
			matcher.match(descriptors1, descriptors2, matches);      //匹配两个图像的特征矩阵  
			//cout << "Match个数：" << matches.size() << endl;


														//计算匹配结果中距离的最大和最小值  
														//距离是指两个特征向量间的欧式距离，表明两个特征的差异，值越小表明两个特征点越接近  
			double max_dist = 0;
			double min_dist = 100;
			for (int i = 0; i<matches.size(); i++)
			{
				double dist = matches[i].distance;
				if (dist < min_dist) min_dist = dist;
				if (dist > max_dist) max_dist = dist;
			}
			
			//cout << "最大距离：" << max_dist << endl;
			//cout << "最小距离：" << min_dist << endl;

			
			vector<DMatch> goodMatches;                               //筛选出较好的匹配点  
			for (int i = 0; i<matches.size(); i++)
			{
				if (matches[i].distance < 0.46 * max_dist)
				{
					goodMatches.push_back(matches[i]);
				}
			}
			
			//cout << "goodMatch个数：" << goodMatches.size() << endl;

			if (goodMatches.size() > 0)
			{
				m_mI[i].matched++;

				switch (goodMatches.size())
				{
				case 1:
					m_mI[i].matched1++;
					break;

				case 2:
					m_mI[i].matched2++;
					break;

				default:
					m_mI[i].matched3++;
					break;
				}
			}


			Mat img_matches;
			
			//drawMatches(img1, keypoints1, img2, keypoints2, goodMatches, img_matches,         //红色连接的是匹配的特征点对，绿色是未匹配的特征点  
			//	Scalar::all(-1)/*CV_RGB(255,0,0)*/, CV_RGB(0, 255, 0), Mat(), 2);

			//imshow("MatchSIFT", img_matches);
			//waitKey(0);

			++itDescriptor;
			++itKeyPoints;

		}


		m_mI[i].mRate = (double)m_mI[i].matched / m_mI[i].total;

		if (m_mI[i].matched != 0)
		{
			m_mI[i].m1Rate = (double)m_mI[i].matched1 / m_mI[i].matched;
			m_mI[i].m2Rate = (double)m_mI[i].matched2 / m_mI[i].matched;
			m_mI[i].m3Rate = (double)m_mI[i].matched3 / m_mI[i].matched;
		}

		double scoreRate;

		scoreRate = int(m_mI[i].mRate * 10) / 10.0;

		m_mI[i].score = 7 * m_mI[i].mRate + scoreRate * 0.5 * m_mI[i].m1Rate + scoreRate * 4 * m_mI[i].m2Rate + scoreRate * 3 * m_mI[i].m3Rate;
		m_mI[i].num = i;
		if (m_bShowImfor)
		{
			cout << "mRate " << m_mI[i].mRate;
			cout << "  m1Rate " << m_mI[i].m1Rate;
			cout << "  m2Rate " << m_mI[i].m2Rate;
			cout << "  m3Rate " << m_mI[i].m3Rate << endl;

			cout << "编号" << i << "分数" << m_mI[i].score << endl;
		}

		if (m_mI[i].score > m_dBestScore)
		{
			m_dBestScore = m_mI[i].score;
			m_iFittestNum = i;
		}
		else if (m_mI[i].score > m_dSecondBestScore)
		{
			m_dSecondBestScore = m_mI[i].score;
			m_iSecondFittestNum = i;
		}
	}

	m_dSignificanceScore = m_dBestScore - m_dSecondBestScore;
}

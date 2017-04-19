#include "LogoRecognizer.h"



LogoRecognizer::LogoRecognizer(bool debug)
{
	m_bShowImfor = debug;

	m_iLogoCategories = LOGO_CATEGORIES;
	m_dBestScore = 0;
	m_dSecondBestScore = 0;
	m_iFittestNum = 0;
	m_iSecondFittestNum = 0;
}


LogoRecognizer::~LogoRecognizer()
{
}

void LogoRecognizer::matchLogo(const Mat & src)
{
	
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
		stringstream ss;

		ss << i;

		m_ir.init(Paths::s_strTemplatePath + "\\" + ss.str());

		while (m_ir.next(imgName))
		{
			mI[i].total++;
			
			img2 = imread(Paths::s_strTemplatePath + "\\" + ss.str()+ "\\" + imgName);
			
			if (img2.channels() != 1)
			{
				cvtColor(img2, img2, COLOR_BGR2GRAY);
			}

			imshow("r1", img1);
			imshow("r2", img2);
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

			double t = getTickCount();                               //��ǰ�δ���  
			
			f2d->detect(img2, keypoints2);
			f2d->compute(img2, keypoints2, descriptors2);

			t = ((double)getTickCount() - t) / getTickFrequency();
			
			//cout << "SIFT�㷨��ʱ��" << t << "��" << endl;
			//cout << "ͼ��1�������������С��" << descriptors1.size()
			//	<< "����������������" << descriptors1.rows << "��ά����" << descriptors1.cols << endl;
			//cout << "ͼ��2�������������С��" << descriptors2.size()
			//	<< "����������������" << descriptors2.rows << "��ά����" << descriptors2.cols << endl;

			Mat img_keypoints1, img_keypoints2;
			drawKeypoints(img1, keypoints1, img_keypoints1, Scalar::all(-1), 0);
			drawKeypoints(img2, keypoints2, img_keypoints2, Scalar::all(-1), 0);
			//imshow("Src1",img_keypoints1);  
			//imshow("Src2",img_keypoints2);  
			
			BFMatcher matcher;
			vector<DMatch> matches;
			matcher.match(descriptors1, descriptors2, matches);      //ƥ������ͼ�����������  
			//cout << "Match������" << matches.size() << endl;


														//����ƥ�����о����������Сֵ  
														//������ָ���������������ŷʽ���룬�������������Ĳ��죬ֵԽС��������������Խ�ӽ�  
			double max_dist = 0;
			double min_dist = 100;
			for (int i = 0; i<matches.size(); i++)
			{
				double dist = matches[i].distance;
				if (dist < min_dist) min_dist = dist;
				if (dist > max_dist) max_dist = dist;
			}
			
			//cout << "�����룺" << max_dist << endl;
			//cout << "��С���룺" << min_dist << endl;

			
			vector<DMatch> goodMatches;                               //ɸѡ���Ϻõ�ƥ���  
			for (int i = 0; i<matches.size(); i++)
			{
				if (matches[i].distance < 0.46 * max_dist)
				{
					goodMatches.push_back(matches[i]);
				}
			}
			
			//cout << "goodMatch������" << goodMatches.size() << endl;

			if (goodMatches.size() > 0)
			{
				mI[i].matched++;

				switch (goodMatches.size())
				{
				case 1:
					mI[i].matched1++;
					break;

				case 2:
					mI[i].matched2++;
					break;

				default:
					mI[i].matched3++;
					break;
				}
			}


			Mat img_matches;
			
			drawMatches(img1, keypoints1, img2, keypoints2, goodMatches, img_matches,         //��ɫ���ӵ���ƥ���������ԣ���ɫ��δƥ���������  
				Scalar::all(-1)/*CV_RGB(255,0,0)*/, CV_RGB(0, 255, 0), Mat(), 2);

			imshow("MatchSIFT", img_matches);
			//waitKey(0);
		}


		mI[i].mRate = (double)mI[i].matched / mI[i].total;

		if (mI[i].matched != 0)
		{
			mI[i].m1Rate = (double)mI[i].matched1 / mI[i].matched;
			mI[i].m2Rate = (double)mI[i].matched2 / mI[i].matched;
			mI[i].m3Rate = (double)mI[i].matched3 / mI[i].matched;
		}

		double scoreRate;

		scoreRate = int(mI[i].mRate * 10) / 10.0;

		mI[i].score = 7 * mI[i].mRate + scoreRate * 0.5 * mI[i].m1Rate + scoreRate * 4 * mI[i].m2Rate + scoreRate * 3 * mI[i].m3Rate;
		mI[i].num = i;
		if (m_bShowImfor)
		{
			cout << "mRate " << mI[i].mRate;
			cout << "  m1Rate " << mI[i].m1Rate;
			cout << "  m2Rate " << mI[i].m2Rate;
			cout << "  m3Rate " << mI[i].m3Rate << endl;

			cout << "���" << i << "����" << mI[i].score << endl;
		}

		if (mI[i].score > m_dBestScore)
		{
			m_dBestScore = mI[i].score;
			m_iFittestNum = i;
		}
		else if (mI[i].score > m_dSecondBestScore)
		{
			m_dSecondBestScore = mI[i].score;
			m_iSecondFittestNum = i;
		}
	}

	m_dSignificanceScore = m_dBestScore - m_dSecondBestScore;
}

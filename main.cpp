#include"stdafx.h"



using namespace std;
using namespace cv;

Mat g_srcImg;
Mat g_sobelImg;
Mat g_blueImg;
Mat g_tmpOut;
Mat g_Backup;

string names[LOGO_CATEGORIES] = { "����", "�µ�", "����", "��ľ", "����","�ִ�","�ǳ���" };
int cateCount[LOGO_CATEGORIES] = { 0 };
int cateRight[LOGO_CATEGORIES] = { 0 };


int main()
{
	SobelRecognizer sr;

	ColorSearcher cs;

	ContoursFinder cf;

	ImageReader ir;

	LogoLocater ll;

	ImageImfor ii;

	vector<Mat> src;
	vector<vector<Rect>> rects;
	vector<Rect> logos;

	ir.init(".\\test");

	String imgName;
	String Path = ".\\test";
	String OutPath = ".\\out";
	String TemplatePath = ".\\template2\\1";
	String TemplateOut = ".\\template\\1";
	String LogoOut = ".\\logos";
	String fullPath;
	String outText = ".\\out.txt";

	//LogoRecognizer lr;
	
	
	Hog hog;
	hog.Init(1764, LOGO_CATEGORIES, ".\\template");
	/*
	NeuralNet nn;
	nn.Create(1764, 6, 48);
	nn.Train(hog.GetTrainDate(), hog.GetTrainLabel());
	Mat test = imread(".\\template\\0\\1.jpg", 0);
	Mat hogInput;
	Mat responseOut;
	hog.Compute(test, hogInput);
	nn.Predict(hogInput, responseOut);
	cout << responseOut;
	*/

	Svm svm;

	svm.Train(hog.GetTrainDate(), hog.GetSvmLabel());
	

	fstream outFile;
	outFile.open(outText, ios::out);

	int matched = 0;
	int unknown = 0;
	int c = 1;
	//while (false)
	while (ir.next(imgName))
	{
		bool flags[6] = { false };
		bool flag = false;
		double dScale;
		int fittestNum = 0;
		double bestSignificanScore = 0;

		fullPath = Path + "\\" + imgName;

		cout << imgName << endl;
		outFile << imgName << endl;

		g_srcImg = imread(fullPath);

		g_srcImg.copyTo(g_Backup);

		//cvtColor(g_Backup, g_Backup, COLOR_BGR2GRAY);

		//GaussianBlur(g_Backup, g_Backup, Size(5, 5), 0, 0);
		//medianBlur(g_Backup, g_Backup, 3);

		//equalizeHist(g_Backup, g_Backup);

		Size imSize = g_srcImg.size();

		if (g_srcImg.size().width > 400)
		{
			dScale = g_srcImg.size().width / 400.0;

			resize(g_srcImg, g_srcImg, Size(400, imSize.height * 400 / imSize.width));
		}
		else
		{
			dScale = 0;
		}

		ii.init(g_srcImg, imgName);

		sr.SobelPlate(g_srcImg, g_sobelImg);
		cs.searchColor(g_srcImg, g_blueImg);

		src.clear();

		src.push_back(g_blueImg);
		src.push_back(g_sobelImg);


		cf.findRect(src, rects, 1.8, 5);

		g_srcImg.copyTo(g_tmpOut);


		for (int i = 0; i < rects.size(); i++)
		{
			for (int j = 0; j < rects[i].size(); j++)
			{

				logos = ll.logoLocate(ii, rects[i][j]);

				rectangle(g_tmpOut, rects[i][j], Scalar(0, 0, 255));

				for (int k = 0; k < logos.size(); ++k)
				{

					rectangle(g_tmpOut, logos[k], Scalar(0, 255, 0));

					if (dScale != 0)
					{
						logos[k].x *= dScale;
						if (logos[k].x > g_Backup.size().width)
							logos[k].x = g_Backup.size().width - 1;

						logos[k].y *= dScale;
						if (logos[k].x > g_Backup.size().width)
							logos[k].x = g_Backup.size().height - 1;

						logos[k].width *= dScale;
						if (logos[k].width + logos[k].x > g_Backup.size().width)
							logos[k].width = g_Backup.size().width - logos[k].x;

						logos[k].height *= dScale;
						if (logos[k].height + logos[k].y > g_Backup.size().height)
							logos[k].height = g_Backup.size().height - logos[k].y;
					}

					Mat logoROI = g_Backup(logos[k]);
					stringstream ss;

					ss << c << ".jpg";
					c++;
					imwrite(LogoOut + "\\" + ss.str(), logoROI);


					//lr.matchLogo(logoROI, imgName);

					Mat hogInput;
					float response = 6;
					hog.Compute(logoROI, hogInput);
					response = svm.Predict(hogInput);

					if (response != LOGO_CATEGORIES - 1)
					{
						imwrite(String("svmlogos") + "\\" + ss.str(), logoROI);
						cout << names[int(response)] << endl;
						outFile << "	" << ss.str() << " " << names[int(response)] << endl;

						if (imgName[0] - '0' == response + 1 && !flag)
						{
							flags[int(response)] = true;
							matched++;
							flag = true;
							cateRight[imgName[0] - '0' - 1]++;
						}
					}

					/*
					if (lr.getSignificanceScore() > bestSignificanScore)
					{
						bestSignificanScore = lr.getSignificanceScore();
						fittestNum = lr.getFittestNum();
					}*/
				}
			}
		}
		/*
		cout << "���� " << bestSignificanScore << " ";
		cateCount[imgName[0] - '0' - 1]++;

		if (bestSignificanScore != 0)
		{
			cout << names[fittestNum] << endl;
			outFile << imgName << names[fittestNum] << endl;

			if (imgName[0] - '0' == fittestNum + 1)
			{
				matched++;
				cateRight[imgName[0] - '0' - 1]++;
			}
		}
		else
		{
			cout << "δ֪" << endl;
			outFile << imgName << "δ֪" << endl;
			unknown++;
		}
		*/

		imwrite(OutPath + "\\" + imgName, g_tmpOut);

	}
	
	cout << "�ܹ��� " << ir.getCount() << "��ͼ��" << endl << endl;

	cout << "��ȷʶ�� " << matched << "��ͼ��" << endl;
	cout << "ƥ����" << (double)matched / ir.getCount() * 100 << '%' << endl << endl;
	/*
	cout << "����ʶ�� " << ir.getCount() - unknown - matched << "��ͼ��" << endl;
	cout << "������" << ((double)ir.getCount() - unknown - matched) / ir.getCount() * 100 << '%' << endl << endl;

	cout << "δ��ʶ�� " << unknown << "��ͼ��" << endl;
	cout << "δ֪��" << (double)unknown / ir.getCount() * 100 << '%' << endl;

	for (int i = 0; i < LOGO_CATEGORIES; i++)
	{
		cout << names[i] << " ʶ���ʣ�" << static_cast<double>(cateRight[i]) / cateCount[i] << endl;
	}
	*/
	outFile.close();

	int key;
	bool bBusy = true;

	while (bBusy)
	{
		key = waitKey(0);

		switch (key)
		{
		default:
			break;
		}
	}

	return 0;
}


////////////////////////////////////////////////////////////////////////////////////////

void test()
{
	SobelRecognizer sr;

	ColorSearcher cs;

	ContoursFinder cf;

	ImageReader ir;

	LogoLocater ll;

	ImageImfor ii;

	ImageReader tr;



	vector<Mat> src;
	vector<vector<Rect>> rects;
	vector<Rect> logos;

	ir.init(".\\test");
	tr.init(".\\template\\3");

	String imgName;
	String Path = ".\\test";
	String OutPath = ".\\out";
	String TemplatePath = ".\\template2\\1";
	String TemplateOut = ".\\template\\1";
	String fullPath;

	while (tr.next(imgName))
	{
		fullPath = TemplatePath + "\\" + imgName;

		g_srcImg = imread(fullPath);

		cvtColor(g_srcImg, g_srcImg, COLOR_BGR2GRAY);

		GaussianBlur(g_srcImg, g_srcImg, Size(3, 3), 0, 0);

		imwrite(TemplateOut + "\\" + imgName, g_srcImg);
	}

	Ptr<Feature2D> f2d = xfeatures2d::SIFT::create();
	//����ͼ��  
	Mat img1 = imread("1.jpg");
	Mat img2 = imread("2.jpg");

	//��������  
	double t = getTickCount();//��ǰ�δ���  
	vector<KeyPoint> keypoints1, keypoints2;
	f2d->detect(img1, keypoints1);//���img1�е�SIFT�����㣬�洢��keypoints1��  
	f2d->detect(img2, keypoints2);
	cout << "ͼ��1���������:" << keypoints1.size() << endl;
	cout << "ͼ��2���������:" << keypoints2.size() << endl;

	//����������������������Ӿ��󣬼�������������  
	Mat descriptors1, descriptors2;
	f2d->compute(img1, keypoints1, descriptors1);
	f2d->compute(img2, keypoints2, descriptors2);
	t = ((double)getTickCount() - t) / getTickFrequency();
	cout << "SIFT�㷨��ʱ��" << t << "��" << endl;


	cout << "ͼ��1�������������С��" << descriptors1.size()
		<< "����������������" << descriptors1.rows << "��ά����" << descriptors1.cols << endl;
	cout << "ͼ��2�������������С��" << descriptors2.size()
		<< "����������������" << descriptors2.rows << "��ά����" << descriptors2.cols << endl;

	//����������  
	Mat img_keypoints1, img_keypoints2;
	drawKeypoints(img1, keypoints1, img_keypoints1, Scalar::all(-1), 0);
	drawKeypoints(img2, keypoints2, img_keypoints2, Scalar::all(-1), 0);
	//imshow("Src1",img_keypoints1);  
	//imshow("Src2",img_keypoints2);  
	BFMatcher matcher;
	//����ƥ��  
	vector<DMatch> matches;//ƥ����  
	matcher.match(descriptors1, descriptors2, matches);//ƥ������ͼ�����������  
	cout << "Match������" << matches.size() << endl;

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
	cout << "�����룺" << max_dist << endl;
	cout << "��С���룺" << min_dist << endl;

	//ɸѡ���Ϻõ�ƥ���  
	vector<DMatch> goodMatches;
	for (int i = 0; i<matches.size(); i++)
	{
		if (matches[i].distance < 0.31 * max_dist)
		{
			goodMatches.push_back(matches[i]);
		}
	}
	cout << "goodMatch������" << goodMatches.size() << endl;

	//����ƥ����  
	Mat img_matches;
	//��ɫ���ӵ���ƥ���������ԣ���ɫ��δƥ���������  
	drawMatches(img1, keypoints1, img2, keypoints2, matches, img_matches,
		Scalar::all(-1)/*CV_RGB(255,0,0)*/, CV_RGB(0, 255, 0), Mat(), 2);

	imshow("MatchSIFT", img_matches);
	waitKey(0);
}
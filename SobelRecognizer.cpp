#include "SobelRecognizer.h"
#include "ContoursFinder.h"

void SobelRecognizer::SobelPlate(const Mat& src, Mat& dst, bool showProcess, int core_size)
{
	int open_core_size;

	int sizes[6] = { 5,25,43,55,68,80 };
	int o_sizes[6] = { 2,7,10,13,15,20 };
	int size_level = src.cols / 300;


	if (core_size == -1)
	{
		if (src.cols / 300 < 6)
		{
			core_size = sizes[size_level];
			open_core_size = o_sizes[size_level];
		}
		else
		{
			printf("too large");
			return;
		}
		/*
		if (0 < src.cols < 300)
		{
			core_size = 20;
			open_core_size = core_size / 8;
		}
		else if (src.cols >= 800 && src.cols < 1500)
		{
			core_size = 40;
			open_core_size = core_size / 5;
		}
		else if(src.cols >= 1500 && src.cols < 2000)
		{
			core_size = 80;
			open_core_size = core_size / 4;
		}
		else
		{
			printf("too large");
			return;
		}
		*/
	}

	GaussianBlur(src, blurM, Size(7, 7), 0, 0);

	cvtColor(blurM, blurM, COLOR_RGB2GRAY);

	//equalizeHist(blurM, blurM);

	Sobel(blurM, gradX, -1, 1, 0, 3);
	Sobel(blurM, gradY, -1, 0, 1, 3);

	//Sobel(gradX, gradX, -1, 1, 0, 1);
	//Sobel(gradX, gradX, -1, 1, 0, 1);

	convertScaleAbs(gradX, absX);

	//equalizeHist(absX, absX);

	if (showProcess)
		imshow("sobelX", absX);

	threshold(absX, threshM, 50, 255, THRESH_BINARY);

	threshM.copyTo(morph);

	if (showProcess)
		imshow("threshM", threshM);

	erode(morph, morph, getStructuringElement(MORPH_RECT, Size(1, open_core_size)));
	dilate(morph, morph, getStructuringElement(MORPH_RECT, Size(1, open_core_size)));	

	erode(morph, morph, getStructuringElement(MORPH_RECT, Size(1, open_core_size)));
	dilate(morph, morph, getStructuringElement(MORPH_RECT, Size(1, open_core_size)));

	morphologyEx(threshM, morph, MORPH_CLOSE, getStructuringElement(MORPH_RECT, Size(core_size, core_size/10+1)));

	if (showProcess)
		imshow("sobelmorph1", morph);

	//erode(morph, morph, getStructuringElement(MORPH_RECT, Size(open_core_size, 5)));
	//dilate(morph, morph, getStructuringElement(MORPH_RECT, Size(open_core_size, 5)));

	erode(morph, morph, getStructuringElement(MORPH_RECT, Size(1, open_core_size/3+1)));
	dilate(morph, morph, getStructuringElement(MORPH_RECT, Size(1, open_core_size/3+1)));

	morphologyEx(morph, morph, MORPH_CLOSE, getStructuringElement(MORPH_RECT, Size(core_size,1)));

	//erode(morph, morph, getStructuringElement(MORPH_RECT, Size(1, open_core_size)));
	//dilate(morph, morph, getStructuringElement(MORPH_RECT, Size(1, open_core_size)));

	//erode(morph, morph, getStructuringElement(MORPH_RECT, Size(open_core_size*1.5, 1)));
	//dilate(morph, morph, getStructuringElement(MORPH_RECT, Size(open_core_size*1.5, 1)));

	if (showProcess)
		imshow("sobelmorph", morph);

	vector<Rect> candidatePlates;

	vector<Mat> sobelMat;
	ContoursFinder cf;
	Mat tmpMat;

	morph.copyTo(tmpMat);

	sobelMat.push_back(tmpMat);

	cf.findRect(sobelMat, candidatePlates,0,9999);

	for (int i = 0; i < candidatePlates.size(); i++)
	{
		stringstream ss;

		ss << i;

		Mat Roi = threshM(candidatePlates[i]);
		
		if (showProcess)
			imshow(ss.str(), Roi);

		//morphologyEx(Roi, Roi, MORPH_CLOSE, getStructuringElement(MORPH_RECT, Size(1, Roi.size().height / 5)));
		morphologyEx(Roi, Roi, MORPH_CLOSE, getStructuringElement(MORPH_RECT, Size(Roi.size().width/5, 1)));


		//morphologyEx(Roi, Roi, MORPH_OPEN, getStructuringElement(MORPH_RECT, Size(1, Roi.size().height / 2)));

		//erode(Roi, Roi, getStructuringElement(MORPH_RECT, Size(1, Roi.size().height  / 2)));
		//dilate(Roi, Roi, getStructuringElement(MORPH_RECT, Size(1, Roi.size().height / 2)));

		ss << i + 1000;
		if (showProcess)
			imshow(ss.str(), Roi);
	}

	if (showProcess)
		imshow("threshM2", threshM);

	dst = morph;
}
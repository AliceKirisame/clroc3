#include "ImageImfor.h"



ImageImfor::ImageImfor()
{
	blurM = Mat();
	gradX = Mat(); 
	gradY = Mat(); 
	absX = Mat(); 
	threshXM = Mat();
	threshYM = Mat();
	threshXTZ = Mat();
	threshYTZ = Mat();
}


ImageImfor::~ImageImfor()
{
}

void ImageImfor::init(Mat & src)
{
	GaussianBlur(src, blurM, Size(7, 7), 0, 0);

	cvtColor(blurM, blurM, COLOR_RGB2GRAY);

	//equalizeHist(blurM, blurM);

	Canny(blurM, cannyM, 150, 100);

	Sobel(cannyM, cannyMX, -1, 1, 0, 3);

	Sobel(blurM, gradX, -1, 1, 0, 3);
	Sobel(blurM, gradY, -1, 0, 1, 3);

	//Sobel(gradX, gradX, -1, 1, 0, 1);
	//Sobel(gradX, gradX, -1, 1, 0, 1);

	convertScaleAbs(gradX, absX);
	convertScaleAbs(gradY, absY);

	//equalizeHist(absX, absX);

	threshold(absX, threshXM, 50, 255, THRESH_BINARY);
	threshold(absX, threshXTZ, 80, 255, THRESH_TOZERO);

	threshold(absY, threshYM, 50, 255, THRESH_BINARY);
	threshold(absY, threshYTZ, 80, 255, THRESH_TOZERO);
	
}

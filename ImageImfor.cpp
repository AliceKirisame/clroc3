#include "ImageImfor.h"



ImageImfor::ImageImfor()
{
	blurM = Mat();
	gradX = Mat(); 
	gradY = Mat(); 
	absX = Mat(); 
	threshXM = Mat();
	threshYM = Mat();
	threshTZ = Mat();

}


ImageImfor::~ImageImfor()
{
}

void ImageImfor::init(Mat & src, String imgname)
{
	name = imgname;

	GaussianBlur(src, blurM, Size(3, 3), 0, 0);

	cvtColor(blurM, blurM, COLOR_RGB2GRAY);

	//equalizeHist(blurM, blurM);

	threshold(blurM, threshTZ, 50, 255, THRESH_TOZERO);

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

	threshold(absY, threshYM, 50, 255, THRESH_BINARY);
	
}

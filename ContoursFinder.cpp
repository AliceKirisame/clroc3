#include "ContoursFinder.h"



ContoursFinder::ContoursFinder():
	ELLIPSE_A_DIV(6.0), ELLIPSE_B_DIV(4.0), MIN_PLATE_AREA(900)
{
}


ContoursFinder::~ContoursFinder()
{
}

void ContoursFinder::countDist(Point pt, Rect rc, double a, double b, vector<double> &dists)
{
	dists.clear();
	double distX, distY;

	distX = rc.x - pt.x;
	distY = rc.y - pt.y;

	//dists.push_back(distX + distY);
	dists.push_back(distX*distX / (a*a) + distY*distY / (b*b));

	distX = rc.x + rc.width - pt.x;
	distY = rc.y - pt.y;

	//dists.push_back(distX + distY);
	dists.push_back(distX*distX / (a*a) + distY*distY / (b*b));

	distX = rc.x + rc.width - pt.x;
	distY = rc.y + rc.height - pt.y;

	//dists.push_back(distX + distY);
	dists.push_back(distX*distX / (a*a) + distY*distY / (b*b));

	distX = rc.x - pt.x;
	distY = rc.y + rc.height - pt.y;

	//dists.push_back(distX + distY);
	dists.push_back(distX*distX / (a*a) + distY*distY / (b*b));
}

void ContoursFinder::findRect(std::vector<Mat>& src, std::vector<vector<Rect>>& rects, float rmin, float rmax)
{

	Mat tmpMat;
	rects.clear();

	for (int i = 0; i < src.size(); i++)
	{
		src[i].copyTo(tmpMat);
		rects.push_back(vector<Rect>());

		m_rectContoursNum.clear();

		findContours(tmpMat, m_contours, m_hie, RETR_TREE, CHAIN_APPROX_SIMPLE, Point(0, 0));


		contours_poly.resize(m_contours.size());
		boundRect.resize(m_contours.size());
		center.resize(m_contours.size());
		radius.resize(m_contours.size());
		minRect.resize(m_contours.size());
		g_hull.resize(m_contours.size());

		for (int i = 0; i < m_contours.size(); i++)
		{
			approxPolyDP(Mat(m_contours[i]), contours_poly[i], 3, false);

			boundRect[i] = boundingRect(Mat(m_contours[i]));

			minEnclosingCircle(m_contours[i], center[i], radius[i]);

			minRect[i] = minAreaRect(Mat(m_contours[i]));

			convexHull(m_contours[i], g_hull[i], true);
		}





		for (int i = 0; i < m_contours.size(); i++)
		{
			double conArea = contourArea(m_contours[i]);
			double recArea = minRect[i].size.area();
			double hullArea = contourArea(g_hull[i]);
			Rect nowRc;

			//if (abs(recArea - hullArea) < recArea / 5 && hullArea > MIN_PLATE_AREA)
			if (recArea > MIN_PLATE_AREA)
			{
				//drawContours(g_tmpOut, m_contours, i, Scalar(0, 0, 255));

				//for (int j = 0; j < g_hull[i].size(); j++)
				//{
				//line(g_tmpOut, g_hull[i][j], g_hull[i][(j + 1) % g_hull[i].size()], Scalar(0, 255, 0));
				//}
				nowRc = boundRect[i];

				if (double(nowRc.width) / nowRc.height >= rmin && double(nowRc.width) / nowRc.height <= rmax)
					if(nowRc.width < tmpMat.cols/2)
						m_rectContoursNum.push_back(i);
			}
		}



		for (int j = 0; j < m_rectContoursNum.size(); j++)
		{
			rects[i].push_back(boundRect[m_rectContoursNum[j]]);
		}


	}
}

void ContoursFinder::findRect(Mat& src, std::vector<Rect>& rects, float rmin, float rmax, int minArea)
{

	rects.clear();

	m_rectContoursNum.clear();

	findContours(src, m_contours, m_hie, RETR_TREE, CHAIN_APPROX_SIMPLE, Point(0, 0));


	contours_poly.resize(m_contours.size());
	boundRect.resize(m_contours.size());
	center.resize(m_contours.size());
	radius.resize(m_contours.size());
	minRect.resize(m_contours.size());
	g_hull.resize(m_contours.size());

	for (int i = 0; i < m_contours.size(); i++)
	{
		approxPolyDP(Mat(m_contours[i]), contours_poly[i], 3, false);

		boundRect[i] = boundingRect(Mat(m_contours[i]));

		minEnclosingCircle(m_contours[i], center[i], radius[i]);

		minRect[i] = minAreaRect(Mat(m_contours[i]));

		convexHull(m_contours[i], g_hull[i], true);
	}





	for (int i = 0; i < m_contours.size(); i++)
	{
		double conArea = contourArea(m_contours[i]);
		double recArea = minRect[i].size.area();
		double hullArea = contourArea(g_hull[i]);
		Rect nowRc;

		//if (abs(recArea - hullArea) < recArea / 5 && hullArea > MIN_PLATE_AREA)
		if (recArea > minArea)
		{
			//drawContours(g_tmpOut, m_contours, i, Scalar(0, 0, 255));

			//for (int j = 0; j < g_hull[i].size(); j++)
			//{
			//line(g_tmpOut, g_hull[i][j], g_hull[i][(j + 1) % g_hull[i].size()], Scalar(0, 255, 0));
			//}
			nowRc = boundRect[i];

			if (double(nowRc.width) / nowRc.height >= rmin && double(nowRc.width) / nowRc.height <= rmax)
				m_rectContoursNum.push_back(i);
		}
	}




	for (int j = 0; j < m_rectContoursNum.size(); j++)
	{
		rects.push_back(boundRect[m_rectContoursNum[j]]);
	}

}

void ContoursFinder::findRect(std::vector<Mat>& src, std::vector<Rect>& rects, float rmin , float rmax)
{

	rects.clear();

	for (int i = 0; i < src.size(); i++)
	{
		m_rectContoursNum.clear();
		
		findContours(src[i], m_contours, m_hie, RETR_TREE, CHAIN_APPROX_SIMPLE, Point(0, 0));

		
		contours_poly.resize(m_contours.size());
		boundRect.resize(m_contours.size());
		center.resize(m_contours.size());
		radius.resize(m_contours.size());
		minRect.resize(m_contours.size());
		g_hull.resize(m_contours.size());
		
		for (int i = 0; i < m_contours.size(); i++)
		{
			approxPolyDP(Mat(m_contours[i]), contours_poly[i], 3, false);

			boundRect[i] = boundingRect(Mat(m_contours[i]));

			minEnclosingCircle(m_contours[i], center[i], radius[i]);

			minRect[i] = minAreaRect(Mat(m_contours[i]));

			convexHull(m_contours[i], g_hull[i], true);
		}
		
		

		

		for (int i = 0; i < m_contours.size(); i++)
		{
			double conArea = contourArea(m_contours[i]);
			double recArea = minRect[i].size.area();
			double hullArea = contourArea(g_hull[i]);



			Rect nowRc;

			//if (abs(recArea - hullArea) < recArea / 5 && hullArea > MIN_PLATE_AREA)
			if (recArea > MIN_PLATE_AREA)
			{
				//drawContours(g_tmpOut, m_contours, i, Scalar(0, 0, 255));

				//for (int j = 0; j < g_hull[i].size(); j++)
				//{
				//line(g_tmpOut, g_hull[i][j], g_hull[i][(j + 1) % g_hull[i].size()], Scalar(0, 255, 0));
				//}
				nowRc = boundRect[i];

				if (double(nowRc.width) / nowRc.height >= rmin && double(nowRc.width) / nowRc.height <= rmax)
					m_rectContoursNum.push_back(i);
			}
		}

		

		
		for (int j = 0; j < m_rectContoursNum.size(); j++)
		{
			rects.push_back(boundRect[m_rectContoursNum[j]]);
		}
		
		
	}

	
	/*
		g_tmpOut = Mat::zeros(g_srcImg.rows, g_srcImg.cols, g_srcImg.type());

		for (int i = 0; i < m_contours.size(); i++)
		{
		drawContours(g_tmpOut, m_contours, i, Scalar(0, 0, 255));

		rectangle(g_tmpOut, boundRect[i], Scalar(0, 255, 0));

		circle(g_tmpOut, center[i], radius[i], Scalar(255, 0, 0));
		}
		*/

		//过滤严重变形和不是类似车牌的轮廓


		/*
		vector<vector<Point>> verts;
		vector<vector<double>> dists;
		vector<Point> firstPoints;
		vector<double> firstDists;

		verts.reserve(m_rectContoursNum.size());
		dists.reserve(m_rectContoursNum.size());

		vector<Point> nowContours;
		vector<double> newDist;
		Rect nowRc;
		int nowContoursNum;

		double a, b;

		firstPoints.resize(4);
		firstDists.resize(4);

		for (int i = 0; i < m_rectContoursNum.size(); i++)
		{
		nowContoursNum = m_rectContoursNum[i];

		nowContours = m_contours[nowContoursNum];
		nowRc = boundRect[nowContoursNum];

		a = (double)nowRc.width / ELLIPSE_A_DIV;
		b = (double)nowRc.height / ELLIPSE_B_DIV;

		for (int j = 0; j < 4; j++)
		{
		firstPoints[j] = nowContours[0];
		firstDists[j] = 0.0;
		}

		verts.push_back(firstPoints);
		dists.push_back(firstDists);

		countDist(verts[i][0], nowRc, a, b, dists[i]);

		for (int j = 1; j < int(nowContours.size()); j++)
		{
		countDist(nowContours[j], nowRc, a, b, newDist);

		for (int k = 0; k < 4; k++)
		{
		if (dists[i][k] > newDist[k])
		{
		dists[i][k] = newDist[k];
		verts[i][k] = nowContours[j];
		}
		}
		}

		}

		int counting;
		bool flag;

		counting = 0;
		for (vector<int>::iterator it = m_rectContoursNum.begin(); it != m_rectContoursNum.end(); counting++)
		{
		flag = false;

		nowContoursNum = *it;

		nowContours = m_contours[nowContoursNum];
		nowRc = boundRect[nowContoursNum];

		//double maxOffset = nowRc.height / 10.0 + nowRc.width / 10.0;
		//double maxOffset = nowRc.height / 10.0 * nowRc.height / 10.0 + nowRc.width / 10.0 * nowRc.width / 10.0;


		//for (int j = 0; j < 4 && flag != true; j++)
		//{
		//if (verts[counting][j] == verts[counting][(j + 1) % 4])
		//{
		//flag = true;
		//}
		//}


		for (int j = 0; j < 4 && flag != true; j++)
		{
		if (dists[counting][j] > 1)
		{
		flag = true;
		}
		}

		if (double(nowRc.width) / nowRc.height < 1.5 || double(nowRc.width) / nowRc.height > 5.5)
		flag = true;

		if (flag == true)
		{
		it = m_rectContoursNum.erase(it);
		}
		else
		{
		it++;
		}

		}
		*/
}

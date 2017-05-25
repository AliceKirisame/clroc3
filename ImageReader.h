#pragma once
#include <core.hpp>
#include <opencv2\imgproc\imgproc.hpp>
#include <opencv2\highgui\highgui.hpp>

#include <iostream>
#include <vector>
#include <io.h>
#include <stdlib.h>

using namespace cv;
using namespace std;


class ImageReader
{
public:
	ImageReader(bool debug = false);
	~ImageReader();

	void init(const String& imgFolder);
	bool next(String& imgName);
	inline int getCount() { return m_vecNames.size(); };

private:
	String m_strFolder;
	int m_iIndex;
	vector<String> m_vecNames;

	long m_file;
	bool m_bShowImfor;

	vector<String>::iterator m_itIndex;
};


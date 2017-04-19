#include "imageReader.h"



ImageReader::ImageReader(bool debug)
{
	m_bShowImfor = debug;
}


ImageReader::~ImageReader()
{
}

void ImageReader::init(const String & path)
{
	m_strFolder = path;
	m_iIndex = 0;
	m_vecNames.clear();

	m_file = 0;

	intptr_t hFile = 0;
	struct _finddata_t fileInfo;
	string pathName ;

	if ((hFile = _findfirst(pathName.assign(path).append("\\*").c_str(), &fileInfo)) == -1) {
		return;
	}
	do {
		if(m_bShowImfor)
			cout << fileInfo.name << (fileInfo.attrib&_A_SUBDIR ? "[folder]" : "[file]") << endl;

		if (!(fileInfo.attrib&_A_SUBDIR))
		{
			m_vecNames.push_back(fileInfo.name);
		}

	} while (_findnext(hFile, &fileInfo) == 0);
	_findclose(hFile);

	m_iIndex = 0;

	m_itIndex = m_vecNames.begin();

	return;

}

bool ImageReader::next(String & imgName)
{
	
	if (m_iIndex < m_vecNames.size())
	{
		imgName = *m_itIndex;

		m_itIndex++;
		m_iIndex++;

		return true;
	}
	return false;
}

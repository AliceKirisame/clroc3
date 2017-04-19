#pragma once

#ifndef  PATHS_H

#define PATHS_H

#include <iostream>
#include <string>

using namespace std;

class Paths
{
public:
	Paths();
	~Paths();

	static string s_strOutPath;
	static string s_strTemplatePath;
	static string s_strTestPath;
};

#endif // ! PATHS_H
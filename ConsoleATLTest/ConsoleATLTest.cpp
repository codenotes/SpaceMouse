// ConsoleATLTest.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

using namespace ATL;
using namespace osgVisual;

int _tmain(int argc, _TCHAR* argv[])
{

	auto s = osgVisual::SpaceMouse();
	double x, y, z;
	int ret=s.initialize();

	printf("returned:%d", ret);

	//return 0;
	while (true)
	{

		//s.getTranslations(x, y, z);
		s.getRotations(x, y, z);
		printf("%f.5 %.5f %.5f\n", x, y, z);


	}

	return 0;
}


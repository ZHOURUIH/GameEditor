#include <iostream>
#include "Utility.h"

void main()
{
	int input = 0;
	std::cout << "RGBתHSL(1),HSLתRGB(2)" << std::endl;
	std::cin >> input;
	if (input != 1 && input != 2)
	{
		return;
	}
	std::vector<std::string> fileList;
	std::vector<std::string> patterns;
	patterns.push_back(".png");
	Utility::findFiles("./", fileList, patterns);
	int count = fileList.size();
	for (int i = 0; i < count; ++i)
	{
		if (input == 1)
		{
			Utility::generateHSLTexture(fileList[i]);
		}
		else
		{
			Utility::generateRGBTexture(fileList[i]);
		}
	}
}
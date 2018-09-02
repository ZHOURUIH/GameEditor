#ifndef _UTILITY_H_
#define _UTILITY_H_

#include <string>
#include <vector>
#include <windows.h>
#include <cstdio>
#include <cstdlib>
#include <direct.h>
#include "Vector3.h"
#include "FreeImage.h"

class Utility
{
public:
	static void findFiles(std::string path, std::vector<std::string>& fileList, const std::vector<std::string>& patterns);
	static float getMin(float v1, float v2);
	static float getMin(float v1, float v2, float v3);
	static float getMax(float v1, float v2);
	static float getMax(float v1, float v2, float v3);
	static float HueToRGB(float v1, float v2, float vH);
	static vec3 RGBtoHSL(vec3 rgb);
	static vec3 HSLtoRGB(vec3 hsl);
	static float clamp(float value, float minValue, float maxValue);
	static void openFile(const std::string& fileName, char*& buffer, int& fileSize);
	static bool writeFile(const std::string& fileName, char* buffer, int writeCount);
	static void generateRGBTexture(const std::string& src);
	static void generateHSLTexture(const std::string& src);
};

#endif
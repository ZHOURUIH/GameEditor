#include "Utility.h"

void Utility::findFiles(std::string path, std::vector<std::string>& fileList, const std::vector<std::string>& patterns)
{
	WIN32_FIND_DATA FindData;
	HANDLE hError;
	// 构造路径
	std::string pathName = path + "\\*.*";
	hError = FindFirstFile(pathName.c_str(), &FindData);
	if (hError == INVALID_HANDLE_VALUE)
	{
		return;
	}
	while (::FindNextFile(hError, &FindData))
	{
		// 过虑.和..
		if (strcmp(FindData.cFileName, ".") == 0
			|| strcmp(FindData.cFileName, "..") == 0)
		{
			continue;
		}

		// 构造完整路径
		std::string fullname = path + "\\" + std::string(FindData.cFileName);
		if (FindData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
		{
			findFiles(fullname, fileList, patterns);
		}
		else
		{
			int patternCount = patterns.size();
			for (int i = 0; i < patternCount; ++i)
			{
				if (fullname.length() >= patterns[i].length() && fullname.substr(fullname.length() - patterns[i].length(), patterns[i].length()) == patterns[i])
				{
					fileList.push_back(fullname);
				}
			}
		}
	}
}

float Utility::getMin(float v1, float v2)
{
	return v1 < v2 ? v1 : v2;
}

float Utility::getMin(float v1, float v2, float v3)
{
	return getMin(getMin(v1, v2), v3);
}

float Utility::getMax(float v1, float v2)
{
	return v1 > v2 ? v1 : v2;
}

float Utility::getMax(float v1, float v2, float v3)
{
	return getMax(getMax(v1, v2), v3);
}

float Utility::HueToRGB(float v1, float v2, float vH)
{
	if (vH < 0.0f)
	{
		vH += 1.0f;
	}
	if (vH > 1.0f)
	{
		vH -= 1.0f;
	}
	if (6.0f * vH < 1.0f)
	{
		return v1 + (v2 - v1) * 6.0f * vH;
	}
	if (2.0f * vH < 1.0f)
	{
		return v2;
	}
	if (3.0f * vH < 2.0f)
	{
		return v1 + (v2 - v1) * (0.666f - vH) * 6.0f;
	}
	return v1;
}

// rgb转换为色相(H),饱和度(S),亮度(L)
// HSL和RGB的范围都是0-1
vec3 Utility::RGBtoHSL(vec3 rgb)
{
	float minRGB = getMin(rgb.r, rgb.g, rgb.b);
	float maxRGB = getMax(rgb.r, rgb.g, rgb.b);
	float delta = maxRGB - minRGB;

	float H = 0.0f;
	float S = 0.0f;
	float L = (maxRGB + minRGB) / 2.0f;
	// 如果三个分量的最大和最小相等,则说明该颜色是灰色的,灰色的色相和饱和度都为0
	if (delta > 0.0f)								//Chromatic data...
	{
		if (L < 0.5f)
		{
			S = delta / (maxRGB + minRGB);
		}
		else
		{
			S = delta / (2.0f - maxRGB - minRGB);
		}

		float delR = ((maxRGB - rgb.r) / 6.0f + delta / 2.0f) / delta;
		float delG = ((maxRGB - rgb.g) / 6.0f + delta / 2.0f) / delta;
		float delB = ((maxRGB - rgb.b) / 6.0f + delta / 2.0f) / delta;

		if (rgb.r == maxRGB)
		{
			H = delB - delG;
		}
		else if (rgb.g == maxRGB)
		{
			H = 0.333f + delR - delB;
		}
		else if (rgb.b == maxRGB)
		{
			H = 0.666f + delG - delR;
		}

		if (H < 0.0f)
		{
			H += 1.0f;
		}
		if (H > 1.0f)
		{
			H -= 1.0f;
		}
	}
	return vec3(H, S, L);
}

// 色相(H),饱和度(S),亮度(L),转换为rgb
// HSL和RGB的范围都是0-1
vec3 Utility::HSLtoRGB(vec3 hsl)
{
	vec3 rgb;
	float H = hsl.r;
	float S = hsl.g;
	float L = hsl.b;
	if (S == 0.0f)						//HSL from 0 to 1
	{
		rgb.r = L;				//RGB results from 0 to 255
		rgb.g = L;
		rgb.b = L;
	}
	else
	{
		float var2;
		if (L < 0.5f)
		{
			var2 = L * (1.0f + S);
		}
		else
		{
			var2 = L + S - (S * L);
		}

		float var1 = 2.0f * L - var2;
		rgb.r = HueToRGB(var1, var2, H + 0.333f);
		rgb.g = HueToRGB(var1, var2, H);
		rgb.b = HueToRGB(var1, var2, H - 0.333f);
	}
	return rgb;
}

float Utility::clamp(float value, float minValue, float maxValue)
{
	if (value < minValue)
	{
		value = minValue;
	}
	else if (value > maxValue)
	{
		value = maxValue;
	}
	return value;
}

void Utility::openFile(const std::string& fileName, char*& buffer, int& fileSize)
{
	FILE* pFile = NULL;
	fopen_s(&pFile, fileName.c_str(), "rb");
	if (pFile == NULL)
	{
		buffer = NULL;
		fileSize = 0;
		return;
	}
	fseek(pFile, 0, SEEK_END);
	fileSize = ftell(pFile);
	rewind(pFile);
	buffer = new char[fileSize];
	fread(buffer, sizeof(char), fileSize, pFile);
	fclose(pFile);
}

bool Utility::writeFile(const std::string& fileName, char* buffer, int writeCount)
{
	if (buffer == NULL || writeCount == 0)
	{
		return true;
	}
	FILE* pFile = NULL;
	fopen_s(&pFile, fileName.c_str(), "wb");
	if (pFile == NULL)
	{
		return false;
	}
	fwrite(buffer, sizeof(char), writeCount, pFile);
	fclose(pFile);
	return true;
}

void Utility::generateRGBTexture(const std::string& src)
{
	FreeImage_Initialise(1);
	FREE_IMAGE_FORMAT fif = FreeImage_GetFileType(src.c_str(), 0);
	FIBITMAP* bitmap = FreeImage_Load(fif, src.c_str(), 0);
	if (bitmap == NULL)
	{
		return;
	}

	// 读取原图像素
	unsigned short width = FreeImage_GetWidth(bitmap);
	unsigned short height = FreeImage_GetHeight(bitmap);
	BOOL hasAlpha = FreeImage_IsTransparent(bitmap);
	int pixelSize = (3 + hasAlpha);
	int lineLength = width * pixelSize;
	int bufferSize = lineLength * height;
	char* buffer = new char[bufferSize];
	for (int y = 0; y < height; ++y)
	{
		for (int x = 0; x < width; ++x)
		{
			RGBQUAD color;
			FreeImage_GetPixelColor(bitmap, x, y, &color);
			vec3 hsl(color.rgbRed / 255.0f, color.rgbGreen / 255.0f, color.rgbBlue / 255.0f);
			vec3 rgb = HSLtoRGB(hsl);
			rgb.r = clamp(rgb.r, 0.0f, 1.0f);
			rgb.g = clamp(rgb.g, 0.0f, 1.0f);
			rgb.b = clamp(rgb.b, 0.0f, 1.0f);
			buffer[lineLength * y + x * pixelSize + 0] = (int)(rgb.r * 255.0f + 0.5f);
			buffer[lineLength * y + x * pixelSize + 1] = (int)(rgb.g * 255.0f + 0.5f);
			buffer[lineLength * y + x * pixelSize + 2] = (int)(rgb.b * 255.0f + 0.5f);
			if (hasAlpha)
			{
				buffer[lineLength * y + x * pixelSize + 3] = color.rgbReserved;
			}
		}
	}
	FreeImage_Unload(bitmap);

	// 保存HSL到新的图片
	FIBITMAP* bitmapRet = FreeImage_Allocate(width, height, pixelSize * 8);
	for (int y = 0; y < height; ++y)
	{
		BYTE* bits = FreeImage_GetScanLine(bitmapRet, y);
		for (int x = 0; x < width; ++x)
		{
			bits[x * pixelSize + 0] = buffer[lineLength * y + pixelSize * x + 2];
			bits[x * pixelSize + 1] = buffer[lineLength * y + pixelSize * x + 1];
			bits[x * pixelSize + 2] = buffer[lineLength * y + pixelSize * x + 0];
			if (hasAlpha)
			{
				bits[x * pixelSize + 3] = buffer[lineLength * y + pixelSize * x + 3];
			}
		}
	}
	delete[] buffer;
	// 保存为png
	std::string newFileName = src + "_RGB";
	int lastPos = src.find_last_of('.');
	if (lastPos != -1)
	{
		std::string suffix = src.substr(lastPos, src.length() - lastPos);
		std::string fileName = src.substr(0, lastPos);
		newFileName = fileName + "_RGB" + suffix;
	}
	FreeImage_Save(FIF_PNG, bitmapRet, newFileName.c_str());
	FreeImage_Unload(bitmapRet);

	FreeImage_DeInitialise();
	return;
}

//return 1 if succ, -1 if failed
void Utility::generateHSLTexture(const std::string& src)
{
	FreeImage_Initialise(1);
	FREE_IMAGE_FORMAT fif = FreeImage_GetFileType(src.c_str(), 0);
	FIBITMAP* bitmap = FreeImage_Load(fif, src.c_str(), 0);
	if (bitmap == NULL)
	{
		return;
	}

	// 读取原图像素
	unsigned short width = FreeImage_GetWidth(bitmap);
	unsigned short height = FreeImage_GetHeight(bitmap);
	BOOL hasAlpha = FreeImage_IsTransparent(bitmap);
	int pixelSize = (3 + hasAlpha);
	int lineLength = width * pixelSize;
	int bufferSize = lineLength * height;
	char* buffer = new char[bufferSize];
	for (int y = 0; y < height; ++y)
	{
		for (int x = 0; x < width; ++x)
		{
			RGBQUAD color;
			FreeImage_GetPixelColor(bitmap, x, y, &color);
			vec3 rgb(color.rgbRed / 255.0f, color.rgbGreen / 255.0f, color.rgbBlue / 255.0f);
			vec3 hsl = RGBtoHSL(rgb);
			hsl.r = clamp(hsl.r, 0.0f, 1.0f);
			hsl.g = clamp(hsl.g, 0.0f, 1.0f);
			hsl.b = clamp(hsl.b, 0.0f, 1.0f);
			buffer[lineLength * y + x * pixelSize + 0] = (int)(hsl.r * 255.0f + 0.5f);
			buffer[lineLength * y + x * pixelSize + 1] = (int)(hsl.g * 255.0f + 0.5f);
			buffer[lineLength * y + x * pixelSize + 2] = (int)(hsl.b * 255.0f + 0.5f);
			if (hasAlpha)
			{
				buffer[lineLength * y + x * pixelSize + 3] = color.rgbReserved;
			}
		}
	}
	FreeImage_Unload(bitmap);
	
	// 保存HSL到新的图片
	FIBITMAP* bitmapRet = FreeImage_Allocate(width, height, pixelSize * 8);
	for (int y = 0; y < height; ++y)
	{
		BYTE* bits = FreeImage_GetScanLine(bitmapRet, y);
		for (int x = 0; x < width; ++x)
		{
			bits[x * pixelSize + 0] = buffer[lineLength * y + pixelSize * x + 2];
			bits[x * pixelSize + 1] = buffer[lineLength * y + pixelSize * x + 1];
			bits[x * pixelSize + 2] = buffer[lineLength * y + pixelSize * x + 0];
			if (hasAlpha)
			{
				bits[x * pixelSize + 3] = buffer[lineLength * y + pixelSize * x + 3];
			}
		}
	}
	delete[] buffer;
	// 保存为png
	std::string newFileName = src + "_HSL";
	int lastPos = src.find_last_of('.');
	if (lastPos != -1)
	{
		std::string suffix = src.substr(lastPos, src.length() - lastPos);
		std::string fileName = src.substr(0, lastPos);
		newFileName = fileName + "_HSL" + suffix;
	}
	FreeImage_Save(FIF_PNG, bitmapRet, newFileName.c_str());
	FreeImage_Unload(bitmapRet);

	FreeImage_DeInitialise();
	return;
}
#include "Utility.h"
#include "FreeImage.h"
#include "txMemeryTrace.h"

std::string Utility::getFileName(std::string str)
{
	rightToLeft(str);
	int dotPos = str.find_last_of('/');
	if (dotPos != -1)
	{
		return str.substr(dotPos + 1, str.length() - 1);
	}
	return str;
}

std::string Utility::getFileNameNoSuffix(std::string str)
{
	rightToLeft(str);
	int namePos = str.find_last_of('/');
	int dotPos = str.find_last_of('.');
	if (dotPos != -1)
	{
		return str.substr(namePos + 1, dotPos - namePos - 1);
	}

	return str;
}

std::string Utility::getParentDir(std::string dir)
{
	rightToLeft(dir);
	int pos = dir.find_last_of('/');
	std::string tempDir = dir;
	if (-1 != pos)
	{
		tempDir = dir.substr(0, pos);
	}

	return tempDir;
}

void Utility::rightToLeft(std::string& str)
{
	int pathLength = str.length();
	for (int i = 0; i < pathLength; ++i)
	{
		if ('\\' == str[i])
		{
			str[i] = '/';
		}
	}
}

void Utility::split(std::string str, const char* deli, std::vector<std::string>* vec)
{
	while (true)
	{
		int devidePos = str.find_first_of(deli);
		if (devidePos == -1)
		{
			vec->push_back(str);
			break;
		}
		else
		{
			std::string curString = str.substr(0, devidePos);
			vec->push_back(curString);
			str = str.substr(devidePos + strlen(deli), str.length() - devidePos - strlen(deli));
		}
	}
}
bool Utility::writeFile(std::string filePath, int length, const char* buffer)
{
	if (length <= 0 || NULL == buffer)
	{
		return false;
	}

	Utility::rightToLeft(filePath);

	int pos = filePath.find_last_of('/');
	if (-1 != pos)
	{
		std::string dirPath = filePath.substr(0, pos);
		if (!createFolder(dirPath))
		{
			return false;
		}
	}
	else
	{
		if (!createFolder(filePath))
		{
			return false;
		}
	}

	FILE* pFile = NULL;
	fopen_s(&pFile, filePath.c_str(), "wb");

	if (pFile == NULL)
	{
		return false;
	}

	fwrite(buffer, sizeof(char), length, pFile);
	fclose(pFile);

	return true;
}
bool Utility::createFolder(std::string path)
{
	// 如果有上一级目录,并且上一级目录不存在,则先创建上一级目录
	std::string parentDir = getParentDir(path);
	if (parentDir != path)
	{
		createFolder(parentDir);
	}
	// 如果文件不存在则创建文件
	if (!isFileExist(path))
	{
		if (0 != _mkdir(path.c_str()))
		{
			return false;
		}
	}
	return true;
}
bool Utility::isFileExist(std::string fullPath)
{
	int ret = _access(fullPath.c_str(), 0);
	return ret == 0;
}
std::string Utility::intToString(int i, int limitLen)
{
	char str[256];
	sprintf_s(str, 256, "%d", i);
	std::string retString(str);
	int addLen = limitLen - strlen(str);
	if (addLen > 0)
	{
		char addStr[256];
		memset(addStr, 0, 256);
		for (int j = 0; j < addLen; ++j)
		{
			addStr[j] = '0';
		}
		retString = std::string(addStr) + retString;
	}

	return retString;
}
float Utility::pow_f(float f, int p)
{
	float ret = 1.0f;
	while (p)
	{
		ret *= f;
		--p;
	}
	return ret;
}
std::string Utility::floatToString(float f, int precision)
{
	f = f * pow_f(10.0f, precision) + 0.5f;
	int MAX_INT = 0x7FFFFFFF;
	if (f > (float)MAX_INT)
	{
		char str[256];
		sprintf_s(str, 256, "%f", f);
		return std::string(str);
	}

	int intValue = (int)f;

	char str[256];
	sprintf_s(str, 256, "%d", intValue);
	if (precision == 0)
	{
		return std::string(str);
	}
	else
	{
		int length = strlen(str);
		if (length <= precision)
		{
			char newStr[256];
			memset(newStr, 0, 256 * sizeof(char));
			for (int i = 0; i < precision - length + 1; ++i)
			{
				newStr[i] = '0';
			}
			memcpy(newStr + precision - length + 1, str, length);

			char intStr[256];
			memset(intStr, 0, 256 * sizeof(char));
			char decimalStr[256];
			memset(decimalStr, 0, 256 * sizeof(char));
			memcpy(intStr, newStr, strlen(newStr) - precision);
			memcpy(decimalStr, newStr + strlen(newStr) - precision, precision);
			return std::string(intStr) + "." + std::string(decimalStr);
		}
		else
		{
			char intStr[256];
			memset(intStr, 0, 256 * sizeof(char));
			char decimalStr[256];
			memset(decimalStr, 0, 256 * sizeof(char));
			memcpy(intStr, str, strlen(str) - precision);
			memcpy(decimalStr, str + strlen(str) - precision, precision);
			return std::string(intStr) + "." + std::string(decimalStr);
		}
	}
}
std::string Utility::ANSIToUTF8(const std::string& str, const bool& addBOM)
{
	std::wstring unicodeStr = ANSIToUnicode(str);
	std::string utf8Str = UnicodeToUTF8(unicodeStr);
	if (addBOM)
	{
		char bom[4] = { (char)0xEF, (char)0xBB, (char)0xBF, 0 };
		utf8Str = std::string(bom) + utf8Str;
	}
	return utf8Str;
}
std::wstring Utility::ANSIToUnicode(const std::string& str)
{
	int len = str.length();
	int unicodeLen = ::MultiByteToWideChar(CP_ACP, 0, str.c_str(), -1, NULL, 0);
	wchar_t* pUnicode = TRACE_NEW_ARRAY(wchar_t, unicodeLen + 1, pUnicode);
	memset(pUnicode, 0, (unicodeLen + 1)*sizeof(wchar_t));
	::MultiByteToWideChar(CP_ACP, 0, str.c_str(), -1, (LPWSTR)pUnicode, unicodeLen);
	std::wstring rt = (wchar_t*)pUnicode;
	TRACE_DELETE_ARRAY(pUnicode);
	return rt;
}
std::string Utility::UnicodeToUTF8(const std::wstring& str)
{
	// wide char to multi char
	int iTextLen = WideCharToMultiByte(CP_UTF8, 0, str.c_str(), -1, NULL, 0, NULL, NULL);
	char* pElementText = TRACE_NEW_ARRAY(char, iTextLen + 1, pElementText);
	memset((void*)pElementText, 0, sizeof(char)* (iTextLen + 1));
	::WideCharToMultiByte(CP_UTF8, 0, str.c_str(), -1, pElementText, iTextLen, NULL, NULL);
	std::string strText = pElementText;
	TRACE_DELETE_ARRAY(pElementText);
	return strText;
}

void Utility::findFiles(const std::string& path, std::vector<std::string>& fileList, const std::string& pattern)
{
	std::vector<std::string> list;
	list.push_back(pattern);
	findFiles(path, fileList, list);
}

void Utility::findFiles(const std::string& path, std::vector<std::string>& fileList, const std::vector<std::string>& patterns)
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
		std::string filename = std::string(FindData.cFileName);
		if (FindData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
		{
			findFiles(fullname, fileList, patterns);
		}
		else
		{
			int patternCount = patterns.size();
			if (patternCount > 0)
			{
				for (int i = 0; i < patternCount; ++i)
				{
					if (fullname.length() >= patterns[i].length() && fullname.substr(fullname.length() - patterns[i].length(), patterns[i].length()) == patterns[i])
					{
						fileList.push_back(fullname);
					}
				}
			}
			else
			{
				fileList.push_back(fullname);
			}
		}
	}
}

Vector2 Utility::getImageSize(const std::string& fileName)
{
	FreeImage_Initialise(1);
	FREE_IMAGE_FORMAT fif = FreeImage_GetFileType(fileName.c_str(), 0);
	FIBITMAP* bitmap = FreeImage_Load(fif, fileName.c_str(), 0);
	if (bitmap == NULL)
	{
		FreeImage_DeInitialise();
		return Vector2();
	}
	Vector2 size;
	size.x = FreeImage_GetWidth(bitmap);
	size.y = FreeImage_GetHeight(bitmap);
	FreeImage_DeInitialise();
	return size;
}

void Utility::convert(const std::string& src, const Vector2& targetSize, Vector2& imageNewSize, Vector2& imageTargetSize, FloatVector2& imageTargetPos)
{
	FreeImage_Initialise(1);
	FREE_IMAGE_FORMAT fif = FreeImage_GetFileType(src.c_str(), 0);
	FIBITMAP* bitmap = FreeImage_Load(fif, src.c_str(), 0);
	if (bitmap == NULL)
	{
		FreeImage_DeInitialise();
		return;
	}
	unsigned short width = FreeImage_GetWidth(bitmap);
	unsigned short height = FreeImage_GetHeight(bitmap);

	// 判断裁剪区域
	int minY = 0;
	for (int y = 0; y < height; ++y)
	{
		int minAlpha = 255;
		int maxAlpha = 0;
		int totalAlpha = 0;
		for (int x = 0; x < width; ++x)
		{
			RGBQUAD pixel;
			FreeImage_GetPixelColor(bitmap, x, y, &pixel);
			int alpha = pixel.rgbReserved;
			if (alpha < minAlpha)
			{
				minAlpha = alpha;
			}
			if (alpha >maxAlpha)
			{
				maxAlpha = alpha;
			}
			totalAlpha += alpha;
		}
		if (totalAlpha / width < 15 && (maxAlpha - minAlpha) < 20)
		{
			minY = y;
		}
		else
		{
			break;
		}
	}
	int maxY = height - 1;
	for (int y = 0; y < height; ++y)
	{
		int minAlpha = 255;
		int maxAlpha = 0;
		int totalAlpha = 0;
		for (int x = 0; x < width; ++x)
		{
			RGBQUAD pixel;
			FreeImage_GetPixelColor(bitmap, x, height - y - 1, &pixel);
			int alpha = pixel.rgbReserved;
			if (alpha < minAlpha)
			{
				minAlpha = alpha;
			}
			if (alpha >maxAlpha)
			{
				maxAlpha = alpha;
			}
			totalAlpha += alpha;
		
		}
		if (totalAlpha / width < 15 && (maxAlpha - minAlpha) <20)
		{
			maxY = height - y ;
		}
		else
		{
			break;
		}
	}
	int minX = 0;
	for (int x = 0; x < width; ++x)
	{
		int minAlpha = 255;
		int maxAlpha = 0;
		int totalAlpha = 0;
		for (int y = 0; y < height; ++y)
		{
			RGBQUAD pixel;
			FreeImage_GetPixelColor(bitmap, x, y, &pixel);
			int alpha = pixel.rgbReserved;
			if (alpha < minAlpha)
			{
				minAlpha = alpha;
			}
			if (alpha >maxAlpha)
			{
				maxAlpha = alpha;
			}
			totalAlpha += alpha;
			
		}
		if (totalAlpha / width < 15 && (maxAlpha - minAlpha) <20)
		{
			minX = x;
		}
		else
		{
			break;
		}
	}
	int maxX = width - 1;
	for (int x = 0; x < width; ++x)
	{
		int minAlpha = 255;
		int maxAlpha = 0;
		int totalAlpha = 0;
		for (int y = 0; y < height; ++y)
		{
			RGBQUAD pixel;
			FreeImage_GetPixelColor(bitmap, width - x - 1, y, &pixel);
			int alpha = pixel.rgbReserved;
			if (alpha < minAlpha)
			{
				minAlpha = alpha;
			}
			if (alpha >maxAlpha)
			{
				maxAlpha = alpha;
			}
			totalAlpha += alpha;
			
		}
		if (totalAlpha / width < 15 && (maxAlpha - minAlpha) <20)
		{
			maxX = width - x ;
		}
		else
		{
			break;
		}
	}
	imageNewSize.x = maxX - minX;
	imageNewSize.y = maxY - minY;
	float targetX = ((minX - (width - maxX)) / 2.0f / width * targetSize.x);
	float newtargetX = ((float)((int)((targetX + 0.005) * 100))) / 100;
	float targetY = ((minY - (height - maxY)) / 2.0f / height * targetSize.y);
	float newtargetY = ((float)((int)((targetY + 0.005) * 100))) / 100;
	imageTargetPos.x = newtargetX;
	imageTargetPos.y = newtargetY;
	// 获得区域的颜色数据
	char* newBitmap = new char[(maxX - minX) * 4 * (maxY - minY)];
	for (int y = 0; y < imageNewSize.y; ++y)
	{
		for (int x = 0; x < imageNewSize.x; ++x)
		{
			RGBQUAD pixel;
			FreeImage_GetPixelColor(bitmap, x + minX, y + minY, &pixel);
			int pixelIndex = x * 4 + y * imageNewSize.x * 4;
			newBitmap[pixelIndex + 0] = pixel.rgbRed;
			newBitmap[pixelIndex + 1] = pixel.rgbGreen;
			newBitmap[pixelIndex + 2] = pixel.rgbBlue;
			newBitmap[pixelIndex + 3] = pixel.rgbReserved;
		}
	}
	FreeImage_Unload(bitmap);
	// 保存新的图片
	FIBITMAP* bitmapNew = FreeImage_Allocate(imageNewSize.x, imageNewSize.y, 32);
	for (int y = 0; y < imageNewSize.y; ++y)
	{
		BYTE* bits = FreeImage_GetScanLine(bitmapNew, y);
		for (int x = 0; x < imageNewSize.x; ++x)
		{
			bits[x * 4 + 0] = newBitmap[y * imageNewSize.x * 4 + 4 * x + 2];
			bits[x * 4 + 1] = newBitmap[y * imageNewSize.x * 4 + 4 * x + 1];
			bits[x * 4 + 2] = newBitmap[y * imageNewSize.x * 4 + 4 * x + 0];
			bits[x * 4 + 3] = newBitmap[y * imageNewSize.x * 4 + 4 * x + 3];
		}
	}
	FreeImage_Save(FIF_PNG, bitmapNew, src.c_str());
	FreeImage_Unload(bitmapNew);
	delete[] newBitmap;
	FreeImage_DeInitialise();
	imageTargetSize.x = (int)((float)imageNewSize.x / width * targetSize.x);
	imageTargetSize.y = (int)((float)imageNewSize.y / height * targetSize.y);
}
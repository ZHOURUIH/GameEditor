#include <windows.h>
#include <io.h>
#include <direct.h>
#include <cctype>
#include <algorithm>

#include "txUtility.h"

void txUtility::findFiles(const char * pathName, std::vector<std::string>& files, std::vector<std::string> patterns, bool recursive)
{
	WIN32_FIND_DATAA FindFileData;
	HANDLE hFind = INVALID_HANDLE_VALUE;

	std::string path(pathName);
	if (path.length() == 0)
	{
		return;
	}
	// 不以/结尾,则加上/
	if (path[path.length() - 1] != '/')
	{
		path += "/";
	}

	hFind = FindFirstFileA((path + "*").c_str(), &FindFileData);
	// 如果找不到文件夹就直接返回
	if (INVALID_HANDLE_VALUE == hFind)
	{
		return;
	}
	while (::FindNextFileA(hFind, &FindFileData))
	{
		// 过滤.和..
		if (strcmp(FindFileData.cFileName, ".") == 0
			|| strcmp(FindFileData.cFileName, "..") == 0)
		{
			continue;
		}

		// 构造完整路径
		std::string fullname = path + std::string(FindFileData.cFileName);
		if (FindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
		{
			if (recursive)
			{
				findFiles(fullname.c_str(), files, patterns, recursive);
			}
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
						files.push_back(fullname);
					}
				}
			}
			else
			{
				files.push_back(fullname);
			}
		}
	}
	::FindClose(hFind);
}

void txUtility::deleteFolder(std::string path)
{
	WIN32_FIND_DATAA FindData;
	// 构造路径
	std::string pathName = path + "/*.*";
	HANDLE hFind = FindFirstFileA(pathName.c_str(), &FindData);
	if (hFind == INVALID_HANDLE_VALUE)
	{
		return;
	}
	while (::FindNextFileA(hFind, &FindData))
	{
		// 过虑.和..
		if (strcmp(FindData.cFileName, ".") == 0
			|| strcmp(FindData.cFileName, "..") == 0)
		{
			continue;
		}

		// 构造完整路径
		std::string fullname = path + "/" + std::string(FindData.cFileName);
		// 如果是文件夹,则递归删除文件夹
		if (FindData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
		{
			deleteFolder(fullname);
		}
		// 如果是文件,则直接删除文件
		else
		{
			DeleteFileA(fullname.c_str());
		}
	}
	::FindClose(hFind);
	// 删除文件夹自身
	BOOL ret = RemoveDirectoryA(path.c_str());
}

bool txUtility::isFileExist(std::string fullPath)
{
	int ret = _access(fullPath.c_str(), 0);
	return ret == 0;
}

void txUtility::swapPixelBuffer(char* buffer, int width, int height, int blockByte)
{
	for (int i = 0; i < height / 2; ++i)
	{
		for (int j = 0; j < width; ++j)
		{
			for (int k = 0; k < blockByte; ++k)
			{
				char temp = buffer[i * width * blockByte + j * blockByte + k];
				buffer[i * width * blockByte + j * blockByte + k] = buffer[(height - 1 - i) * width * blockByte + (width - 1 - j) * blockByte + k];
				buffer[(height - 1 - i) * width * blockByte + (width - 1 - j) * blockByte + k] = temp;
			}
		}
	}

	for (int i = 0; i < height; ++i)
	{
		for (int j = 0; j < width / 2; ++j)
		{
			for (int k = 0; k < blockByte; ++k)
			{
				char temp = buffer[i * width * blockByte + j * blockByte + k];
				buffer[i * width * blockByte + j * blockByte + k] = buffer[i * width * blockByte + (width - 1 - j) * blockByte + k];
				buffer[i * width * blockByte + (width - 1 - j) * blockByte + k] = temp;
			}
		}
	}
}

void txUtility::upsideDownBuffer(char* buffer, int bufferWidth, int startX, int startY, int width, int height, int blockByte)
{
	for (int i = 0; i < height / 2; ++i)
	{
		for (int j = 0; j < width; ++j)
		{
			for (int k = 0; k < blockByte; ++k)
			{
				char temp = buffer[(i + startY) * bufferWidth * blockByte + (j + startX) * blockByte + k];
				buffer[(i + startY) * bufferWidth * blockByte + (j + startX) * blockByte + k] = buffer[(height + startY - 1 - i) * bufferWidth * blockByte + (j + startX) * blockByte + k];
				buffer[(height + startY - 1 - i) * bufferWidth * blockByte + (j + startX) * blockByte + k] = temp;
			}
		}
	}
}

void txUtility::YUVToRGBA(char* YUVData, int width, int height, char* RGBAData)
{
	char Y, U, V;
	for (int i = 0; i < height; ++i)
	{
		for (int j = 0; j < width; ++j)
		{
			Y = YUVData[(i * width + j) * 3 + 0];
			U = YUVData[(i * width + j) * 3 + 1];
			V = YUVData[(i * width + j) * 3 + 2];
			RGBAData[(i * width + j) * 4 + 0] = (char)(Y + 1.14f * V);
			RGBAData[(i * width + j) * 4 + 1] = (char)(Y - 0.394f * U - 0.581f * V);
			RGBAData[(i * width + j) * 4 + 2] = (char)(Y + 2.032f * U);
			RGBAData[(i * width + j) * 4 + 3] = (char)255;
		}
	}
}

void txUtility::RGBAToYUV(char* RGBAData, int width, int height, char* YUVData)
{
	char R, G, B;
	for (int i = 0; i < height; ++i)
	{
		for (int j = 0; j < width; ++j)
		{
			R = RGBAData[(i * width + j) * 4 + 0];
			G = RGBAData[(i * width + j) * 4 + 1];
			B = RGBAData[(i * width + j) * 4 + 2];
			YUVData[(i * width + j) * 3 + 0] = (char)(0.299f * R + 0.587f * G + 0.114f * B);
			YUVData[(i * width + j) * 3 + 1] = (char)(-0.147f * R - 0.289f * G + 0.436f * B);
			YUVData[(i * width + j) * 3 + 2] = (char)(0.615f * R - 0.515f * G - 0.100f * B);
		}
	}
}

void txUtility::YUV420PToRGB32(unsigned char* yuv420[3], unsigned char* rgb32, int width, int height)
{
	int R, G, B, Y, U, V;
	int nWidth = width >> 1; //色度信号宽度
	for (int y = 0; y < height; ++y)
	{
		for (int x = 0; x < width; ++x)
		{
			Y = (int)*(yuv420[0] + y * width + x);
			U = (int)*(yuv420[1] + ((y >> 1) * nWidth) + (x >> 1));
			V = (int)*(yuv420[2] + ((y >> 1) * nWidth) + (x >> 1));
			R = (int)(Y + 1.402f * (V - 128.0f));
			G = (int)(Y - 0.34414f * (U - 128.0f) - 0.71414f * (V - 128.0f));
			B = (int)(Y + 1.772f * (U - 128.0f));

			//防止越界
			limitWithin0To255(R);
			limitWithin0To255(G);
			limitWithin0To255(B);

			*(rgb32 + ((height - y - 1) * width + x) * 4 + 0) = (char)R;
			*(rgb32 + ((height - y - 1) * width + x) * 4 + 1) = (char)G;
			*(rgb32 + ((height - y - 1) * width + x) * 4 + 2) = (char)B;
			*(rgb32 + ((height - y - 1) * width + x) * 4 + 3) = (char)255;
		}
	}
}

void txUtility::split(std::string str, const char* deli, std::vector<std::string>* vec)
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

std::string txUtility::strReplace(std::string str, int begin, int end, std::string reStr)
{
	std::string sub1 = str.substr(0, begin);
	std::string sub2 = str.substr(end, str.length() - end);
	return sub1 + reStr + sub2;
}

float txUtility::calculateFloat(std::string str)
{
	// 判断字符串是否含有非法字符,也就是除数字,小数点,运算符以外的字符
	for (int i = 0; i < (int)str.length();)
	{
		if ((str[i] < '0' || str[i] > '9') && str[i] != '.' && str[i] != '+'
			&& str[i] != '-' && str[i] != '*' && str[i] != '/' && str[i] != '(' && str[i] != ')')
		{
			str.erase(i, 1);
		}
		else
		{
			++i;
		}
	}
	// 判断左右括号数量是否相等
	int leftBracketCount = 0;
	int rightBracketCount = 0;
	for (int i = 0; i < (int)str.length(); ++i)
	{
		if (str[i] == '(')
		{
			++leftBracketCount;
		}
		else if (str[i] == ')')
		{
			++rightBracketCount;
		}
	}
	if (leftBracketCount != rightBracketCount)
	{
		// 计算错误,左右括号数量不对应
		return 0;
	}

	// 循环判断传入的字符串有没有括号
	while (true)
	{
		// 先判断有没有括号，如果有括号就先算括号里的,如果没有就退出while循环
		if (str.find_first_of("(") != -1 || str.find_first_of(")") != -1)
		{
			int curpos = str.find_last_of("(");
			std::string strInBracket = str.substr(curpos + 1, str.length() - curpos - 1);
			strInBracket = strInBracket.substr(0, strInBracket.find_first_of(")"));
			float ret = calculateFloat(strInBracket);
			// 如果括号中的计算结果是负数,则标记为负数
			bool isMinus = false;
			if (ret < 0)
			{
				ret = -ret;
				isMinus = true;
			}
			// 将括号中的计算结果替换原来的表达式,包括括号也一起替换
			std::string floatStr = floatToString(ret, 4);
			str = strReplace(str, curpos, curpos + strInBracket.length() + 2, floatStr);
			if (isMinus)
			{
				// 如果括号中计算出来是负数,则将负号提取出来,将左边的第一个加减号改为相反的符号
				bool changeMark = false;
				for (int i = curpos - 1; i >= 0; --i)
				{
					// 找到第一个+号,则直接改为减号,然后退出遍历
					if (str[i] == '+')
					{
						str[i] = '-';
						changeMark = true;
						break;
					}
					// 找到第一个减号,如果减号的左边有数字,则直接改为+号
					// 如果减号的左边不是数字,则该减号是负号,将减号去掉,
					else if (str[i] == '-')
					{
						if (str[i - 1] >= '0' && str[i - 1] <= '9')
						{
							str[i] = '+';
						}
						else
						{
							str = strReplace(str, i, i + 1, "");
						}
						changeMark = true;
						break;
					}
				}
				// 如果遍历完了还没有找到可以替换的符号,则在表达式最前面加一个负号
				if (!changeMark)
				{
					str = "-" + str;
				}
			}
		}
		else
		{
			break;
		}
	}

	std::vector<float> numbers;
	std::vector<char> factors;
	// 表示上一个运算符的下标+1
	int beginpos = 0;
	for (int i = 0; i < (int)str.length(); ++i)
	{
		// 遍历到了最后一个字符,则直接把最后一个数字放入列表,然后退出循环
		if (i == str.length() - 1)
		{
			std::string num = str.substr(beginpos, str.length() - beginpos);
			float fNum = stringToFloat(num);
			numbers.push_back(fNum);
			break;
		}
		// 找到第一个运算符
		if ((str[i] < '0' || str[i] > '9') && str[i] != '.')
		{
			if (i != 0)
			{
				std::string num = str.substr(beginpos, i);
				float fNum = stringToFloat(num);
				numbers.push_back(fNum);
			}
			// 如果在表达式的开始就发现了运算符,则表示第一个数是负数,那就处理为0减去这个数的绝对值
			else
			{
				numbers.push_back(0);
			}
			factors.push_back(str[i]);
			beginpos = i + 1;
		}
	}
	if (factors.size() + 1 != numbers.size())
	{
		// 计算错误,运算符与数字数量不符
		return 0;
	}
	// 现在开始计算表达式,按照运算优先级,先计算乘除和取余
	while (true)
	{
		// 表示是否还有乘除表达式
		bool hasMS = false;
		for (int i = 0; i < (int)factors.size(); ++i)
		{
			// 先遍历到哪个就先计算哪个
			if (factors[i] == '*' || factors[i] == '/')
			{
				// 第一个运算数的下标与运算符的下标是相同的
				float num1 = numbers[i];
				float num2 = numbers[i + 1];
				float num3 = 0.0f;
				if (factors[i] == '*')
				{
					num3 = num1 * num2;
				}
				else if (factors[i] == '/')
				{
					num3 = num1 / num2;
				}
				// 删除第i + 1个数,然后将第i个数替换为计算结果
				numbers.erase(numbers.begin() + i + 1);
				if (numbers.size() == 0)
				{
					// 计算错误
					return 0;
				}
				numbers[i] = num3;
				// 删除第i个运算符
				factors.erase(factors.begin() + i);
				hasMS = true;
				break;
			}
		}
		if (!hasMS)
		{
			break;
		}
	}
	// 再计算加减法
	while (true)
	{
		if (factors.size() == 0)
		{
			break;
		}
		if (factors[0] == '+' || factors[0] == '-')
		{
			// 第一个运算数的下标与运算符的下标是相同的
			float num1 = numbers[0];
			float num2 = numbers[1];
			float num3 = 0.0f;
			if (factors[0] == '+')
			{
				num3 = num1 + num2;
			}
			else if (factors[0] == '-')
			{
				num3 = num1 - num2;
			}
			// 删除第1个数,然后将第0个数替换为计算结果
			numbers.erase(numbers.begin() + 1);
			if (numbers.size() == 0)
			{
				// 计算错误
				return 0;
			}
			numbers[0] = num3;
			// 删除第0个运算符
			factors.erase(factors.begin());
		}
	}
	if (numbers.size() != 1)
	{
		// 计算错误
		return 0;
	}
	else
	{
		return numbers[0];
	}
}

int txUtility::calculateInt(std::string str)
{
	// 判断字符串是否含有非法字符,也就是除数字,小数点,运算符以外的字符
	for (int i = 0; i < (int)str.length(); )
	{
		if ((str[i] < '0' || str[i] > '9') && str[i] != '.' && str[i] != '+'
			&& str[i] != '-' && str[i] != '*' && str[i] != '/' && str[i] != '%' 
			&& str[i] != '(' && str[i] != ')')
		{
			str.erase(i, 1);
		}
		else
		{
			++i;
		}
	}
	// 判断左右括号数量是否相等
	int leftBracketCount = 0;
	int rightBracketCount = 0;
	for (int i = 0; i < (int)str.length(); ++i)
	{
		if (str[i] == '(')
		{
			++leftBracketCount;
		}
		else if (str[i] == ')')
		{
			++rightBracketCount;
		}
	}
	if (leftBracketCount != rightBracketCount)
	{
		// 计算错误,左右括号数量不对应
		return 0;
	}

	// 循环判断传入的字符串有没有括号
	while (true)
	{
		// 先判断有没有括号，如果有括号就先算括号里的,如果没有就退出while循环
		if (str.find_first_of("(") != -1 || str.find_first_of(")") != -1)
		{
			int curpos = str.find_last_of("(");
			std::string strInBracket = str.substr(curpos + 1, str.length() - curpos - 1);
			strInBracket = strInBracket.substr(0, strInBracket.find_first_of(")"));
			int ret = calculateInt(strInBracket);
			// 如果括号中的计算结果是负数,则标记为负数
			bool isMinus = false;
			if (ret < 0)
			{
				ret = -ret;
				isMinus = true;
			}
			// 将括号中的计算结果替换原来的表达式,包括括号也一起替换
			std::string intStr = intToString(ret, 4);
			str = strReplace(str, curpos, curpos + strInBracket.length() + 2, intStr);
			if (isMinus)
			{
				// 如果括号中计算出来是负数,则将负号提取出来,将左边的第一个加减号改为相反的符号
				bool changeMark = false;
				for (int i = curpos - 1; i >= 0; --i)
				{
					// 找到第一个+号,则直接改为减号,然后退出遍历
					if (str[i] == '+')
					{
						str[i] = '-';
						changeMark = true;
						break;
					}
					// 找到第一个减号,如果减号的左边有数字,则直接改为+号
					// 如果减号的左边不是数字,则该减号是负号,将减号去掉,
					else if (str[i] == '-')
					{
						if (str[i - 1] >= '0' && str[i - 1] <= '9')
						{
							str[i] = '+';
						}
						else
						{
							str = strReplace(str, i, i + 1, "");
						}
						changeMark = true;
						break;
					}
				}
				// 如果遍历完了还没有找到可以替换的符号,则在表达式最前面加一个负号
				if (!changeMark)
				{
					str = "-" + str;
				}
			}
		}
		else
		{
			break;
		}
	}

	std::vector<int> numbers;
	std::vector<char> factors;
	// 表示上一个运算符的下标+1
	int beginpos = 0;
	for (int i = 0; i < (int)str.length(); ++i)
	{
		// 遍历到了最后一个字符,则直接把最后一个数字放入列表,然后退出循环
		if (i == str.length() - 1)
		{
			std::string num = str.substr(beginpos, str.length() - beginpos);
			int iNum = stringToInt(num);
			numbers.push_back(iNum);
			break;
		}
		// 找到第一个运算符
		if ((str[i] < '0' || str[i] > '9') && str[i] != '.')
		{
			if (i != 0)
			{
				std::string num = str.substr(beginpos, i);
				int iNum = stringToInt(num);
				numbers.push_back(iNum);
			}
			// 如果在表达式的开始就发现了运算符,则表示第一个数是负数,那就处理为0减去这个数的绝对值
			else
			{
				numbers.push_back(0);
			}
			factors.push_back(str[i]);
			beginpos = i + 1;
		}
	}
	if (factors.size() + 1 != numbers.size())
	{
		// 计算错误,运算符与数字数量不符
		return 0;
	}
	// 现在开始计算表达式,按照运算优先级,先计算乘除和取余
	while (true)
	{
		// 表示是否还有乘除表达式
		bool hasMS = false;
		for (int i = 0; i < (int)factors.size(); ++i)
		{
			// 先遍历到哪个就先计算哪个
			if (factors[i] == '*' || factors[i] == '/' || factors[i] == '%')
			{
				// 第一个运算数的下标与运算符的下标是相同的
				int num1 = numbers[i];
				int num2 = numbers[i + 1];
				int num3 = 0;
				if (factors[i] == '*')
				{
					num3 = num1 * num2;
				}
				else if (factors[i] == '/')
				{
					num3 = num1 / num2;
				}
				else if (factors[i] == '%')
				{
					num3 = num1 % num2;
				}
				// 删除第i + 1个数,然后将第i个数替换为计算结果
				numbers.erase(numbers.begin() + i + 1);
				if (numbers.size() == 0)
				{
					// 计算错误
					return 0;
				}
				numbers[i] = num3;
				// 删除第i个运算符
				factors.erase(factors.begin() + i);
				hasMS = true;
				break;
			}
		}
		if (!hasMS)
		{
			break;
		}
	}
	// 再计算加减法
	while (true)
	{
		if (factors.size() == 0)
		{
			break;
		}
		if (factors[0] == '+' || factors[0] == '-')
		{
			// 第一个运算数的下标与运算符的下标是相同的
			int num1 = numbers[0];
			int num2 = numbers[1];
			int num3 = 0;
			if (factors[0] == '+')
			{
				num3 = num1 + num2;
			}
			else if (factors[0] == '-')
			{
				num3 = num1 - num2;
			}
			// 删除第1个数,然后将第0个数替换为计算结果
			numbers.erase(numbers.begin() + 1);
			if (numbers.size() == 0)
			{
				// 计算错误
				return 0;
			}
			numbers[0] = num3;
			// 删除第0个运算符
			factors.erase(factors.begin());
		}
	}
	if (numbers.size() != 1)
	{
		// 计算错误
		return 0;
	}
	else
	{
		return numbers[0];
	}
}

std::string txUtility::intToString(int i, int limitLen)
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

float txUtility::pow_f(float f, int p)
{
	float ret = 1.0f;
	while (p)
	{
		ret *= f;
		--p;
	}
	return ret;
}

std::string txUtility::floatToString(float f, int precision)
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

std::wstring txUtility::ANSIToUnicode(const std::string& str)
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

std::string txUtility::UnicodeToANSI(const std::wstring& str)
{
	int iTextLen = WideCharToMultiByte(CP_ACP, 0, str.c_str(), -1, NULL, 0, NULL, NULL);
	char* pElementText = TRACE_NEW_ARRAY(char, iTextLen + 1, pElementText);
	memset((void*)pElementText, 0, sizeof(char) * (iTextLen + 1));
	::WideCharToMultiByte(CP_ACP, 0, str.c_str(), -1, pElementText, iTextLen, NULL, NULL);
	std::string strText = pElementText;
	TRACE_DELETE_ARRAY(pElementText);
	return strText;
}
std::string txUtility::UnicodeToUTF8(const std::wstring& str)
 {
	// wide char to multi char
	int iTextLen = WideCharToMultiByte(CP_UTF8, 0, str.c_str(), -1, NULL, 0, NULL, NULL);
	char* pElementText = TRACE_NEW_ARRAY(char, iTextLen + 1, pElementText);
	memset((void*)pElementText, 0, sizeof(char) * (iTextLen + 1));
	::WideCharToMultiByte(CP_UTF8, 0, str.c_str(), -1, pElementText, iTextLen, NULL, NULL);
	std::string strText = pElementText;
	TRACE_DELETE_ARRAY(pElementText);
	return strText;
 }

std::wstring txUtility::UTF8ToUnicode(const std::string& str)
 {
	 int len = str.length();
	 int unicodeLen = ::MultiByteToWideChar(CP_UTF8, 0, str.c_str(), -1, NULL, 0);
	 wchar_t* pUnicode = TRACE_NEW_ARRAY(wchar_t, unicodeLen + 1, pUnicode);
	 memset(pUnicode, 0, (unicodeLen + 1)*sizeof(wchar_t));
	 ::MultiByteToWideChar(CP_UTF8, 0, str.c_str(), -1, (LPWSTR)pUnicode, unicodeLen);
	 std::wstring rt = (wchar_t*)pUnicode;
	 TRACE_DELETE_ARRAY(pUnicode);
	 return rt;
}

std::string txUtility::ANSIToUTF8(const std::string& str)
{
	std::wstring unicodeStr = ANSIToUnicode(str);
	std::string utf8Str = UnicodeToUTF8(unicodeStr);
	return utf8Str;
}

std::string txUtility::UTF8ToANSI(const std::string& str)
{
	std::wstring unicodeStr = UTF8ToUnicode(str);
	std::string ansiStr = UnicodeToANSI(unicodeStr);
	return ansiStr;
}

bool txUtility::copyFile(std::string sourceFile, std::string destFile, bool overWrite)
{
	// 如果目标文件所在的目录不存在,则先创建目录
	std::string parentDir = getParentDir(destFile);
	createFolder(parentDir);
#if RUN_PLATFORM == PLATFORM_WINDOWS
	return CopyFileA(sourceFile.c_str(), destFile.c_str(), !overWrite) == TRUE;
#elif RUN_PLATFORM == PLATFORM_ANDROID
	return false;
#endif
}

bool txUtility::createFolder(std::string path)
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

bool txUtility::writeFile(std::string filePath, int length, const char* buffer)
{
	if (length <= 0 || NULL == buffer)
	{
		return false;
	}

	txUtility::rightToLeft(filePath);

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
int txUtility::strFind(std::string res, std::string sub, int startPose, bool fromStart)
{
	int searchLength = res.length() - sub.length();
	
	if (fromStart)
	{
		int j = 0;
		for (int i = startPose; i < searchLength; ++i)
		{
			for (j = 0; j != sub.length(); ++j)
			{
				if (res[i + j] != sub[j])
				{
					break;
				}
			}

			if (j == sub.length())
			{
				return i;
			}
		}
	}
	else
	{
		int j = 0;
		for (int i = searchLength; i != startPose; --i)
		{
			for (j = 0; j != sub.length(); ++j)
			{
				if (res[i + j] != sub[j])
				{
					break;
				}
			}

			if (j == sub.length())
			{
				return i;
			}
		}
	}

	return -1;
}

bool txUtility::findSubstr(std::string res, std::string dst, bool sensitive, int* pos, int startPose, bool firstOrLast)
{
	// 如果不区分大小写
	if (!sensitive)
	{
		// 全转换为小写
		strToLower(res);
		strToLower(dst);
	}
	int posFind = strFind(res, dst, startPose, firstOrLast);

	if (NULL != pos)
	{
		*pos = posFind;
	}
	if (-1 != posFind)
	{
		return true;
	}

	return false;
}

void txUtility::strToLower(std::string& str)
{
	std::string::size_type size = str.length();
	for (std::string::size_type i = 0; i != size; ++i)
	{
		if (str[i] >= 'A' && str[i] <= 'Z')
		{
			str[i] += 'a' - 'A';
		}
	}
}

void txUtility::strToUpper(std::string& str)
{
	std::string::size_type size = str.length();
	for (std::string::size_type i = 0; i != size; ++i)
	{
		if (str[i] >= 'a' && str[i] <= 'z')
		{
			str[i] -= 'a' - 'A';
		}
	}
}

std::string txUtility::charToHex(unsigned char byte, bool upper)
{
	char byteHex[3] = { 0 };
	char* charPool;
	if (upper)
	{
		charPool = "ABCDEF";
	}
	else
	{
		charPool = "abcdef";
	}
	unsigned char highBit = byte >> 4;
	// 高字节的十六进制
	if (highBit < (unsigned char)10)
	{
		byteHex[0] = '0' + highBit;
	}
	else
	{
		byteHex[0] = charPool[highBit - 10];
	}
	// 低字节的十六进制
	unsigned char lowBit = byte & 0x0F;
	if (lowBit < (unsigned char)10)
	{
		byteHex[1] = '0' + lowBit;
	}
	else
	{
		byteHex[1] = charPool[lowBit - 10];
	}
	return byteHex;
}

std::string txUtility::charArrayToHexString(unsigned char* data, const int& dataCount, bool addSpace, bool upper)
{
	int oneLength = addSpace ? 3 : 2;
	int showCount = dataCount * oneLength + 1;
	char* byteData = TRACE_NEW_ARRAY(char, showCount, byteData);
	memset(byteData, 0, showCount);
	for (int j = 0; j < dataCount; ++j)
	{
		unsigned char curByte = data[j];
		std::string byteStr = charToHex(curByte);
		byteData[j * oneLength + 0] = byteStr[0];
		byteData[j * oneLength + 1] = byteStr[1];
		if (oneLength >= 3)
		{
			byteData[j * oneLength + 2] = ' ';
		}
	}
	std::string str(byteData);
	TRACE_DELETE_ARRAY(byteData);
	return str;
}
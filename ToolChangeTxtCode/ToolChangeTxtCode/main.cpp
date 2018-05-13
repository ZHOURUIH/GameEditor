// ToolChangeTxtCode.cpp : 定义控制台应用程序的入口点。
//
#include <iostream>

#include "Utility.h"

std::string getTypeStr(CODE_TYPE type)
{
	switch (type)
	{
	case CT_ANSI:
		return "ANSI";
		break;
	case CT_UTF8:
		return "UTF8";
		break;
	case CT_UNICODE:
		return  "UNICODE";
		break;
	case CT_MAX:
		break;
	default:
		break;
	}
	return "";
}

int main(int argc, char* argv[])
{
	std::cout << "请输入要转换为什么类型 : 0 == ANSI, 1== UTF8, 2 == UNICODE" << std::endl;
	int input = -1;
	while (input < 0 || input > 2)
	{
		std::cin >> input;
	}
	CODE_TYPE targetType = (CODE_TYPE)(input);
	std::vector<std::string> files;
	std::vector<std::string> patterns;
	patterns.push_back(".h");
	patterns.push_back(".cpp");
	Utility::findFiles("./", files, patterns);

	int fileCount = files.size();
	for (int i = 0; i < fileCount; ++i)
	{
		char* buffer = NULL;
		int fileLength = 0;
		Utility::readFile(files[i], buffer, fileLength);
		if (buffer != NULL)
		{
			// 判断文件类型
			CODE_TYPE sourceType = Utility::getFileCodeType(buffer, fileLength);
			if (sourceType == CT_MAX)
			{
				std::cout << files[i] << "转换失败 : 无法获取源文件编码格式" << std::endl;
			}
			else if (sourceType != targetType)
			{
				std::cout << "原文件名:" << files[i] << ",原文件类型:" << getTypeStr(sourceType) << "转换为 : " << getTypeStr(targetType) << std::endl;
				if (sourceType == CT_ANSI)
				{
					if (targetType == CT_UTF8)
					{
						int strLength;
						char* utf8Buffer = Utility::ANSIToUTF8(buffer, strLength);
						int writeBufferByte;
						char* bomBuffer = Utility::addUTF8BOM(utf8Buffer, strLength, writeBufferByte);
						Utility::writeFile(files[i], writeBufferByte, bomBuffer);

						delete[] utf8Buffer;
						delete[] bomBuffer;
					}
					else if (targetType == CT_UNICODE)
					{
						int length;
						wchar_t* unicodeBuffer = Utility::ANSIToUnicode(buffer, length);
						int writeBufferByte;
						wchar_t* bomBuffer = Utility::addUNICODEBOM(unicodeBuffer, length, writeBufferByte);
						Utility::writeFile(files[i], writeBufferByte, (char*)bomBuffer);

						delete[] unicodeBuffer;
						delete[] bomBuffer;
					}
				}
				else if (sourceType == CT_UTF8)
				{
					int dBomBufferByte;
					char* dBomBuffer = Utility::deleteUTF8BOM(buffer, fileLength, dBomBufferByte, true);
					if (targetType == CT_ANSI)
					{
						int strLength;
						char* ansiBuffer = Utility::UTF8ToANSI(dBomBuffer, strLength);
						Utility::writeFile(files[i], strLength * sizeof(char), ansiBuffer);

						delete[] ansiBuffer;
					}
					else if (targetType == CT_UNICODE)
					{
						int strLength;
						wchar_t* unicodeBuffer = Utility::UTF8ToUnicode(dBomBuffer, strLength);
						int writeBufferByte;
						wchar_t* bomBuffer = Utility::addUNICODEBOM(unicodeBuffer, strLength, writeBufferByte);
						Utility::writeFile(files[i], writeBufferByte, (char*)bomBuffer);

						delete[] unicodeBuffer;
						delete[] bomBuffer;
					}
					delete[] dBomBuffer;
				}
				else if (sourceType == CT_UNICODE)
				{
					wchar_t* endNullBuffer = (wchar_t*)new char[fileLength + sizeof(wchar_t)];
					memset(endNullBuffer, 0, fileLength + sizeof(wchar_t));
					memcpy(endNullBuffer, buffer, fileLength);
					int dBomBufferByte;
					wchar_t* dBomBuffer = Utility::deleteUNICODEBOM((char*)endNullBuffer, fileLength, dBomBufferByte, true);
					if (targetType == CT_ANSI)
					{
						int strLength;
						char* ansiBuffer = Utility::UnicodeToANSI(dBomBuffer, strLength);
						Utility::writeFile(files[i], strLength, ansiBuffer);
						delete[] ansiBuffer;
					}
					else if (targetType == CT_UTF8)
					{
						int strLength;
						char* utf8Buffer = Utility::UnicodeToUTF8(dBomBuffer, strLength);
						int writeBufferByte;
						char* bomBuffer = Utility::addUTF8BOM(utf8Buffer, strLength, writeBufferByte);
						Utility::writeFile(files[i], writeBufferByte, bomBuffer);
						delete[] utf8Buffer;
						delete[] bomBuffer;
					}
					delete[] dBomBuffer;
				}
			}
			delete[] buffer;
		}
	}

	system("pause");
	return 0;
}


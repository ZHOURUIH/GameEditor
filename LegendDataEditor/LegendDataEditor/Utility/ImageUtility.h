#ifndef _IMAGE_UTILITY_H_
#define _IMAGE_UTILITY_H_

#include "ImageDefine.h"
#include "FreeImage.h"

class SQLite;
class ImageUtility
{
public:
	static void encodePNG(const std::string& path, char* color, int width, int height, FREE_IMAGE_FORMAT format);
	static void readWixFile(const std::string& filePath, WIXFileImageInfo& info);
	static void readWilHeader(const std::string& filePath, WILFileHeader& header);
	static void wixWilToPNG(const std::string& wixFileName, const std::string& wilFileName, const std::string& outputPath);
	static void writePositionFile(const std::string& positionFile, POINT* posList, int posCount);
	static POINT* readPositionFile(const std::string& positionFile, int& posCount);
	static void groupImage(const std::string& path, IMAGE_TYPE imageType, SQLite* sqlite);
};

#endif
#include "Utility.h"
#include "txSerializer.h"
#include "HumanImage.h"
#include "SQLite.h"
#include "SQLiteCloth.h"
#include "SQLiteClothFrame.h"
#include "SQLiteMonster.h"
#include "SQLiteMonsterFrame.h"
#include "SQLiteWeapon.h"
#include "SQLiteWeaponFrame.h"
#include "HumanAction.h"

void ImageUtility::encodePNG(const std::string& path, char* color, int width, int height, FREE_IMAGE_FORMAT format)
{
	std::string dir = StringUtility::getFilePath(path);
	FileUtility::createFolder(dir);
	FreeImage_Initialise();
	FIBITMAP* bitmap = FreeImage_Allocate(width, height, 32);
	BYTE* bits = FreeImage_GetBits(bitmap);
	memcpy(bits, color, width * height * 4);
	FreeImage_Save(format, bitmap, path.c_str());
	FreeImage_Unload(bitmap);
	FreeImage_DeInitialise();
}

void ImageUtility::readWixFile(const std::string& filePath, WIXFileImageInfo& info)
{
	int fileSize = 0;
	char* fileBuffer = FileUtility::openBinaryFile(filePath, &fileSize);
	if (fileBuffer == NULL)
	{
		return;
	}
	txSerializer serializer(fileBuffer, fileSize);
	serializer.readBuffer(info.mStrHeader, 44);
	serializer.read(info.mIndexCount);
	for (int i = 0; i < info.mIndexCount; ++i)
	{
		int curStartPos = 0;
		serializer.read(curStartPos);
		info.mPositionList.push_back(curStartPos);
	}
	delete[] fileBuffer;
}

void ImageUtility::readWilHeader(const std::string& filePath, WILFileHeader& header)
{
	int fileSize = 0;
	char* fileBuffer = FileUtility::openBinaryFile(filePath, &fileSize);
	if (fileBuffer == NULL)
	{
		return;
	}
	txSerializer serializer(fileBuffer, fileSize);
	serializer.readBuffer(header.mInfo, 44);
	serializer.readBuffer(header.mPlayInfo, 44);
	for (int i = 0; i < 256; ++i)
	{
		for (int j = 0; j < 4; ++j)
		{
			serializer.read(header.mColor[i][j]);
		}
	}
	delete[] fileBuffer;
}

void ImageUtility::wixWilToPNG(const std::string& wixFileName, const std::string& wilFileName, const std::string& outputPath)
{
	// 打开wix文件
	WIXFileImageInfo wixFileHeader;
	readWixFile(wixFileName, wixFileHeader);

	// 打开wil文件
	WILFileHeader wilHeader;
	readWilHeader(wilFileName, wilHeader);

	POINT* posList = TRACE_NEW_ARRAY(POINT, wixFileHeader.mIndexCount, posList);
	int fileSize = 0;
	char* fileBuffer = FileUtility::openBinaryFile(wilFileName, &fileSize);
	txSerializer serializer(fileBuffer, fileSize);
	for (int i = 0; i < wixFileHeader.mIndexCount; ++i)
	{
		// 将下标设置到当前图片起始位置,并且读取图片信息
		int startPos = wixFileHeader.mPositionList[i];
		serializer.setIndex(startPos);
		WILFileImageInfo curInfo;
		// 宽高,位置偏移
		serializer.read(curInfo.mWidth);
		serializer.read(curInfo.mHeight);
		serializer.read(curInfo.mPosX);
		serializer.read(curInfo.mPosY);
		// 根据颜色索引在调色板中获取颜色数据
		int pixelCount = curInfo.mWidth * curInfo.mHeight;
		curInfo.mColor = new char[pixelCount * 4];
		for (int j = 0; j < pixelCount; ++j)
		{
			unsigned char pixelIndex = fileBuffer[startPos + ImageHeaderLength + j];
			// 0蓝,1绿,2红
			curInfo.mColor[j * 4 + 0] = fileBuffer[ColorPadIndex + pixelIndex * 4 + 0];// 蓝
			curInfo.mColor[j * 4 + 1] = fileBuffer[ColorPadIndex + pixelIndex * 4 + 1];// 绿
			curInfo.mColor[j * 4 + 2] = fileBuffer[ColorPadIndex + pixelIndex * 4 + 2];// 红
			curInfo.mColor[j * 4 + 3] = pixelIndex == 0 ? 0 : (char)255;
		}
		posList[i].x = curInfo.mPosX;
		posList[i].y = curInfo.mPosY;
		// 将图片转换为png
		encodePNG(outputPath + StringUtility::intToString(i) + ".png", curInfo.mColor, curInfo.mWidth, curInfo.mHeight, FIF_PNG);
		delete[] curInfo.mColor;
	}
	delete[] fileBuffer;

	writePositionFile(outputPath + "position.txt", posList, wixFileHeader.mIndexCount);
	TRACE_DELETE_ARRAY(posList);
}

void ImageUtility::writePositionFile(const std::string& positionFile, POINT* posList, int posCount)
{
	std::string posStr;
	for (int i = 0; i < posCount; ++i)
	{
		posStr += StringUtility::intToString(posList[i].x);
		posStr += ",";
		posStr += StringUtility::intToString(posList[i].y);
		posStr += "\n";
	}
	FileUtility::writeFile(positionFile, posStr);
}

POINT* ImageUtility::readPositionFile(const std::string& positionFile, int& posCount)
{
	std::string posStr = FileUtility::openTxtFile(positionFile);
	txVector<std::string> posStrList;
	StringUtility::split(posStr, "\n", posStrList);
	posCount = posStrList.size();
	POINT* posList = TRACE_NEW_ARRAY(POINT, posCount, posList);
	for (int i = 0; i < posCount; ++i)
	{
		txVector<std::string> pointList;
		StringUtility::split(posStrList[i], ",", pointList);
		if (pointList.size() != 2)
		{
			continue;
		}
		posList[i].x = StringUtility::stringToInt(pointList[0]);
		posList[i].y = StringUtility::stringToInt(pointList[1]);
	}
	return posList;
}

void ImageUtility::groupImage(const std::string& path, IMAGE_TYPE imageType, SQLite* sqlite)
{
	txVector<std::string> fileList;
	FileUtility::findFiles(path, fileList, ".png");
	if (fileList.size() % GROUP_SIZE != 0)
	{
		std::cout << "数量错误" << std::endl;
		return;
	}

	// 读取位置偏移列表文件
	int posCount = 0;
	POINT* posList = ImageUtility::readPositionFile(path + "/position.txt", posCount);
	if (posCount != fileList.size())
	{
		std::cout << "位置数量与文件数量不一致" << std::endl;
		TRACE_DELETE_ARRAY(posList);
		return;
	}
	if (imageType == IT_HUMAN)
	{
		HumanImage* imageList = TRACE_NEW_ARRAY(HumanImage, fileList.size(), imageList);
		// 将图片分组
		int groupCount = fileList.size() / GROUP_SIZE;
		for (int i = 0; i < groupCount; ++i)
		{
			ImageGroup imageGroup;
			// 将文件全部移动到该文件夹中,重命名该组下的所有图片,根据动作来命名
			for (int j = 0; j < GROUP_SIZE; ++j)
			{
				int imageIndex = i * GROUP_SIZE + j;
				std::string fileName = path + "/" + StringUtility::intToString(imageIndex) + ".png";
				imageList[imageIndex].mPosX = posList[imageIndex].x;
				imageList[imageIndex].mPosY = posList[imageIndex].y;
				imageList[imageIndex].mClothID = i + 1;
				imageList[imageIndex].mOriginFileName = fileName;
				imageList[imageIndex].setIndexInGroup(j);
				imageList[imageIndex].moveImage(path);
				imageGroup.addImage(imageList[imageIndex]);
			}
			// 按序列帧来分组,整理数据后写入数据库
			// 遍历所有动作
			txMap<std::string, ActionSet>& actionList = imageGroup.mAllAction;
			auto iter = actionList.begin();
			auto iterEnd = actionList.end();
			for (; iter != iterEnd; ++iter)
			{
				// 遍历该动作的所有方向
				for (int j = 0; j < DIRECTION_COUNT; ++j)
				{
					ActionAnim& actionAnim = iter->second.mDirectionAction[j];
					ClothFrameData data;
					data.mID = actionAnim.mHumanImageFrame[0].mClothID;
					data.mDirection = j;
					data.mAction = iter->first;
					data.mFrameCount = actionAnim.mHumanImageFrame.size();
					// 遍历该动作的所有帧数
					for (int kk = 0; kk < data.mFrameCount; ++kk)
					{
						data.mPosX.push_back((float)actionAnim.mHumanImageFrame[kk].mPosX);
						data.mPosY.push_back((float)actionAnim.mHumanImageFrame[kk].mPosY);
					}
					bool ret = sqlite->mSQLiteClothFrame->insertOrUpdate(data);
					if (!ret)
					{
						break;
					}
				}
			}
		}
		TRACE_DELETE_ARRAY(imageList);
	}
	else if(imageType == IT_WEAPON)
	{
		;
	}
	else if (imageType == IT_MONSTER)
	{
		;
	}
	TRACE_DELETE_ARRAY(posList);
}
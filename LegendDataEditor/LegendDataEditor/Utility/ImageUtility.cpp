#include "Utility.h"
#include "txSerializer.h"
#include "HumanImage.h"
#include "WeaponImage.h"
#include "SQLite.h"
#include "SQLiteSceneMap.h"
#include "SQLiteNPC.h"
#include "SQLiteMonGen.h"
#include "SQLiteMonsterInfo.h"
#include "SQLiteMagic.h"
#include "SQLiteStdItem.h"
#include "SQLiteAnimationFrame.h"
#include "SQLiteImagePosition.h"
#include "HumanAction.h"
#include "WeaponAction.h"
#include "SceneMap.h"
#include "MapHeader.h"
#include "MapTile.h"
#include "SceneMapAdvance.h"
#include "MapTileAdvance.h"

void ImageUtility::encodePNG(const string& path, unsigned char* color, int width, int height, FREE_IMAGE_FORMAT format)
{
	string dir = StringUtility::getFilePath(path);
	FileUtility::createFolder(dir);
	FreeImage_Initialise();
	FIBITMAP* bitmap = FreeImage_Allocate(width, height, 32);
	BYTE* bits = FreeImage_GetBits(bitmap);
	memcpy(bits, color, width * height * 4);
	FreeImage_Save(format, bitmap, path.c_str());
	FreeImage_Unload(bitmap);
	FreeImage_DeInitialise();
}

bool ImageUtility::readWixFile(const string& filePath, WIXFileImageInfo& info)
{
	int fileSize = 0;
	char* fileBuffer = FileUtility::openBinaryFile(filePath, &fileSize);
	if (fileBuffer == NULL)
	{
		return false;
	}
	txSerializer serializer(fileBuffer, fileSize);
	serializer.readBuffer(info.mStrHeader, 44);
	serializer.read(info.mIndexCount);
	for (int i = 0; i < info.mIndexCount; ++i)
	{
		int curStartPos = 0;
		if (serializer.read(curStartPos))
		{
			info.mPositionList.push_back(curStartPos);
		}
	}
	TRACE_DELETE_ARRAY(fileBuffer);
	return true;
}

bool ImageUtility::readWilHeader(const string& filePath, WILFileHeader& header)
{
	int fileSize = 0;
	char* fileBuffer = FileUtility::openBinaryFile(filePath, &fileSize);
	if (fileBuffer == NULL)
	{
		return false;
	}
	txSerializer serializer(fileBuffer, fileSize);
	serializer.readBuffer(header.mInfo, 44);
	serializer.read(header.mImageCount);
	serializer.read(header.mColorCount);
	serializer.read(header.mColorPadSize);
	TRACE_NEW_ARRAY(unsigned char, header.mColorPadSize, header.mColor);
	memcpy(header.mColor, serializer.getBuffer() + serializer.getIndex(), header.mColorPadSize);
	TRACE_DELETE_ARRAY(fileBuffer);
	return true;
}

void ImageUtility::wixWilToPNG(const string& wixFileName, const string& wilFileName, const string& outputPath)
{
	// 打开wix文件
	WIXFileImageInfo wixFileHeader;
	if (!readWixFile(wixFileName, wixFileHeader))
	{
		std::cout << "找不到wix文件" << std::endl;
		return;
	}

	// 打开wil文件
	WILFileHeader wilHeader;
	if(!readWilHeader(wilFileName, wilHeader))
	{
		std::cout << "找不到wil文件" << std::endl;
		return;
	}
	// 位图的位数,8位图或者16位图,8位图使用调色板,16位图使用16位表示颜色
	int bpp = 0;
	if (wilHeader.mColorCount == 0xFF + 1)
	{
		bpp = 8;
	}
	else if (wilHeader.mColorCount == 0xFFFF + 1)
	{
		bpp = 16;
	}
	string fileNameNoSuffix = StringUtility::getFileNameNoSuffix(wilFileName);
	std::cout << "正在解析" << fileNameNoSuffix << ", 图片数量 : " << wilHeader.mImageCount << ", 颜色位数 : " << bpp << std::endl;
	POINT* posList = TRACE_NEW_ARRAY(POINT, wixFileHeader.mPositionList.size(), posList);
	int fileSize = 0;
	char* fileBuffer = FileUtility::openBinaryFile(wilFileName, &fileSize);
	txSerializer serializer(fileBuffer, fileSize);
	for (int i = 0; i < wixFileHeader.mPositionList.size(); ++i)
	{
		// 将下标设置到当前图片起始位置,并且读取图片信息
		int startPos = wixFileHeader.mPositionList[i];
		if (startPos == 0)
		{
			continue;
		}
		if (i == 12000)
		{
			int a = 0;
		}
		serializer.setIndex(startPos);
		const char* chrBuf = serializer.getBuffer() + serializer.getIndex();
		WILFileImageInfo curInfo;
		// 宽高,位置偏移
		serializer.read(curInfo.mWidth);
		serializer.read(curInfo.mHeight);
		serializer.read(curInfo.mPosX);
		serializer.read(curInfo.mPosY);
		// 根据颜色索引在调色板中获取颜色数据
		int pixelCount = curInfo.mWidth * curInfo.mHeight;
		int lineLength = curInfo.mWidth;
		// 位图要按4字节对齐,每行的字节数必须为4的倍数,不够的需要补齐
		// 8位图,则需要按4个字节对齐
		if (bpp == 8)
		{
			if (curInfo.mWidth % 4 != 0)
			{
				lineLength += 4 - curInfo.mWidth % 4;
			}
		}
		// 16位图图片宽度必须为2的倍数,不够则需要按2的倍数来读取,否则像素会错位
		else  if (bpp == 16)
		{
			if (curInfo.mWidth % (bpp / 8) != 0)
			{
				lineLength += (bpp / 8) - curInfo.mWidth % (bpp / 8);
			}
		}
		// 固定转换为32位png图片
		TRACE_NEW_ARRAY(unsigned char, pixelCount * 4, curInfo.mColor);
		for (int y = 0; y < curInfo.mHeight; ++y)
		{
			for (int x = 0; x < curInfo.mWidth; ++x)
			{
				int pos = y * lineLength + x;
				int pngPos = y * curInfo.mWidth + x;
				// 8bit
				if (wilHeader.mColorCount == 0xFF + 1)
				{
					unsigned char pixelIndex = fileBuffer[startPos + ImageHeaderLength + pos];
					// 0蓝,1绿,2红
					curInfo.mColor[pngPos * 4 + 0] = wilHeader.mColor[pixelIndex * 4 + 0];
					curInfo.mColor[pngPos * 4 + 1] = wilHeader.mColor[pixelIndex * 4 + 1];
					curInfo.mColor[pngPos * 4 + 2] = wilHeader.mColor[pixelIndex * 4 + 2];
					curInfo.mColor[pngPos * 4 + 3] = pixelIndex == 0 ? 0 : (char)255;
				}
				// 16bit
				else if (wilHeader.mColorCount == 0xFFFF + 1)
				{
					unsigned short pixel = *(unsigned short*)(fileBuffer + startPos + ImageHeaderLength + pos * 2);
					// 将16位转为24位,蓝绿红的顺序
					curInfo.mColor[pngPos * 4 + 0] = (pixel & 0x1F) << 3;
					curInfo.mColor[pngPos * 4 + 1] = pixel >> 3;
					curInfo.mColor[pngPos * 4 + 2] = pixel >> 8;
					curInfo.mColor[pngPos * 4 + 3] = pixel == 0 ? 0 : (char)255;
				}
			}
		}
		posList[i].x = curInfo.mPosX;
		posList[i].y = curInfo.mPosY;
		// 将图片转换为png
		encodePNG(outputPath + StringUtility::intToString(i) + ".png", curInfo.mColor, curInfo.mWidth, curInfo.mHeight, FIF_PNG);
		TRACE_DELETE_ARRAY(curInfo.mColor);
	}
	TRACE_DELETE_ARRAY(fileBuffer);

	writePositionFile(outputPath + "position.txt", posList, wixFileHeader.mPositionList.size());
	TRACE_DELETE_ARRAY(posList);
}

void ImageUtility::allWixWilToPNG(const string& sourcePath)
{
	txVector<string> fileList;
	txVector<string> patterns;
	patterns.push_back(".wil");
	FileUtility::findFiles(sourcePath, fileList, patterns);
	int fileCount = fileList.size();
	for (int i = 0; i < fileCount; ++i)
	{
		string wixFile = StringUtility::getFileNameNoSuffix(fileList[i], false) + ".wix";
		string filePath = StringUtility::getFilePath(fileList[i]);
		string folderName = StringUtility::getFileNameNoSuffix(fileList[i]);
		wixWilToPNG(wixFile, fileList[i], filePath + "/" + folderName + "/");
	}
}

void ImageUtility::writePositionFile(const string& positionFile, POINT* posList, int posCount)
{
	string posStr;
	for (int i = 0; i < posCount; ++i)
	{
		posStr += StringUtility::intToString(posList[i].x);
		posStr += ",";
		posStr += StringUtility::intToString(posList[i].y);
		posStr += "\n";
	}
	FileUtility::writeFile(positionFile, posStr);
}

POINT* ImageUtility::readPositionFile(const string& positionFile, int& posCount)
{
	string posStr = FileUtility::openTxtFile(positionFile);
	if (posStr == "")
	{
		return NULL;
	}
	txVector<string> posStrList;
	StringUtility::split(posStr, "\n", posStrList);
	posCount = posStrList.size();
	POINT* posList = TRACE_NEW_ARRAY(POINT, posCount, posList);
	for (int i = 0; i < posCount; ++i)
	{
		txVector<string> pointList;
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

void ImageUtility::autoGroupHumanImage(const string& path)
{
	// 先拆分位置文件
	splitPositionFile(path);
	// 按照600个文件一组,放入单独的文件夹中
	autoMoveFile(path, HUMAN_GROUP_SIZE);

	txVector<string> folderList;
	FileUtility::findFolders(path, folderList);
	int folderCount = folderList.size();
	for (int i = 0; i < folderCount; ++i)
	{
		// 按照动作重命名
		txVector<string> fileList;
		FileUtility::findFiles(folderList[i], fileList, ".png", false);
		sortByFileNumber(fileList);
		int fileCount = fileList.size();
		for (int j = 0; j < fileCount; ++j)
		{
			string actionName;
			int direction;
			int frameIndex;
			bool isValid = getHumanActionInfo(j, actionName, direction, frameIndex);
			// 如果是无效图片则需要删除
			if (!isValid)
			{
				deleteImageWithPosition(fileList[j]);
				continue;
			}
			
			string destPath = StringUtility::getFilePath(fileList[j]) + "/" + actionName + "/";
			string actionFolderName = actionName + "_dir" + StringUtility::intToString(direction);
			string fileName = actionFolderName + "_" + StringUtility::intToString(frameIndex) + StringUtility::getFileSuffix(fileList[j], true);
			moveImageWithPosition(fileList[j], destPath + fileName);
		}
	}
}

void ImageUtility::autoGroupWeaponImage(const string& path)
{
	// 先拆分位置文件
	splitPositionFile(path);
	// 按照600个文件一组,放入单独的文件夹中
	autoMoveFile(path, WEAPON_GROUP_SIZE);
	// 因为武器文件是角色文件的2倍,可能是左右手的区别,所以暂时只解析前一半的文件,后面的删除
	txVector<string> folderList;
	FileUtility::findFolders(path, folderList);
	int folderCount = folderList.size();
	for (int i = 0; i < folderCount; ++i)
	{
		// 按照动作重命名
		txVector<string> fileList;
		FileUtility::findFiles(folderList[i], fileList, ".png", false);
		sortByFileNumber(fileList);
		int fileCount = fileList.size();
		for (int j = 0; j < fileCount; ++j)
		{
			if (WEAPON_GROUP_SIZE == 1200 && j >= WEAPON_GROUP_SIZE / 2)
			{
				deleteImageWithPosition(fileList[j]);
			}
			else
			{
				string actionName;
				int direction;
				int frameIndex;
				bool isValid = getHumanActionInfo(j, actionName, direction, frameIndex);
				// 如果是无效图片则需要删除
				if (!isValid)
				{
					deleteImageWithPosition(fileList[j]);
					continue;
				}
				string destPath = StringUtility::getFilePath(fileList[j]) + "/" + actionName + "/";
				string actionFolderName = actionName + "_dir" + StringUtility::intToString(direction);
				string fileName = actionFolderName + "_" + StringUtility::intToString(frameIndex) + StringUtility::getFileSuffix(fileList[j], true);
				moveImageWithPosition(fileList[j], destPath + fileName);
			}
		}
	}
}

void ImageUtility::autoGroupMonsterImage0(const string& path)
{
	// 拆分位置文件
	ImageUtility::splitPositionFile(path);
	// 然后按360个文件一组,移动到单独的文件夹
	ImageUtility::autoMoveFile(path, MONSTER_GROUP_SIZE);
	// 怪物图片不像角色和武器图片那样会有规律排列,所以只能在大致分组后手动对每个文件夹进行动作分组
}

void ImageUtility::autoGroupMonsterImage1(const string& path)
{
	// 手动对动作进行分组后,就可以对每组动作文件进行方向分组
	// 重命名文件,将每个文件夹中的图片都重命名为该文件夹中的位置序号
	ImageUtility::renameImage(path);
	// 自动计算方向并分组
	ImageUtility::renameByDirection(path, DIRECTION_COUNT);
}

void ImageUtility::autoGroupEffectImage(const string& path)
{
	// 先拆分位置文件
	splitPositionFile(path);
	// 按照10个文件一组,放入单独的文件夹中
	autoMoveFile(path, EFFECT_GROUP_SIZE);
	// 删除无效图片
	deleteInvalidImage(path);
}

void ImageUtility::autoGroupNPCImage(const string& path)
{
	// 先拆分位置文件
	splitPositionFile(path);
	// 按照60个文件一组,放入单独的文件夹中
	autoMoveFile(path, NPC_GROUP_SIZE);

	txVector<string> folderList;
	FileUtility::findFolders(path, folderList);
	int folderCount = folderList.size();
	for (int i = 0; i < folderCount; ++i)
	{
		// 按照动作重命名
		txVector<string> fileList;
		FileUtility::findFiles(folderList[i], fileList, ".png", false);
		sortByFileNumber(fileList);
		int fileCount = fileList.size();
		for (int j = 0; j < fileCount; ++j)
		{
			string actionName;
			int direction;
			int frameIndex;
			bool isValid = getNPCActionInfo(j, actionName, direction, frameIndex);
			// 如果是无效图片则需要删除
			if (!isValid)
			{
				deleteImageWithPosition(fileList[j]);
				continue;
			}
			string actionFolderName = actionName + "_dir" + StringUtility::intToString(direction);
			string destPath = StringUtility::getFilePath(fileList[j]) + "/";
			moveImageWithPosition(fileList[j], destPath + actionFolderName + "_" + StringUtility::intToString(frameIndex) + StringUtility::getFileSuffix(fileList[j], true));
		}
	}
}

void ImageUtility::renameImage(const string& path)
{
	// 将目录中的文件按文件名排序后,重命名为从0开始的数字
	txVector<string> folderList;
	FileUtility::findFolders(path, folderList, true);
	int folderCount = folderList.size();
	for (int i = 0; i < folderCount; ++i)
	{
		txVector<string> fileList;
		FileUtility::findFiles(folderList[i], fileList, ".png");
		// 先根据文件名重新排列
		sortByFileNumber(fileList);
		int count = fileList.size();
		for (int j = 0; j < count; ++j)
		{
			string curFilePath = StringUtility::getFilePath(fileList[j]) + "/";
			string suffix = StringUtility::getFileSuffix(fileList[j]);
			renameImageWithPosition(fileList[j], curFilePath + StringUtility::intToString(j) + "." + suffix);
		}
	}
}

void ImageUtility::renameImageToAnim(const string& path)
{
	// 将目录中的文件按文件名排序后,重命名为序列帧格式的名字,以文件夹名开头,以从0开始的数字结尾
	txVector<string> folderList;
	FileUtility::findFolders(path, folderList, true);
	int folderCount = folderList.size();
	for (int i = 0; i < folderCount; ++i)
	{
		txVector<string> fileList;
		FileUtility::findFiles(folderList[i], fileList, ".png", false);
		if (fileList.size() == 0)
		{
			continue;
		}
		// 先根据文件名重新排列
		sortByFileNumber(fileList);
		int count = fileList.size();
		for (int j = 0; j < count; ++j)
		{
			string folderName = StringUtility::getFolderName(fileList[j]);
			string curFilePath = StringUtility::getFilePath(fileList[j]) + "/";
			string suffix = StringUtility::getFileSuffix(fileList[j]);
			renameImageWithPosition(fileList[j], curFilePath + folderName + "_" + StringUtility::intToString(j) + "." + suffix);
		}
	}
}

void ImageUtility::splitPositionFile(const string& path, bool pathWithFileName)
{
	// 将position.txt文件拆分为单个的txt文件,每个txt文件中只包含一个坐标
	int posCount = 0;
	string filePath = path;
	string pathWithFile = path;
	if (!pathWithFileName)
	{
		pathWithFile += "/position.txt";
	}
	else
	{
		filePath = StringUtility::getFilePath(filePath);
	}
	POINT* posList = readPositionFile(pathWithFile, posCount);
	if (posList == NULL)
	{
		return;
	}
	for (int i = 0; i < posCount; ++i)
	{
		string posStr = StringUtility::intToString(posList[i].x) + "," + StringUtility::intToString(posList[i].y);
		FileUtility::writeFile(filePath + "/" + StringUtility::intToString(i) + ".txt", posStr);
	}
	TRACE_DELETE_ARRAY(posList);
}

void ImageUtility::renameByDirection(const string& path, int directionCount, bool autoGroup)
{
	// 将目录中的所有文件先按照文件名排序,然后按照顺序分组为8个方向,再对每个方向的文件重命名
	txVector<string> folderList;
	FileUtility::findFolders(path, folderList, true);
	int folderCount = folderList.size();
	for (int i = 0; i < folderCount; ++i)
	{
		txVector<string> fileList;
		FileUtility::findFiles(folderList[i], fileList, ".png", false);
		if (fileList.size() == 0)
		{
			continue;
		}
		sortByFileNumber(fileList, false);
		int fileCount = fileList.size();
		if (fileCount % directionCount != 0)
		{
			std::cout << "图片数量错误,必须为方向的整数倍, path:" << folderList[i] << std::endl;
			continue;
		}
		int actionFrameCount = fileCount / directionCount;
		for (int j = 0; j < fileCount; ++j)
		{
			int imageDir = j / actionFrameCount;
			int index = j % actionFrameCount;
			// 把文件移动到一个新建文件夹中
			string curPath = StringUtility::getFilePath(fileList[j]) + "/";
			if (autoGroup)
			{
				string destFolderName = StringUtility::getFolderName(fileList[j]) + "_dir" + StringUtility::intToString(imageDir);
				string destPath = StringUtility::getFilePath(curPath) + "/" + destFolderName + "/";
				string destFileName = destFolderName + "_" + StringUtility::intToString(index) + "." + StringUtility::getFileSuffix(fileList[j]);
				moveImageWithPosition(fileList[j], destPath + destFileName);
			}
			else
			{
				string preName = StringUtility::getFolderName(fileList[j]) + "_dir" + StringUtility::intToString(imageDir);
				string destFileName = preName + "_" + StringUtility::intToString(index) + "." + StringUtility::getFileSuffix(fileList[j]);
				renameImageWithPosition(fileList[j], curPath + destFileName);
			}
		}
	}
	// 删除空的目录
	FileUtility::deleteEmptyFolder(path);
}

void ImageUtility::sortByFileNumber(txVector<string>& fileList, bool fileNameIsNumber)
{
	// 根据文件名的数字进行排序
	int count = fileList.size();
	if (fileNameIsNumber)
	{
		txMap<int, string> sortedList;
		for (int i = 0; i < count; ++i)
		{
			sortedList.insert(StringUtility::stringToInt(StringUtility::getFileNameNoSuffix(fileList[i])), fileList[i]);
		}
		if (sortedList.size() != fileList.size())
		{
			return;
		}
		fileList.clear();
		auto iter = sortedList.begin();
		auto iterEnd = sortedList.end();
		for (; iter != iterEnd; ++iter)
		{
			fileList.push_back(iter->second);
		}
	}
	else
	{
		txMap<string, txMap<int, string>> sortedList;
		for (int i = 0; i < count; ++i)
		{
			string fileName = StringUtility::getFileNameNoSuffix(fileList[i]);
			string preName = fileName.substr(0, fileName.find_last_of('_') + 1);
			if (!sortedList.contains(preName))
			{
				sortedList.insert(preName, txMap<int, string>());
			}
			string suffixNumber = fileName.substr(fileName.find_last_of('_') + 1);
			sortedList[preName].insert(StringUtility::stringToInt(suffixNumber), fileList[i]);
		}
		fileList.clear();
		auto iter = sortedList.begin();
		auto iterEnd = sortedList.end();
		for (; iter != iterEnd; ++iter)
		{
			auto iterName = iter->second.begin();
			auto iterNameEnd = iter->second.end();
			for (; iterName != iterNameEnd; ++iterName)
			{
				fileList.push_back(iterName->second);
			}
		}
	}
}

void ImageUtility::autoMoveFile(const string& path, int groupSize)
{
	txVector<string> fileList;
	FileUtility::findFiles(path, fileList, ".png");
	sortByFileNumber(fileList);
	int fileCount = fileList.size();
	for (int i = 0; i < fileCount; ++i)
	{
		int groupIndex = i / groupSize;
		string destFolderName = StringUtility::intToString(groupIndex, 3);
		string destPath = StringUtility::getFilePath(fileList[i]) + "/" + destFolderName + "/";
		moveImageWithPosition(fileList[i], destPath + StringUtility::getFileName(fileList[i]));
	}
}

bool ImageUtility::getHumanActionInfo(int index, string& actionName, int& dir, int& frameIndex)
{
	int actionIndex = 0;
	while (true)
	{
		if (index - HUMAN_ACTION[actionIndex].mMaxFrame * DIRECTION_COUNT < 0)
		{
			break;
		}
		index -= HUMAN_ACTION[actionIndex].mMaxFrame * DIRECTION_COUNT;
		++actionIndex;
	}
	// 因为一组动作资源包含了8个方向上的所有动作,所以可以根据下标计算出方向和序列帧下标,前提是保留了空图片作为填充位置
	dir = index / HUMAN_ACTION[actionIndex].mMaxFrame;
	frameIndex = index % HUMAN_ACTION[actionIndex].mMaxFrame;
	actionName = HUMAN_ACTION[actionIndex].mName;
	return frameIndex < HUMAN_ACTION[actionIndex].mFrameCount;
}

bool ImageUtility::getNPCActionInfo(int index, string& actionName, int& dir, int& frameIndex)
{
	int actionIndex = 0;
	while (true)
	{
		if (index - NPC_ACTION[actionIndex].mMaxFrame * NPC_DIRECTION_COUNT < 0)
		{
			break;
		}
		index -= NPC_ACTION[actionIndex].mMaxFrame * NPC_DIRECTION_COUNT;
		++actionIndex;
	}
	// 因为一组动作资源包含了3个方向上的所有动作,所以可以根据下标计算出方向和序列帧下标,前提是保留了空图片作为填充位置
	// NPC的方向从3开始,并且只有3,4,5这3个方向的动作
	dir = index / NPC_ACTION[actionIndex].mMaxFrame + 3;
	frameIndex = index % NPC_ACTION[actionIndex].mMaxFrame;
	actionName = NPC_ACTION[actionIndex].mName;
	return frameIndex < NPC_ACTION[actionIndex].mFrameCount;
}

void ImageUtility::moveImageWithPosition(const string& fullFileName, const string& destFullFileName)
{
	string sourceFileNameNoSuffix = StringUtility::getFileNameNoSuffix(fullFileName);
	string destFileNameNoSuffix = StringUtility::getFileNameNoSuffix(destFullFileName);
	string sourcePath = StringUtility::getFilePath(fullFileName) + "/";
	string destPath = StringUtility::getFilePath(destFullFileName) + "/";
	moveFileWithMeta(fullFileName, destFullFileName);
	// 如果有同名位置文件,也需要一起移动
	string positionFileName = sourcePath + sourceFileNameNoSuffix + ".txt";
	if (FileUtility::isFileExist(positionFileName))
	{
		string destPosFileName = destPath + destFileNameNoSuffix + ".txt";
		moveFileWithMeta(positionFileName, destPosFileName);
	}
}

void ImageUtility::renameImageWithPosition(const string& fullFileName, const string& destFullFileName)
{
	string sourceFileNameNoSuffix = StringUtility::getFileNameNoSuffix(fullFileName);
	string destFileNameNoSuffix = StringUtility::getFileNameNoSuffix(destFullFileName);
	string sourcePath = StringUtility::getFilePath(fullFileName) + "/";
	string destPath = StringUtility::getFilePath(destFullFileName) + "/";
	renameFileWithMeta(fullFileName, destFullFileName);
	// 如果有同名位置文件,也需要一起重命名
	string positionFileName = sourcePath + sourceFileNameNoSuffix + ".txt";
	if (FileUtility::isFileExist(positionFileName))
	{
		string destPosFileName = destPath + destFileNameNoSuffix + ".txt";
		renameFileWithMeta(positionFileName, destPosFileName);
	}
}

void ImageUtility::deleteImageWithPosition(const string& fullFileName)
{
	string sourceFileNameNoSuffix = StringUtility::getFileNameNoSuffix(fullFileName);
	string sourcePath = StringUtility::getFilePath(fullFileName) + "/";
	deleteFileWithMeta(fullFileName);
	// 如果有同名位置文件,也需要一起删除
	string positionFileName = sourcePath + sourceFileNameNoSuffix + ".txt";
	if (FileUtility::isFileExist(positionFileName))
	{
		deleteFileWithMeta(positionFileName);
	}
}

void ImageUtility::moveFileWithMeta(const string& fullFileName, const string& destFullFileName)
{
	// 移动文件和同名meta文件
	FileUtility::moveFile(fullFileName, destFullFileName);
	string metaFile = fullFileName + ".meta";
	if (FileUtility::isFileExist(metaFile))
	{
		FileUtility::moveFile(metaFile, destFullFileName + ".meta");
	}
}

void ImageUtility::renameFileWithMeta(const string& fullFileName, const string& destFullFileName)
{
	// 重命名文件和同名meta文件
	FileUtility::renameFile(fullFileName, destFullFileName);
	string metaFile = fullFileName + ".meta";
	if (FileUtility::isFileExist(metaFile))
	{
		FileUtility::renameFile(metaFile, destFullFileName + ".meta");
	}
}

void ImageUtility::deleteFileWithMeta(const string& fullFileName)
{
	// 删除文件和同名meta文件
	FileUtility::deleteFile(fullFileName);
	string metaFile = fullFileName + ".meta";
	if (FileUtility::isFileExist(metaFile))
	{
		FileUtility::deleteFile(metaFile);
	}
}

void ImageUtility::deleteInvalidImage(const string& path)
{
	txVector<string> fileList;
	FileUtility::findFiles(path, fileList, ".png");
	sortByFileNumber(fileList);
	int count = fileList.size();
	for (int i = 0; i < count; ++i)
	{
		if (!isInvalidImage(fileList[i]))
		{
			deleteImageWithPosition(fileList[i]);
		}
	}
	FileUtility::deleteEmptyFolder(path);
}

bool ImageUtility::isInvalidImage(const string& fileName)
{
	FreeImage_Initialise(1);
	FREE_IMAGE_FORMAT format = FreeImage_GetFileType(fileName.c_str());
	FIBITMAP* bitmap = FreeImage_Load(format, fileName.c_str());
	int width = FreeImage_GetWidth(bitmap);
	int height = FreeImage_GetHeight(bitmap);
	FreeImage_Unload(bitmap);
	FreeImage_DeInitialise();
	return width * height > 4;
}

POINT ImageUtility::getImagePosition(const string& imageFullPath)
{
	POINT pos;
	string posFileName = StringUtility::getFilePath(imageFullPath) + "/" + StringUtility::getFileNameNoSuffix(imageFullPath) + ".txt";
	string posFile = FileUtility::openTxtFile(posFileName);
	txVector<int> posValue;
	StringUtility::stringToIntArray(posFile, posValue);
	if (posValue.size() == 2)
	{
		pos.x = posValue[0];
		pos.y = posValue[1];
	}
	else
	{
		std::cout << "位置文件内容错误 : " << imageFullPath << std::endl;
	}
	return pos;
}

void ImageUtility::collectMapTexture(const string& fileName)
{
	string file = StringUtility::getFileNameNoSuffix(fileName);
	SceneMap* map = TRACE_NEW(SceneMap, map);
	map->readFile(fileName + ".map");
	int tileCount = map->mHeader->mWidth * map->mHeader->mHeight;
	for (int i = 0; i < tileCount; ++i)
	{
		string srcFilePath = "../media/Objects" + StringUtility::intToString(map->mTileList[i].mObjFileIdx + 1) + "/";
		string srcFileName = StringUtility::intToString(map->mTileList[i].mObjImgIdx) + ".png";
		string destFilePath = "../media/MapTexture/" + file + "/";
		if (FileUtility::isFileExist(srcFilePath + srcFileName))
		{
			FileUtility::copyFile(srcFilePath + srcFileName, destFilePath + srcFileName);
			FileUtility::copyFile(srcFilePath + srcFileName + ".meta", destFilePath + srcFileName + ".meta");
		}
	}
	TRACE_DELETE(map);
}

void ImageUtility::groupAtlas(const string& filePath, int countInAltas)
{
	string atlasInfo;
	txSerializer serializer;
	txVector<string> fileList;
	FileUtility::findFiles(filePath, fileList, ".png", false);
	sortByFileNumber(fileList);
	int count = fileList.size();
	for (int i = 0; i < count; ++i)
	{
		int atlasIndex = i / countInAltas;
		string curFile = StringUtility::getFileNameNoSuffix(fileList[i]);
		string folderName = StringUtility::intToString(atlasIndex);
		string newPath = StringUtility::getFilePath(fileList[i]) + "/" + folderName + "/";
		FileUtility::moveFile(fileList[i], newPath + curFile + ".png");
		FileUtility::moveFile(fileList[i] + ".meta", newPath + curFile + ".png.meta");
		// 每一行需要记录图片在图集中的位置
		atlasInfo += StringUtility::getFileName(fileList[i]) + ":" + StringUtility::intToString(atlasIndex) + "\n";
		serializer.write<unsigned short>(StringUtility::stringToInt(StringUtility::getFileNameNoSuffix(fileList[i])));
		serializer.write<unsigned char>(atlasIndex);
	}
	FileUtility::writeFile(filePath + "/atlas.txt", atlasInfo);
	FileUtility::writeFile(filePath + "/atlas.index", serializer.getBuffer(), serializer.getDataSize());
}

void ImageUtility::texturePacker(const string& texturePath)
{
	string outputFileName = StringUtility::getFileName(texturePath);
	string outputPath = StringUtility::getFilePath(texturePath);
	string cmdLine;
	cmdLine += "--data " + outputPath + "/" + outputFileName + ".txt ";
	cmdLine += "--sheet " + outputPath + "/" + outputFileName + ".png ";
	cmdLine += "--format json ";
	cmdLine += "--allow-free-size ";
	cmdLine += "--maxrects-heuristics Best ";
	cmdLine += "--trim-mode None ";
	cmdLine += "--disable-rotation ";
	cmdLine += "--size-constraints POT ";
	cmdLine += "--max-size 2048 ";
	cmdLine += "--padding 1 ";
	cmdLine += texturePath;

	SHELLEXECUTEINFOA ShExecInfo = { 0 };
	ShExecInfo.cbSize = sizeof(SHELLEXECUTEINFOA);
	ShExecInfo.fMask = SEE_MASK_NOCLOSEPROCESS;
	ShExecInfo.hwnd = NULL;
	ShExecInfo.lpVerb = NULL;
	ShExecInfo.lpFile = "C:\\Program Files\\CodeAndWeb\\TexturePacker\\bin\\TexturePacker.exe";
	ShExecInfo.lpParameters = cmdLine.c_str();
	ShExecInfo.lpDirectory = NULL;
	ShExecInfo.nShow = SW_HIDE;
	ShExecInfo.hInstApp = NULL;
	BOOL ret = ShellExecuteExA(&ShExecInfo);
	WaitForSingleObject(ShExecInfo.hProcess, INFINITE);
}

void ImageUtility::texturePackerAll(const string& texturePath)
{
	txVector<string> folderList;
	FileUtility::findFolders(texturePath, folderList, true);
	int count = folderList.size();
	for (int i = 0; i < count; ++i)
	{
		if (!FileUtility::isEmptyFolder(folderList[i]))
		{
			texturePacker(folderList[i]);
		}
	}
}

void ImageUtility::readAtlasIndexFile(const string& fileName, txMap<int, int>& indexMap)
{
	int fileSize = 0;
	char* fileBuffer = FileUtility::openBinaryFile(fileName, &fileSize);
	txSerializer serializer(fileBuffer, fileSize);
	int indexCount = fileSize / 3;
	for (int j = 0; j < indexCount; ++j)
	{
		unsigned short imageIndex;
		unsigned char atlasIndex;
		serializer.read(imageIndex);
		serializer.read(atlasIndex);
		if (!indexMap.insert(imageIndex, atlasIndex).second)
		{
			SystemUtility::print("读取图集下标文件失败,有重复的图片文件! image index : " + StringUtility::intToString(imageIndex));
		}
	}
	TRACE_DELETE_ARRAY(fileBuffer);
}

void ImageUtility::convertMapFile(const string& fileName)
{
	string fileNoSuffix = StringUtility::getFileNameNoSuffix(fileName, false);
	SceneMap* map = TRACE_NEW(SceneMap, map);
	map->readFile(fileNoSuffix + ".map");
	// 物体图集下标
	txMap<int, txMap<int, int>> objAtlasIndexMap;
	// 目前只有7个总的物体图片文件夹
	for (int i = 0; i < 7; ++i)
	{
		txMap<int, int> indexMap;
		readAtlasIndexFile("../media/Objects" + StringUtility::intToString(i + 1) + "/atlas.index", indexMap);
		objAtlasIndexMap.insert(i + 1, indexMap);
	}
	// 大地砖图集下标
	txMap<int, int> bngAtlasIndexMap;
	readAtlasIndexFile("../media/Tiles/atlas.index", bngAtlasIndexMap);
	SceneMapAdvance* mapAdvance = TRACE_NEW(SceneMapAdvance, mapAdvance);
	mapAdvance->initFromMap(map, objAtlasIndexMap, bngAtlasIndexMap);
	mapAdvance->saveAdvanceMap(fileNoSuffix + ".amap");
	TRACE_DELETE(map);
	TRACE_DELETE(mapAdvance);
}

void ImageUtility::convertAllMapFile(const string& filePath)
{
	txVector<string> fileList;
	FileUtility::findFiles(filePath, fileList, ".map", false);
	int count = fileList.size();
	for (int i = 0; i < count; ++i)
	{
		convertMapFile(fileList[i]);
	}
}

void ImageUtility::writeAnimFrameSQLite(bool updateOnly)
{
	SQLite* sqlite = TRACE_NEW(SQLite, sqlite, "../media/DataBase.db");
	txVector<string> folders;
	FileUtility::findFolders("../media", folders, true);
	int folderCount = folders.size();
	for (int i = 0; i < folderCount; ++i)
	{
		txVector<string> pngFiles;
		FileUtility::findFiles(folders[i], pngFiles, ".png", false);
		if (pngFiles.size() == 0)
		{
			continue;
		}
		// 1个Atlas中包含多个动画,一个动画有多个帧
		txMap<string, pair<txVector<int>, txVector<int>>> animationInfo;
		int pngCount = pngFiles.size();
		for (int j = 0; j < pngCount; ++j)
		{
			string& fullFileName = pngFiles[j];
			string fileNameNoSuffix = StringUtility::getFileNameNoSuffix(fullFileName);
			string animation = fileNameNoSuffix.substr(0, StringUtility::getFileNameNoSuffix(fullFileName).find_last_of('_'));
			if (!animationInfo.contains(animation))
			{
				animationInfo.insert(animation, pair<txVector<int>, txVector<int>>());
			}
			string posString = FileUtility::openTxtFile(StringUtility::getFileNameNoSuffix(fullFileName, false) + ".txt");
			txVector<int> pos;
			StringUtility::stringToIntArray(posString, pos);
			if (pos.size() == 2)
			{
				animationInfo[animation].first.push_back(pos[0]);
				animationInfo[animation].second.push_back(pos[1]);
			}
			else
			{
				std::cout << "位置文件错误：" << StringUtility::getFileNameNoSuffix(fullFileName, false) + ".txt" << std::endl;
			}
		}
		auto iter = animationInfo.begin();
		auto iterEnd = animationInfo.end();
		for (; iter != iterEnd; ++iter)
		{
			AnimationFrameData animationData;
			animationData.mAtlas = folders[i].substr(strlen("../media/"));
			animationData.mAnimation = iter->first;
			animationData.mFrameCount = iter->second.first.size();
			animationData.mPosX = iter->second.first;
			animationData.mPosY = iter->second.second;
			if (updateOnly)
			{
				sqlite->mSQLiteAnimationFrame->updateData(animationData);
			}
			else
			{
				sqlite->mSQLiteAnimationFrame->insert(animationData);
			}
		}
	}
	TRACE_DELETE(sqlite);
}

void ImageUtility::writeImagePosSQLite(const string& path)
{
	SQLite* sqlite = TRACE_NEW(SQLite, sqlite, "../media/DataBase.db");
	txVector<std::string> files;
	FileUtility::findFiles(path, files, ".png");
	int count = files.size();
	for (int i = 0; i < count; ++i)
	{
		string folder = StringUtility::getFolderName(files[i]);
		string fileName = StringUtility::getFileNameNoSuffix(files[i]);
		ImagePositionData data;
		data.mAtlas = folder;
		data.mImage = fileName;
		POINT pos = getImagePosition(files[i]);
		data.mPosX = pos.x;
		data.mPosY = pos.y;
		sqlite->mSQLiteImagePosition->insert(data);
	}
	TRACE_DELETE(sqlite);
}
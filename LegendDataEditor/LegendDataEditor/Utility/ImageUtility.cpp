#include "Utility.h"
#include "txSerializer.h"
#include "HumanImage.h"
#include "WeaponImage.h"
#include "SQLite.h"
#include "SQLiteSceneMap.h"
#include "SQLiteSceneMapFile.h"
#include "SQLiteSceneMapTransfer.h"
#include "SQLiteNPC.h"
#include "SQLiteGoods.h"
#include "SQLiteMonGen.h"
#include "SQLiteMonster.h"
#include "SQLiteMonsterTemplate.h"
#include "SQLiteMagic.h"
#include "SQLiteStdItem.h"
#include "SQLiteImagePositionAnimation.h"
#include "SQLiteMonsterCollider.h"
#include "SQLiteImagePositionEffect.h"
#include "SQLiteImagePositionIcon.h"
#include "SQLiteItemEquip.h"
#include "SQLiteItemConsumable.h"
#include "SQLiteItemSkillBook.h"
#include "SQLiteSkillEffectDirection.h"
#include "SQLitePeaceArea.h"
#include "SQLiteMapEffect.h"
#include "HumanAction.h"
#include "WeaponAction.h"
#include "MapData.h"
#include "MapHeader.h"
#include "MapTile.h"
#include "UnreachTileGroup.h"

void ImageUtility::encodePNG(const string& path, unsigned char* color, int width, int height, FREE_IMAGE_FORMAT format)
{
	string dir = getFilePath(path);
	createFolder(dir);
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
	char* fileBuffer = openBinaryFile(filePath, &fileSize);
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
	char* fileBuffer = openBinaryFile(filePath, &fileSize);
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
		cout << "找不到wix文件" << endl;
		return;
	}

	// 打开wil文件
	WILFileHeader wilHeader;
	if(!readWilHeader(wilFileName, wilHeader))
	{
		cout << "找不到wil文件" << endl;
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
	string fileNameNoSuffix = getFileNameNoSuffix(wilFileName);
	cout << "正在解析" << fileNameNoSuffix << ", 图片数量 : " << wilHeader.mImageCount << ", 颜色位数 : " << bpp << endl;
	POINT* posList = TRACE_NEW_ARRAY(POINT, wixFileHeader.mPositionList.size(), posList);
	int fileSize = 0;
	char* fileBuffer = openBinaryFile(wilFileName, &fileSize);
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
		encodePNG(outputPath + intToString(i) + ".png", curInfo.mColor, curInfo.mWidth, curInfo.mHeight, FIF_PNG);
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
	findFiles(sourcePath, fileList, patterns);
	int fileCount = fileList.size();
	for (int i = 0; i < fileCount; ++i)
	{
		string wixFile = getFileNameNoSuffix(fileList[i], false) + ".wix";
		string filePath = getFilePath(fileList[i]);
		string folderName = getFileNameNoSuffix(fileList[i]);
		wixWilToPNG(wixFile, fileList[i], filePath + "/" + folderName + "/");
	}
}

void ImageUtility::writePositionFile(const string& positionFile, POINT* posList, int posCount)
{
	string posStr;
	for (int i = 0; i < posCount; ++i)
	{
		posStr += intToString(posList[i].x);
		posStr += ",";
		posStr += intToString(posList[i].y);
		posStr += "\n";
	}
	writeFile(positionFile, posStr);
}

POINT* ImageUtility::readPositionFile(const string& positionFile, int& posCount)
{
	string posStr = openTxtFile(positionFile);
	if (posStr == "")
	{
		return NULL;
	}
	txVector<string> posStrList;
	split(posStr, "\n", posStrList);
	posCount = posStrList.size();
	POINT* posList = TRACE_NEW_ARRAY(POINT, posCount, posList);
	for (int i = 0; i < posCount; ++i)
	{
		txVector<string> pointList;
		split(posStrList[i], ",", pointList);
		if (pointList.size() != 2)
		{
			continue;
		}
		posList[i].x = stringToInt(pointList[0]);
		posList[i].y = stringToInt(pointList[1]);
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
	findFolders(path, folderList);
	int folderCount = folderList.size();
	for (int i = 0; i < folderCount; ++i)
	{
		// 按照动作重命名
		txVector<string> fileList;
		findFiles(folderList[i], fileList, ".png", false);
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
			string destPath = getFilePath(fileList[j]) + "/" + actionName + "/";
			string actionFolderName = actionName + "_dir" + intToString(direction);
			string fileName = actionFolderName + "_" + intToString(frameIndex) + getFileSuffix(fileList[j], true);
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
	findFolders(path, folderList);
	int folderCount = folderList.size();
	for (int i = 0; i < folderCount; ++i)
	{
		// 按照动作重命名
		txVector<string> fileList;
		findFiles(folderList[i], fileList, ".png", false);
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
				string destPath = getFilePath(fileList[j]) + "/" + actionName + "/";
				string actionFolderName = actionName + "_dir" + intToString(direction);
				string fileName = actionFolderName + "_" + intToString(frameIndex) + getFileSuffix(fileList[j], true);
				moveImageWithPosition(fileList[j], destPath + fileName);
			}
		}
	}
}

void ImageUtility::autoGroupMonsterImage0(const string& path)
{
	// 拆分位置文件
	splitPositionFile(path);
	// 然后按360个文件一组,移动到单独的文件夹
	autoMoveFile(path, MONSTER_GROUP_SIZE);
	// 怪物图片不像角色和武器图片那样会有规律排列,所以只能在大致分组后手动对每个文件夹进行动作分组
}

void ImageUtility::autoGroupMonsterImage1(const string& path)
{
	// 手动对动作进行分组后,就可以对每组动作文件进行方向分组
	// 重命名文件,将每个文件夹中的图片都重命名为该文件夹中的位置序号
	renameImage(path);
	// 自动计算方向并分组
	renameByDirection(path, DIRECTION_COUNT);
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
	findFolders(path, folderList);
	int folderCount = folderList.size();
	for (int i = 0; i < folderCount; ++i)
	{
		// 按照动作重命名
		txVector<string> fileList;
		findFiles(folderList[i], fileList, ".png", false);
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
			string actionFolderName = actionName + "_dir" + intToString(direction);
			string destPath = getFilePath(fileList[j]) + "/";
			moveImageWithPosition(fileList[j], destPath + actionFolderName + "_" + intToString(frameIndex) + getFileSuffix(fileList[j], true));
		}
	}
}

void ImageUtility::renameImage(const string& path)
{
	// 将目录中的文件按文件名排序后,重命名为从0开始的数字
	txVector<string> folderList;
	findFolders(path, folderList, true);
	int folderCount = folderList.size();
	for (int i = 0; i < folderCount; ++i)
	{
		txVector<string> fileList;
		findFiles(folderList[i], fileList, ".png");
		// 先根据文件名重新排列
		sortByFileNumber(fileList);
		int count = fileList.size();
		for (int j = 0; j < count; ++j)
		{
			string curFilePath = getFilePath(fileList[j]) + "/";
			string suffix = getFileSuffix(fileList[j]);
			renameImageWithPosition(fileList[j], curFilePath + intToString(j) + "." + suffix);
		}
	}
}

void ImageUtility::renameImageToAnim(const string& path)
{
	// 将目录中的文件按文件名排序后,重命名为序列帧格式的名字,以文件夹名开头,以从0开始的数字结尾
	txVector<string> folderList;
	findFolders(path, folderList, true);
	int folderCount = folderList.size();
	for (int i = 0; i < folderCount; ++i)
	{
		txVector<string> fileList;
		findFiles(folderList[i], fileList, ".png", false);
		if (fileList.size() == 0)
		{
			continue;
		}
		// 先根据文件名重新排列
		sortByFileNumber(fileList);
		int count = fileList.size();
		for (int j = 0; j < count; ++j)
		{
			string folderName = getFolderName(fileList[j]);
			string curFilePath = getFilePath(fileList[j]) + "/";
			string suffix = getFileSuffix(fileList[j]);
			renameImageWithPosition(fileList[j], curFilePath + folderName + "_" + intToString(j) + "." + suffix);
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
		filePath = getFilePath(filePath);
	}
	POINT* posList = readPositionFile(pathWithFile, posCount);
	if (posList == NULL)
	{
		return;
	}
	for (int i = 0; i < posCount; ++i)
	{
		string posStr = intToString(posList[i].x) + "," + intToString(posList[i].y);
		writeFile(filePath + "/" + intToString(i) + ".txt", posStr);
	}
	TRACE_DELETE_ARRAY(posList);
}

void ImageUtility::renameByDirection(const string& path, int directionCount, bool autoGroup)
{
	// 将目录中的所有文件先按照文件名排序,然后按照顺序分组为8个方向,再对每个方向的文件重命名
	txVector<string> folderList;
	findFolders(path, folderList, true);
	int folderCount = folderList.size();
	for (int i = 0; i < folderCount; ++i)
	{
		txVector<string> fileList;
		findFiles(folderList[i], fileList, ".png", false);
		if (fileList.size() == 0)
		{
			continue;
		}
		sortByFileNumber(fileList, false);
		int fileCount = fileList.size();
		if (fileCount % directionCount != 0)
		{
			cout << "图片数量错误,必须为方向的整数倍, path:" << folderList[i] << endl;
			continue;
		}
		int actionFrameCount = fileCount / directionCount;
		for (int j = 0; j < fileCount; ++j)
		{
			int imageDir = j / actionFrameCount;
			int index = j % actionFrameCount;
			// 把文件移动到一个新建文件夹中
			string curPath = getFilePath(fileList[j]) + "/";
			if (autoGroup)
			{
				string destFolderName = getFolderName(fileList[j]) + "_dir" + intToString(imageDir);
				string destPath = getFilePath(curPath) + "/" + destFolderName + "/";
				string destFileName = destFolderName + "_" + intToString(index) + "." + getFileSuffix(fileList[j]);
				moveImageWithPosition(fileList[j], destPath + destFileName);
			}
			else
			{
				string preName = getFolderName(fileList[j]) + "_dir" + intToString(imageDir);
				string destFileName = preName + "_" + intToString(index) + "." + getFileSuffix(fileList[j]);
				renameImageWithPosition(fileList[j], curPath + destFileName);
			}
		}
	}
	// 删除空的目录
	deleteEmptyFolder(path);
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
			sortedList.insert(stringToInt(getFileNameNoSuffix(fileList[i])), fileList[i]);
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
			string fileName = getFileNameNoSuffix(fileList[i]);
			string preName = fileName.substr(0, fileName.find_last_of('_') + 1);
			if (!sortedList.contains(preName))
			{
				sortedList.insert(preName, txMap<int, string>());
			}
			string suffixNumber = fileName.substr(fileName.find_last_of('_') + 1);
			sortedList[preName].insert(stringToInt(suffixNumber), fileList[i]);
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
	findFiles(path, fileList, ".png");
	sortByFileNumber(fileList);
	int fileCount = fileList.size();
	for (int i = 0; i < fileCount; ++i)
	{
		int groupIndex = i / groupSize;
		string destFolderName = intToString(groupIndex, 3);
		string destPath = getFilePath(fileList[i]) + "/" + destFolderName + "/";
		moveImageWithPosition(fileList[i], destPath + getFileName(fileList[i]));
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
	string sourceFileNameNoSuffix = getFileNameNoSuffix(fullFileName);
	string destFileNameNoSuffix = getFileNameNoSuffix(destFullFileName);
	string sourcePath = getFilePath(fullFileName) + "/";
	string destPath = getFilePath(destFullFileName) + "/";
	moveFileWithMeta(fullFileName, destFullFileName);
	// 如果有同名位置文件,也需要一起移动
	string positionFileName = sourcePath + sourceFileNameNoSuffix + ".txt";
	if (isFileExist(positionFileName))
	{
		string destPosFileName = destPath + destFileNameNoSuffix + ".txt";
		moveFileWithMeta(positionFileName, destPosFileName);
	}
}

void ImageUtility::renameImageWithPosition(const string& fullFileName, const string& destFullFileName)
{
	string sourceFileNameNoSuffix = getFileNameNoSuffix(fullFileName);
	string destFileNameNoSuffix = getFileNameNoSuffix(destFullFileName);
	string sourcePath = getFilePath(fullFileName) + "/";
	string destPath = getFilePath(destFullFileName) + "/";
	renameFileWithMeta(fullFileName, destFullFileName);
	// 如果有同名位置文件,也需要一起重命名
	string positionFileName = sourcePath + sourceFileNameNoSuffix + ".txt";
	if (isFileExist(positionFileName))
	{
		string destPosFileName = destPath + destFileNameNoSuffix + ".txt";
		renameFileWithMeta(positionFileName, destPosFileName);
	}
}

void ImageUtility::deleteImageWithPosition(const string& fullFileName)
{
	string sourceFileNameNoSuffix = getFileNameNoSuffix(fullFileName);
	string sourcePath = getFilePath(fullFileName) + "/";
	deleteFileWithMeta(fullFileName);
	// 如果有同名位置文件,也需要一起删除
	string positionFileName = sourcePath + sourceFileNameNoSuffix + ".txt";
	if (isFileExist(positionFileName))
	{
		deleteFileWithMeta(positionFileName);
	}
}

void ImageUtility::moveFileWithMeta(const string& fullFileName, const string& destFullFileName)
{
	// 移动文件和同名meta文件
	moveFile(fullFileName, destFullFileName);
	string metaFile = fullFileName + ".meta";
	if (isFileExist(metaFile))
	{
		moveFile(metaFile, destFullFileName + ".meta");
	}
}

void ImageUtility::renameFileWithMeta(const string& fullFileName, const string& destFullFileName)
{
	// 重命名文件和同名meta文件
	renameFile(fullFileName, destFullFileName);
	string metaFile = fullFileName + ".meta";
	if (isFileExist(metaFile))
	{
		renameFile(metaFile, destFullFileName + ".meta");
	}
}

void ImageUtility::deleteFileWithMeta(const string& fullFileName)
{
	// 删除文件和同名meta文件
	deleteFile(fullFileName);
	string metaFile = fullFileName + ".meta";
	if (isFileExist(metaFile))
	{
		deleteFile(metaFile);
	}
}

void ImageUtility::deleteInvalidImage(const string& path)
{
	txVector<string> fileList;
	findFiles(path, fileList, ".png");
	sortByFileNumber(fileList);
	int count = fileList.size();
	for (int i = 0; i < count; ++i)
	{
		if (!isInvalidImage(fileList[i]))
		{
			deleteImageWithPosition(fileList[i]);
		}
	}
	deleteEmptyFolder(path);
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
	string posFileName = getFilePath(imageFullPath) + "/" + getFileNameNoSuffix(imageFullPath) + ".txt";
	string posFile = openTxtFile(posFileName);
	txVector<int> posValue;
	stringToIntArray(posFile, posValue);
	if (posValue.size() == 2)
	{
		pos.x = posValue[0];
		pos.y = posValue[1];
	}
	else
	{
		cout << "位置文件内容错误 : " << imageFullPath << endl;
	}
	return pos;
}

void ImageUtility::groupAtlas(const string& filePath, int countInAltas)
{
	string atlasInfo;
	txSerializer serializer;
	txVector<string> fileList;
	findFiles(filePath, fileList, ".png", false);
	sortByFileNumber(fileList);
	int count = fileList.size();
	for (int i = 0; i < count; ++i)
	{
		int atlasIndex = i / countInAltas;
		string curFile = getFileNameNoSuffix(fileList[i]);
		string folderName = intToString(atlasIndex);
		string newPath = getFilePath(fileList[i]) + "/" + folderName + "/";
		moveFile(fileList[i], newPath + curFile + ".png");
		moveFile(fileList[i] + ".meta", newPath + curFile + ".png.meta");
		// 每一行需要记录图片在图集中的位置
		atlasInfo += getFileName(fileList[i]) + ":" + intToString(atlasIndex) + "\n";
		serializer.write<unsigned short>(stringToInt(getFileNameNoSuffix(fileList[i])));
		serializer.write<unsigned char>(atlasIndex);
	}
	writeFile(filePath + "/atlas.txt", atlasInfo);
	writeFile(filePath + "/atlas.index", serializer.getBuffer(), serializer.getDataSize());
}

void ImageUtility::texturePacker(const string& texturePath)
{
	string outputFileName = getFileName(texturePath);
	string outputPath = getFilePath(texturePath);
	string cmdLine;
	cmdLine += "--data " + outputPath + "/" + outputFileName + ".tpsheet ";
	cmdLine += "--sheet " + outputPath + "/" + outputFileName + ".png ";
	cmdLine += "--format unity-texture2d ";
	cmdLine += "--alpha-handling KeepTransparentPixels ";
	cmdLine += "--force-squared ";
	cmdLine += "--maxrects-heuristics Best ";
	cmdLine += "--disable-rotation ";
	cmdLine += "--size-constraints POT ";
	cmdLine += "--max-size 2048 ";
	cmdLine += "--trim-mode None ";
	cmdLine += "--extrude 1 ";
	cmdLine += "--shape-padding 1 ";
	cmdLine += "--border-padding 1 ";
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
	findFolders(texturePath, folderList, true);
	int count = folderList.size();
	for (int i = 0; i < count; ++i)
	{
		if (!isEmptyFolder(folderList[i]))
		{
			texturePacker(folderList[i]);
			cout << "已打包:" << i + 1 << "/" << count << endl;
		}
	}
}

void ImageUtility::writeAnimFrameSQLite(bool updateIfExist)
{
	SQLite* sqlite = TRACE_NEW(SQLite, sqlite, "../media/DataBase.db");
	txVector<string> folders;
	findFolders("../media", folders, true);
	int folderCount = folders.size();
	for (int i = 0; i < folderCount; ++i)
	{
		txVector<string> pngFiles;
		findFiles(folders[i], pngFiles, ".png", false);
		if (pngFiles.size() == 0)
		{
			continue;
		}
		// 1个Atlas中包含多个动画,一个动画有多个帧
		txMap<string, pair<txMap<int, int>, txMap<int, int>>> animationInfo;
		int pngCount = pngFiles.size();
		for (int j = 0; j < pngCount; ++j)
		{
			string& fullFileName = pngFiles[j];
			string fileNameNoSuffix = getFileNameNoSuffix(fullFileName);
			string animation = fileNameNoSuffix.substr(0, getFileNameNoSuffix(fullFileName).find_last_of('_'));
			if (!animationInfo.contains(animation))
			{
				animationInfo.insert(animation, pair<txMap<int, int>, txMap<int, int>>());
			}
			string posString = openTxtFile(getFileNameNoSuffix(fullFileName, false) + ".txt");
			int posIndex = getLastNumber(getFileNameNoSuffix(fullFileName, true));
			txVector<int> pos;
			stringToIntArray(posString, pos);
			if (pos.size() == 2)
			{
				animationInfo[animation].first.insert(posIndex, pos[0]);
				animationInfo[animation].second.insert(posIndex, pos[1]);
			}
			else
			{
				cout << "位置文件错误：" << getFileNameNoSuffix(fullFileName, false) + ".txt" << endl;
			}
		}
		auto iter = animationInfo.begin();
		auto iterEnd = animationInfo.end();
		for (; iter != iterEnd; ++iter)
		{
			ImagePositionAnimationData animationData;
			animationData.mID = 0;
			animationData.mAtlas = folders[i].substr(strlen("../media/"));
			animationData.mAnimation = iter->first;
			animationData.mFrameCount = iter->second.first.size();
			txVector<int> tempList;
			iter->second.first.valueList(tempList);
			animationData.mPosX = tempList;
			iter->second.second.valueList(tempList);
			animationData.mPosY = tempList;
			txVector<ImagePositionAnimationData*> existDataList;
			sqlite->mSQLiteImagePositionAnimation->query(animationData.mAtlas, animationData.mAnimation, existDataList);
			if (existDataList.size() > 0 && updateIfExist)
			{
				sqlite->mSQLiteImagePositionAnimation->updateData(animationData);
				int count = existDataList.size();
				for (int i = 0; i < count; ++i)
				{
					TRACE_DELETE(existDataList[i]);
				}
			}
			else
			{
				sqlite->mSQLiteImagePositionAnimation->insert(animationData);
			}
		}
	}
	TRACE_DELETE(sqlite);
}

int ImageUtility::tileIndexToTileX(int index, int mapHeight)
{
	return index / mapHeight;
}
int ImageUtility::tileIndexToTileY(int index, int mapHeight)
{
	return index % mapHeight;
}
Vector2i ImageUtility::tileIndexToTilePos(int index, int mapHeight)
{
	return Vector2i(index / mapHeight, index % mapHeight);
}
int ImageUtility::tilePosToTileIndex(int x, int y, int mapHeight)
{
	return x * mapHeight + y;
}
int ImageUtility::pixelPosToTileX(Vector2 pixelPos, int mapHeight, int mapWidth)
{
	int tileX = pixelPosToTilePos(pixelPos, mapHeight).x;
	if (tileX < 0 || tileX >= mapWidth)
	{
		return -1;
	}
	return tileX;
}
int ImageUtility::pixelPosToTileY(Vector2 pixelPos, int mapHeight)
{
	int tileY = pixelPosToTilePos(pixelPos, mapHeight).y;
	if (tileY < 0 || tileY >= mapHeight)
	{
		return -1;
	}
	return tileY;
}
int ImageUtility::pixelPosToTileIndex(Vector2 pixelPos, int mapHeight, int mapWidth)
{
	int tileX = pixelPosToTileX(pixelPos, mapHeight, mapWidth);
	int tileY = pixelPosToTileY(pixelPos, mapHeight);
	if (tileX < 0 || tileY < 0)
	{
		return -1;
	}
	return tilePosToTileIndex(tileX, tileY, mapHeight);
}
// 根据地砖左下角的像素坐标转换为地砖下标的x和y
Vector2i ImageUtility::pixelPosToTilePos(Vector2 pixelPos, int mapHeight)
{
	return Vector2i((int)floor(pixelPos.x * (1.0f / TILE_WIDTH)), (int)floor((mapHeight * TILE_HEIGHT - pixelPos.y) * (1.0f / TILE_HEIGHT)));
}
// 计算出地砖坐标所对应的像素坐标,是地砖左下角的像素坐标
Vector2 ImageUtility::tilePosToPixelPos(int x, int y, int mapHeight)
{
	return Vector2(TILE_WIDTH * (float)x, mapHeight * TILE_HEIGHT - TILE_HEIGHT * (float)y - TILE_HEIGHT);
}
// 计算出地砖下所对应的像素坐标,是地砖左下角的像素坐标
Vector2 ImageUtility::tileIndexToPixelPos(int index, int mapHeight)
{
	int x = index / mapHeight;
	int y = index % mapHeight;
	return tilePosToPixelPos(x, y, mapHeight);
}
// 以地砖左下角为原点的像素坐标所处的三角形下标
TILE_TRIANGLE ImageUtility::pixelPosToTriangleIndex(Vector2 pos)
{
	if (pos.x < 0.0f || pos.x > TILE_WIDTH || pos.y < 0.0f || pos.y > TILE_HEIGHT)
	{
		return TT_MAX;
	}
	// 对角线斜率
	float k = (float)TILE_HEIGHT / TILE_WIDTH;
	// 位于左半部分
	if (pos.x <= TILE_WIDTH * 0.5f)
	{
		// 位于左下部分小矩形中
		if (pos.y <= TILE_HEIGHT * 0.5f)
		{
			// 相对于小矩形的右下角的相对坐标
			Vector2 relative = pos - Vector2(TILE_WIDTH * 0.5f, 0.0f);
			// 根据相对坐标的斜率判断属于哪个三角形
			if (abs(relative.y / relative.x) > k)
			{
				return TT_INNER_LEFT_BOTTOM;
			}
			else
			{
				return TT_LEFT_BOTTOM;
			}
		}
		// 位于左上部分
		else
		{
			// 相对于地砖中左上角小矩形的左下角的相对坐标
			Vector2 relative = pos - Vector2(0.0f, TILE_HEIGHT * 0.5f);
			// 根据相对坐标的斜率判断属于哪个三角形
			if (abs(relative.y / relative.x) > k)
			{
				return TT_LEFT_TOP;
			}
			else
			{
				return TT_INNER_LEFT_TOP;
			}
		}
	}
	// 位于右半部分
	else
	{
		// 位于右下部分
		if (pos.y <= TILE_HEIGHT * 0.5f)
		{
			// 相对于地砖中右下角小矩形的左下角的相对坐标
			Vector2 relative = pos - Vector2(TILE_WIDTH * 0.5f, 0.0f);
			// 根据相对坐标的斜率判断属于哪个三角形
			if (abs(relative.y / relative.x) > k)
			{
				return TT_INNER_RIGHT_BOTTOM;
			}
			else
			{
				return TT_RIGHT_BOTTOM;
			}
		}
		// 位于右上部分
		else
		{
			// 相对于地砖中右上角小矩形的左下角的相对坐标
			Vector2 relative = pos - Vector2(TILE_WIDTH, TILE_HEIGHT * 0.5f);
			// 根据相对坐标的斜率判断属于哪个三角形
			if (abs(relative.y / relative.x) > k)
			{
				return TT_RIGHT_TOP;
			}
			else
			{
				return TT_INNER_RIGHT_TOP;
			}
		}
	}
}
// 获取指定位置上的三角形三个顶点的坐标,坐标是相对于所属地砖左下角
void ImageUtility::getTrianglePoints(TILE_TRIANGLE pos, Vector2& point0, Vector2& point1, Vector2& point2)
{
	if (pos == TT_LEFT_TOP)
	{
		point0 = Vector2(0.0f, TILE_HEIGHT);
		point1 = Vector2(TILE_WIDTH * 0.5f, TILE_HEIGHT);
		point2 = Vector2(0.0f, TILE_HEIGHT * 0.5f);
	}
	else if (pos == TT_RIGHT_TOP)
	{
		point0 = Vector2(TILE_WIDTH * 0.5f, TILE_HEIGHT);
		point1 = Vector2(TILE_WIDTH, TILE_HEIGHT);
		point2 = Vector2(TILE_WIDTH, TILE_HEIGHT * 0.5f);
	}
	else if (pos == TT_RIGHT_BOTTOM)
	{
		point0 = Vector2(TILE_WIDTH, TILE_HEIGHT * 0.5f);
		point1 = Vector2(TILE_WIDTH, 0.0f);
		point2 = Vector2(TILE_WIDTH * 0.5f, 0.0f);
	}
	else if (pos == TT_LEFT_BOTTOM)
	{
		point0 = Vector2(TILE_WIDTH * 0.5f, 0.0f);
		point1 = Vector2(0.0f, 0.0f);
		point2 = Vector2(0.0f, TILE_HEIGHT * 0.5f);
	}
	else if (pos == TT_INNER_LEFT_TOP)
	{
		point0 = Vector2(0.0f, TILE_HEIGHT * 0.5f);
		point1 = Vector2(TILE_WIDTH * 0.5f, TILE_HEIGHT);
		point2 = Vector2(TILE_WIDTH * 0.5f, TILE_HEIGHT * 0.5f);
	}
	else if (pos == TT_INNER_RIGHT_TOP)
	{
		point0 = Vector2(TILE_WIDTH * 0.5f, TILE_HEIGHT * 0.5f);
		point1 = Vector2(TILE_WIDTH * 0.5f, TILE_HEIGHT);
		point2 = Vector2(TILE_WIDTH, TILE_HEIGHT * 0.5f);
	}
	else if (pos == TT_INNER_RIGHT_BOTTOM)
	{
		point0 = Vector2(TILE_WIDTH * 0.5f, TILE_HEIGHT * 0.5f);
		point1 = Vector2(TILE_WIDTH, TILE_HEIGHT * 0.5f);
		point2 = Vector2(TILE_WIDTH * 0.5f, 0.0f);
	}
	else if (pos == TT_INNER_LEFT_BOTTOM)
	{
		point0 = Vector2(TILE_WIDTH * 0.5f, TILE_HEIGHT * 0.5f);
		point1 = Vector2(TILE_WIDTH * 0.5f, 0.0f);
		point2 = Vector2(0.0f, TILE_HEIGHT * 0.5f);
	}
}

void ImageUtility::generateUnreachFile(const string& path)
{
	MapData* mapData = TRACE_NEW(MapData, mapData);
	mapData->readFile(path);
	mapData->writeUnreachFile();
	TRACE_DELETE(mapData);
}

void ImageUtility::generateAllUnreachFile(const string& path)
{
	txVector<string> fileList;
	findFiles(path, fileList, ".map");
	int fileCount = fileList.size();
	for (int i = 0; i < fileCount; ++i)
	{
		generateUnreachFile(fileList[i]);
		print("完成计算阻挡区域:" + getFileName(fileList[i]));
	}
}

void ImageUtility::updateSceneMapTransferSQLite()
{
	SQLite* sqlite = TRACE_NEW(SQLite, sqlite, "../media/DataBase.db");
	txVector<SceneMapTransferData*> transferList;
	sqlite->mSQLiteSceneMapTransfer->queryAll(transferList);
	txMap<int, txVector<int>> sceneTransferList;
	int count = transferList.size();
	for (int i = 0; i < count; ++i)
	{
		if (sceneTransferList.contains(transferList[i]->mSourceMap))
		{
			sceneTransferList.insert(transferList[i]->mSourceMap, txVector<int>());
		}
		sceneTransferList[transferList[i]->mSourceMap].push_back(transferList[i]->mID);
	}
	txVector<SceneMapData*> sceneMapDataList;
	sqlite->mSQLiteSceneMap->queryAll(sceneMapDataList);
	int sceneCount = sceneMapDataList.size();
	for (int i = 0; i < sceneCount; ++i)
	{
		int sceneID = sceneMapDataList[i]->mID;
		if (sceneTransferList.contains(sceneID))
		{
			sceneMapDataList[i]->mTransferPoint = sceneTransferList[sceneID];
		}
		else
		{
			sceneMapDataList[i]->mTransferPoint.clear();
		}
		sqlite->mSQLiteSceneMap->update(*sceneMapDataList[i]);
	}
	TRACE_DELETE(sqlite);
}

void ImageUtility::updateSceneMapNPCSQLite()
{
	SQLite* sqlite = TRACE_NEW(SQLite, sqlite, "../media/DataBase.db");
	txVector<NPCData*> npcList;
	sqlite->mSQLiteNPC->queryAll(npcList);
	txMap<int, txVector<int>> sceneNPCList;
	int count = npcList.size();
	for (int i = 0; i < count; ++i)
	{
		if (sceneNPCList.contains(npcList[i]->mMapID))
		{
			sceneNPCList.insert(npcList[i]->mMapID, txVector<int>());
		}
		sceneNPCList[npcList[i]->mMapID].push_back(npcList[i]->mID);
	}
	txVector<SceneMapData*> sceneMapDataList;
	sqlite->mSQLiteSceneMap->queryAll(sceneMapDataList);
	int sceneCount = sceneMapDataList.size();
	for (int i = 0; i < sceneCount; ++i)
	{
		int sceneID = sceneMapDataList[i]->mID;
		if (sceneNPCList.contains(sceneID))
		{
			sceneMapDataList[i]->mNPC = sceneNPCList[sceneID];
		}
		else
		{
			sceneMapDataList[i]->mNPC.clear();
		}
		sqlite->mSQLiteSceneMap->update(*sceneMapDataList[i]);
	}
	TRACE_DELETE(sqlite);
}

void ImageUtility::updateSceneMapMonsterRegionSQLite()
{
	SQLite* sqlite = TRACE_NEW(SQLite, sqlite, "../media/DataBase.db");
	txVector<MonGenData*> monsterRegionList;
	sqlite->mSQLiteMonGen->queryAll(monsterRegionList);
	txMap<int, txVector<int>> sceneMonsterRegionList;
	int count = monsterRegionList.size();
	for (int i = 0; i < count; ++i)
	{
		if (sceneMonsterRegionList.contains(monsterRegionList[i]->mMapID))
		{
			sceneMonsterRegionList.insert(monsterRegionList[i]->mMapID, txVector<int>());
		}
		sceneMonsterRegionList[monsterRegionList[i]->mMapID].push_back(monsterRegionList[i]->mID);
	}
	txVector<SceneMapData*> sceneMapDataList;
	sqlite->mSQLiteSceneMap->queryAll(sceneMapDataList);
	int sceneCount = sceneMapDataList.size();
	for (int i = 0; i < sceneCount; ++i)
	{
		int sceneID = sceneMapDataList[i]->mID;
		if (sceneMonsterRegionList.contains(sceneID))
		{
			sceneMapDataList[i]->mMonsterRegion = sceneMonsterRegionList[sceneID];
		}
		else
		{
			sceneMapDataList[i]->mMonsterRegion.clear();
		}
		sqlite->mSQLiteSceneMap->update(*sceneMapDataList[i]);
	}
	TRACE_DELETE(sqlite);
}

void ImageUtility::updateSceneMapPeaceAreaSQLite()
{
	SQLite* sqlite = TRACE_NEW(SQLite, sqlite, "../media/DataBase.db");
	txVector<PeaceAreaData*> peaceAreaList;
	sqlite->mSQLitePeaceArea->queryAll(peaceAreaList);
	txMap<int, txVector<int>> scenePeaceAreaList;
	int count = peaceAreaList.size();
	for (int i = 0; i < count; ++i)
	{
		if (scenePeaceAreaList.contains(peaceAreaList[i]->mMapID))
		{
			scenePeaceAreaList.insert(peaceAreaList[i]->mMapID, txVector<int>());
		}
		scenePeaceAreaList[peaceAreaList[i]->mMapID].push_back(peaceAreaList[i]->mID);
	}
	txVector<SceneMapData*> sceneMapDataList;
	sqlite->mSQLiteSceneMap->queryAll(sceneMapDataList);
	int sceneCount = sceneMapDataList.size();
	for (int i = 0; i < sceneCount; ++i)
	{
		int sceneID = sceneMapDataList[i]->mID;
		if (scenePeaceAreaList.contains(sceneID))
		{
			sceneMapDataList[i]->mPeaceArea = scenePeaceAreaList[sceneID];
		}
		else
		{
			sceneMapDataList[i]->mPeaceArea.clear();
		}
		sqlite->mSQLiteSceneMap->update(*sceneMapDataList[i]);
	}
	TRACE_DELETE(sqlite);
}

void ImageUtility::updateNPCGoodsSQLite()
{
	SQLite* sqlite = TRACE_NEW(SQLite, sqlite, "../media/DataBase.db");
	txVector<GoodsData*> goodsList;
	sqlite->mSQLiteGoods->queryAll(goodsList);
	txMap<int, txVector<int>> npcGoodsList;
	int count = goodsList.size();
	for (int i = 0; i < count; ++i)
	{
		if (npcGoodsList.contains(goodsList[i]->mNPC))
		{
			npcGoodsList.insert(goodsList[i]->mNPC, txVector<int>());
		}
		npcGoodsList[goodsList[i]->mNPC].push_back(goodsList[i]->mID);
	}
	txVector<NPCData*> npcDataList;
	sqlite->mSQLiteNPC->queryAll(npcDataList);
	int sceneCount = npcDataList.size();
	for (int i = 0; i < sceneCount; ++i)
	{
		int sceneID = npcDataList[i]->mID;
		if (npcGoodsList.contains(sceneID))
		{
			npcDataList[i]->mGoods = npcGoodsList[sceneID];
		}
		else
		{
			npcDataList[i]->mGoods.clear();
		}
		sqlite->mSQLiteNPC->update(*npcDataList[i]);
	}
	TRACE_DELETE(sqlite);
}

void ImageUtility::processAllShadow(const string& path)
{
	txVector<string> fileList;
	findFiles(path, fileList, ".png");
	int fileCount = fileList.size();
	for (int i = 0; i < fileCount; ++i)
	{
		processShadowVertical(fileList[i]);
		processShadowHorizontal(fileList[i]);
		cout << "已处理文件数:" << i + 1 << "/" << fileCount << endl;
	}
}

void setPixel(BYTE* pixelData, int bpp, const RGBQUAD& rgb)
{
	if (bpp == 24)
	{
		pixelData[0] = rgb.rgbBlue;
		pixelData[1] = rgb.rgbGreen;
		pixelData[2] = rgb.rgbRed;
	}
	else if (bpp == 32)
	{
		pixelData[0] = rgb.rgbBlue;
		pixelData[1] = rgb.rgbGreen;
		pixelData[2] = rgb.rgbRed;
		pixelData[3] = rgb.rgbReserved;
	}
}

RGBQUAD getPixel(BYTE* pixelData, int bpp, RGBQUAD* palette)
{
	RGBQUAD pixel;
	pixel.rgbBlue = 0;
	pixel.rgbGreen = 0;
	pixel.rgbRed = 0;
	pixel.rgbReserved = 0;
	if (bpp == 8)
	{
		if (palette != NULL)
		{
			pixel = palette[*pixelData];
			bool isEmpty = (pixel.rgbBlue + pixel.rgbGreen + pixel.rgbRed) == 0;
			pixel.rgbReserved = isEmpty ? 0 : 255;
		}
	}
	else if (bpp == 24)
	{
		pixel.rgbBlue = pixelData[0];
		pixel.rgbGreen = pixelData[1];
		pixel.rgbRed = pixelData[2];
		pixel.rgbReserved = 255;
	}
	else if (bpp == 32)
	{
		pixel.rgbBlue = pixelData[0];
		pixel.rgbGreen = pixelData[1];
		pixel.rgbRed = pixelData[2];
		pixel.rgbReserved = pixelData[3];
	}
	return pixel;
}

bool isBlack(const RGBQUAD& rgb)
{
	if (rgb.rgbReserved == 127)
	{
		return rgb.rgbBlue == 0 && rgb.rgbGreen == 0 && rgb.rgbRed == 0;
	}
	if (rgb.rgbReserved > 0)
	{
		return (rgb.rgbBlue == 8 && rgb.rgbGreen == 8 && rgb.rgbRed == 16 ||
				rgb.rgbBlue == 0 && rgb.rgbGreen == 0 && rgb.rgbRed == 8 || 
				rgb.rgbBlue == 24 && rgb.rgbGreen == 24 && rgb.rgbRed == 29);
	}
	return false;
}

bool isEmpty(const RGBQUAD& rgb)
{
	return rgb.rgbReserved == 0;
}

RGBQUAD getColor(FIBITMAP* bitmap, int x, int y)
{
	BYTE* line = FreeImage_GetScanLine(bitmap, y);
	RGBQUAD* palette = NULL;
	int bpp = FreeImage_GetBPP(bitmap);
	if (bpp == 8)
	{
		palette = FreeImage_GetPalette(bitmap);
	}
	return getPixel(line + x * bpp / 8, bpp, palette);
}

void setColor(FIBITMAP* bitmap, int x, int y, const RGBQUAD& rgb)
{
	int bpp = FreeImage_GetBPP(bitmap);
	BYTE* line = FreeImage_GetScanLine(bitmap, y);
	setPixel(line + x * bpp / 8, bpp, rgb);
}

void ImageUtility::processShadowHorizontal(const string& filePath)
{
	FreeImage_Initialise(1);
	FREE_IMAGE_FORMAT format = FreeImage_GetFileType(filePath.c_str());
	FIBITMAP* bitmap = FreeImage_Load(format, filePath.c_str());
	if (!FreeImage_IsTransparent(bitmap))
	{
		FreeImage_Unload(bitmap);
		FreeImage_DeInitialise();
		return;
	}
	int width = FreeImage_GetWidth(bitmap);
	int height = FreeImage_GetHeight(bitmap);
	FIBITMAP* newBitmap = FreeImage_Allocate(width, height, 32);
	for (int y = 0; y < height; ++y)
	{
		bool lastShadowMark = false;
		int startIndex = -1;
		txVector<pair<int, int>> shadowBlockList;
		for (int x = 0; x < width; ++x)
		{
			// 检查这一行是否有影子
			// 当前像素为黑色或者透明,0表示两者都不是,1表示黑色,2表示透明
			int blackOrEmpty = 0;
			RGBQUAD rgb = getColor(bitmap, x, y);
			if (isBlack(rgb))
			{
				blackOrEmpty = 1;
			}
			else if(isEmpty(rgb))
			{
				blackOrEmpty = 2;
			}
			// 还未开始阴影部分或者阴影部分已经结束
			if (blackOrEmpty == 0 || x == width - 1)
			{
				// 阴影部分结束,则重新计算开始部分
				if (startIndex >= 0)
				{
					if (x - startIndex > 1)
					{
						shadowBlockList.push_back(make_pair(startIndex, x));
					}
					startIndex = -1;
				}
				continue;
			}
			bool curShadowMark = blackOrEmpty == 1;
			if (startIndex == -1)
			{
				startIndex = x;
				lastShadowMark = curShadowMark;
				continue;
			}
			// 不符合阴影像素的规律,阴影部分结束,则重新计算开始部分
			if (lastShadowMark == curShadowMark)
			{
				if (x - startIndex > 1)
				{
					shadowBlockList.push_back(make_pair(startIndex, x));
				}
				startIndex = -1;
			}
			lastShadowMark = curShadowMark;
		}
		// 拷贝原图,并且重新设置阴影部分颜色
		for (int j = 0; j < width; ++j)
		{
			RGBQUAD pixel = getColor(bitmap, j, y);
			setColor(newBitmap, j, y, pixel);
		}
		RGBQUAD halfTransparent;
		halfTransparent.rgbRed = 0;
		halfTransparent.rgbGreen = 0;
		halfTransparent.rgbBlue = 0;
		halfTransparent.rgbReserved = 127;
		// 重新绘制阴影部分
		int shadowCount = shadowBlockList.size();
		for (int j = 0; j < shadowCount; ++j)
		{
			int start = shadowBlockList[j].first;
			int end = shadowBlockList[j].second;
			for (int k = start; k <= end; ++k)
			{
				setColor(newBitmap, k, y, halfTransparent);
			}
		}
	}
	FreeImage_Save(format, newBitmap, filePath.c_str());
	FreeImage_Unload(bitmap);
	FreeImage_Unload(newBitmap);
	FreeImage_DeInitialise();
}

void ImageUtility::processShadowVertical(const string& filePath)
{
	FreeImage_Initialise(1);
	FREE_IMAGE_FORMAT format = FreeImage_GetFileType(filePath.c_str());
	FIBITMAP* bitmap = FreeImage_Load(format, filePath.c_str());
	if (!FreeImage_IsTransparent(bitmap))
	{
		FreeImage_Unload(bitmap);
		FreeImage_DeInitialise();
		return;
	}
	int width = FreeImage_GetWidth(bitmap);
	int height = FreeImage_GetHeight(bitmap);
	FIBITMAP* newBitmap = FreeImage_Allocate(width, height, 32);
	for (int x = 0; x < width; ++x)
	{
		bool lastShadowMark = false;
		int startIndex = -1;
		txVector<pair<int, int>> shadowBlockList;
		for (int y = 0; y < height; ++y)
		{
			// 检查这一行是否有影子
			// 当前像素为黑色或者透明,0表示两者都不是,1表示黑色,2表示透明
			int blackOrEmpty = 0;
			RGBQUAD rgb = getColor(bitmap, x, y);
			if (isBlack(rgb))
			{
				blackOrEmpty = 1;
			}
			else if (isEmpty(rgb))
			{
				blackOrEmpty = 2;
			}
			// 还未开始阴影部分或者阴影部分已经结束
			if (blackOrEmpty == 0 || y == height - 1)
			{
				// 阴影部分结束,则重新计算开始部分
				if (startIndex >= 0)
				{
					if (y - startIndex > 1)
					{
						shadowBlockList.push_back(make_pair(startIndex, y));
					}
					startIndex = -1;
				}
				continue;
			}
			bool curShadowMark = blackOrEmpty == 1;
			if (startIndex == -1)
			{
				startIndex = y;
				lastShadowMark = curShadowMark;
				continue;
			}
			// 不符合阴影像素的规律,阴影部分结束,则重新计算开始部分
			if (lastShadowMark == curShadowMark)
			{
				if (y - startIndex > 1)
				{
					shadowBlockList.push_back(make_pair(startIndex, y));
				}
				startIndex = -1;
			}
			lastShadowMark = curShadowMark;
		}
		// 拷贝原图,并且重新设置阴影部分颜色
		for (int j = 0; j < height; ++j)
		{
			RGBQUAD pixel = getColor(bitmap, x, j);
			setColor(newBitmap, x, j, pixel);
		}
		RGBQUAD halfTransparent;
		halfTransparent.rgbRed = 0;
		halfTransparent.rgbGreen = 0;
		halfTransparent.rgbBlue = 0;
		halfTransparent.rgbReserved = 127;
		// 重新绘制阴影部分
		int shadowCount = shadowBlockList.size();
		for (int j = 0; j < shadowCount; ++j)
		{
			int start = shadowBlockList[j].first;
			int end = shadowBlockList[j].second;
			for (int k = start; k <= end; ++k)
			{
				setColor(newBitmap, x, k, halfTransparent);
			}
		}
	}
	FreeImage_Save(format, newBitmap, filePath.c_str());
	FreeImage_Unload(bitmap);
	FreeImage_Unload(newBitmap);
	FreeImage_DeInitialise();
}

void ImageUtility::moveMapObjectTexture(const string& sourcePath)
{
	txVector<string> targetFileList;
	findFiles("./", targetFileList);
	int targetFileCount = targetFileList.size();
	txVector<string> sourceFileList;
	findFiles(sourcePath, sourceFileList);
	int sourceFileCount = sourceFileList.size();
	for (int i = 0; i < sourceFileCount; ++i)
	{
		// 找到同名且不同路径的文件
		string sourceFileName = getFileName(sourceFileList[i]);
		string sourceFileFolder = getFolderName(sourceFileList[i]);
		for (int j = 0; j < targetFileCount; ++j)
		{
			string targetFileName = getFileName(targetFileList[j]);
			string targetFileFolder = getFolderName(targetFileList[j]);
			if (targetFileName == sourceFileName && sourceFileFolder != targetFileFolder)
			{
				moveFile(sourceFileList[i], targetFileList[j]);
				break;
			}
		}
	}
}

void ImageUtility::updateMapEffect()
{
	txVector<pair<int, int>> effectImage;
	// 查找特效序列帧所在目录
	txVector<string> folders;
	findFolders("../media/", folders, true);
	int folderCount = folders.size();
	for (int i = 0; i < folderCount; ++i)
	{
		txVector<string> files;
		findFiles(folders[i], files, ".png", false);
		int fileCount = files.size();
		if (fileCount == 0)
		{
			continue;
		}
		string effectIndex = getFolderName(files[0]);
		string effectFileIndex = getFolderName(getFilePath(files[0]));
		// 因为只有地图中存储的图片下标是从1开始的,其他所有地方都是从0开始的,所以此处图片下标+1
		effectImage.push_back(make_pair(stringToInt(effectFileIndex), stringToInt(effectIndex) + 1));
	}
	// 检查所有地图文件,找出所有用到了特效的地方,然后保存到表格中
	txVector<MapEffectData*> mapEffectList;
	int effectCount = effectImage.size();
	SQLite* sqlite = TRACE_NEW(SQLite, sqlite, "../media/DataBase.db");
	txVector<SceneMapFileData*> mapFileDataList;
	sqlite->mSQLiteSceneMapFile->queryAll(mapFileDataList);
	int fileDataCount = mapFileDataList.size();
	for (int i = 0; i < fileDataCount; ++i)
	{
		MapData* mapData = TRACE_NEW(MapData, mapData);
		mapData->readFile("../media/" + mapFileDataList[i]->mFileName);
		int tileCount = mapData->mHeader->mWidth * mapData->mHeader->mHeight;
		for (int j = 0; j < tileCount; ++j)
		{
			MapTile& tile = mapData->mTileList[j];
			Vector2i tilePos = tileIndexToTilePos(j, mapData->mHeader->mHeight);
			for (int k = 0; k < effectCount; ++k)
			{
				if (tile.mObjFileIdx == effectImage[k].first &&
					tile.mObjImgIdx == effectImage[k].second)
				{
					MapEffectData* effectData = TRACE_NEW(MapEffectData, effectData);
					effectData->mID = mapEffectList.size() + 1;
					effectData->mMapFileID = mapFileDataList[i]->mID;
					effectData->mTilePosition = tilePos;
					effectData->mEffectFileIndex = tile.mObjFileIdx;
					effectData->mEffectIndex = tile.mObjImgIdx - 1;
					effectData->mSpeed = 0.5f;
					effectData->mBlendType = 0;
					mapEffectList.push_back(effectData);
					break;
				}
			}
		}
		TRACE_DELETE(mapData);
	}
	sqlite->mSQLiteMapEffect->deleteAll();
	int mapEffectCount = mapEffectList.size();
	for (int i = 0; i < mapEffectCount; ++i)
	{
		sqlite->mSQLiteMapEffect->insert(*mapEffectList[i]);
	}
	TRACE_DELETE(sqlite);
}
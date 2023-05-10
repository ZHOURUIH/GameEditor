#include "txSerializer.h"
#include "HumanImage.h"
#include "SQLiteHeader.h"
#include "HumanAction.h"
#include "MapData.h"
#include "MapDataSimple.h"
#include "MapTileSimple.h"
#include "MapHeader.h"
#include "MapTile.h"
#include "UnreachTileGroup.h"
#include "UnreachTileGroupSimple.h"
#include "ImageUtility.h"

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
	FileContent file;
	openBinaryFile(filePath, file);
	if (file.mBuffer == NULL)
	{
		return false;
	}
	txSerializer serializer(file.mBuffer, file.mFileSize);
	serializer.readBuffer(info.mStrHeader, 44);
	serializer.read(info.mIndexCount);
	FOR_I(info.mIndexCount)
	{
		int curStartPos = 0;
		if (serializer.read(curStartPos))
		{
			info.mPositionList.push_back(curStartPos);
		}
	}
	return true;
}

bool ImageUtility::readWilHeader(const string& filePath, WILFileHeader& header)
{
	FileContent file;
	openBinaryFile(filePath, file);
	if (file.mBuffer == NULL)
	{
		return false;
	}
	txSerializer serializer(file.mBuffer, file.mFileSize);
	serializer.readBuffer(header.mInfo, 44);
	serializer.read(header.mImageCount);
	serializer.read(header.mColorCount);
	serializer.read(header.mColorPadSize);
	NEW_ARRAY(unsigned char, header.mColorPadSize, header.mColor);
	memcpy(header.mColor, serializer.getBuffer() + serializer.getIndex(), header.mColorPadSize);
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
	string fileNameNoSuffix = getFileNameNoSuffix(wilFileName, true);
	cout << "正在解析" << fileNameNoSuffix << ", 图片数量 : " << wilHeader.mImageCount << ", 颜色位数 : " << bpp << endl;
	POINT* posList = NEW_ARRAY(POINT, wixFileHeader.mPositionList.size(), posList);
	FileContent file;
	openBinaryFile(wilFileName, file);
	txSerializer serializer(file.mBuffer, file.mFileSize);
	FOR_I(wixFileHeader.mPositionList.size())
	{
		// 将下标设置到当前图片起始位置,并且读取图片信息
		int startPos = wixFileHeader.mPositionList[i];
		if (startPos == 0)
		{
			continue;
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
		NEW_ARRAY(unsigned char, pixelCount * 4, curInfo.mColor);
		FOR_Y(curInfo.mHeight)
		{
			FOR_X(curInfo.mWidth)
			{
				int pos = y * lineLength + x;
				int pngPos = y * curInfo.mWidth + x;
				// 8bit
				if (wilHeader.mColorCount == 0xFF + 1)
				{
					unsigned char pixelIndex = file.mBuffer[startPos + ImageHeaderLength + pos];
					// 0蓝,1绿,2红
					curInfo.mColor[pngPos * 4 + 0] = wilHeader.mColor[pixelIndex * 4 + 0];
					curInfo.mColor[pngPos * 4 + 1] = wilHeader.mColor[pixelIndex * 4 + 1];
					curInfo.mColor[pngPos * 4 + 2] = wilHeader.mColor[pixelIndex * 4 + 2];
					curInfo.mColor[pngPos * 4 + 3] = pixelIndex == 0 ? 0 : (char)255;
				}
				// 16bit
				else if (wilHeader.mColorCount == 0xFFFF + 1)
				{
					unsigned short pixel = *(unsigned short*)(file.mBuffer + startPos + ImageHeaderLength + pos * 2);
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
		DELETE_ARRAY(curInfo.mColor);
	}

	writePositionFile(outputPath + "position.txt", posList, wixFileHeader.mPositionList.size());
	DELETE_ARRAY(posList);
}

void ImageUtility::allWixWilToPNG(const string& sourcePath)
{
	myVector<string> fileList;
	myVector<string> patterns;
	patterns.push_back(".wil");
	findFiles(sourcePath, fileList, patterns.data(), patterns.size());
	int fileCount = fileList.size();
	FOR_VECTOR(fileList)
	{
		string wixFile = getFileNameNoSuffix(fileList[i], false) + ".wix";
		string filePath = getFilePath(fileList[i]);
		string folderName = getFileNameNoSuffix(fileList[i], true);
		wixWilToPNG(wixFile, fileList[i], filePath + "/" + folderName + "/");
	}
	END(fileList);
}

void ImageUtility::writePositionFile(const string& positionFile, POINT* posList, int posCount)
{
	string posStr;
	FOR_I(posCount)
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
	string posStr = openTxtFile(positionFile, false);
	if (posStr == "")
	{
		return NULL;
	}
	myVector<string> posStrList;
	split(posStr.c_str(), "\n", posStrList);
	posCount = posStrList.size();
	POINT* posList = NEW_ARRAY(POINT, posCount, posList);
	FOR_VECTOR(posStrList)
	{
		myVector<string> pointList;
		split(posStrList[i].c_str(), ",", pointList);
		if (pointList.size() != 2)
		{
			continue;
		}
		posList[i].x = stringToInt(pointList[0]);
		posList[i].y = stringToInt(pointList[1]);
	}
	END(posStrList);
	return posList;
}

void ImageUtility::autoGroupHumanImage(const string& path)
{
	// 按照动作重命名
	myVector<string> fileList;
	findFiles(path, fileList, ".png", false);
	// 一个目录中的文件数量只能是600个
	if (fileList.size() != HUMAN_GROUP_SIZE)
	{
		cout << "目录中的图片数量错误,请确保图片数量为600个,且不包含子目录" << endl;
		return;
	}
	sortByFileNumber(fileList);
	FOR_VECTOR(fileList)
	{
		string actionName;
		int direction;
		int frameIndex;
		getActionInfo(HUMAN_ACTION, HUMAN_ACTION_COUNT, i, actionName, direction, frameIndex);
		const string& filePath = fileList[i];
		const string fileName = actionName + "_dir" + intToString(direction) + "_" + intToString(frameIndex) + "." + getFileSuffix(filePath);
		moveImageWithPosition(filePath, getFilePath(filePath) + "/" + fileName);
	}
	END(fileList);
}

void ImageUtility::autoGroupMonsterImage(const string& path)
{
	myVector<string> pngFiles;
	findFiles(path, pngFiles, ".png");
	if (pngFiles.size() != MONSTER_GROUP_SIZE)
	{
		cout << "文件夹中的图片数量不是" << MONSTER_GROUP_SIZE << ",不是标准的怪物图片数量" << endl;
		return;
	}
	myVector<string> posFiles;
	findFiles(path, posFiles, ".txt");
	if (posFiles.size() != MONSTER_GROUP_SIZE)
	{
		cout << "文件夹中的位置文件数量不是" << MONSTER_GROUP_SIZE << ",请确保每个图片都有对应的位置文件" << endl;
		return;
	}
	sortByFileNumber(pngFiles);
	FOR_VECTOR(pngFiles)
	{
		string actionName;
		int direction;
		int frameIndex;
		if (!getActionInfo(MONSTER_ACTION, MONSTER_ACTION_COUNT, i, actionName, direction, frameIndex))
		{
			// 无效文件则需要删除
			deleteImageWithPosition(pngFiles[i]);
			continue;
		}
		const string& filePath = pngFiles[i];
		const string fileName = actionName + "_dir" + intToString(direction) + "_" + intToString(frameIndex) + "." + getFileSuffix(filePath);
		moveImageWithPosition(filePath, getFilePath(filePath) + "/" + fileName);
	}
	END(pngFiles);
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

	myVector<string> folderList;
	findFolders(path, folderList);
	FOR_VECTOR(folderList)
	{
		// 按照动作重命名
		myVector<string> fileList;
		findFiles(folderList[i], fileList, ".png", false);
		sortByFileNumber(fileList);
		FOR_VECTOR_J(fileList)
		{
			string actionName;
			int direction;
			int frameIndex;
			bool isValid = getActionInfo(NPC_ACTION, NPC_ACTION_COUNT, j, actionName, direction, frameIndex);
			// 因为一组动作资源包含了3个方向上的所有动作,所以可以根据下标计算出方向和序列帧下标,前提是保留了空图片作为填充位置
			direction += 3;
			// 如果是无效图片则需要删除
			if (!isValid)
			{
				deleteImageWithPosition(fileList[j]);
				continue;
			}
			string actionFolderName = actionName + "_dir" + intToString(direction);
			string destPath = getFilePath(fileList[j]) + "/";
			moveImageWithPosition(fileList[j], destPath + actionFolderName + "_" + intToString(frameIndex) + getFileSuffix(fileList[j]));
		}
		END(fileList);
	}
	END(folderList);
}

void ImageUtility::renameImageToNumber(const string& path)
{
	// 将目录中的文件按文件名排序后,重命名为从0开始的数字
	myVector<string> folderList;
	findFolders(path, folderList, true);
	FOR_VECTOR(folderList)
	{
		myVector<string> fileList;
		findFiles(folderList[i], fileList, ".png");
		// 先根据文件名重新排列
		sortByFileNumber(fileList);
		FOR_VECTOR_J(fileList)
		{
			string curFilePath = getFilePath(fileList[j]) + "/";
			string suffix = getFileSuffix(fileList[j]);
			renameImageWithPosition(fileList[j], curFilePath + intToString(j) + "." + suffix);
		}
		END(fileList);
	}
	END(folderList);
}

void ImageUtility::renameImageToAnim(const string& path)
{
	// 将目录中的文件按文件名排序后,重命名为序列帧格式的名字,以文件夹名开头,以从0开始的数字结尾
	myVector<string> folderList;
	folderList.push_back(path);
	findFolders(path, folderList, true);
	int folderCount = folderList.size();
	FOR_VECTOR(folderList)
	{
		myVector<string> fileList;
		findFiles(folderList[i], fileList, ".png", false);
		if (fileList.size() == 0)
		{
			continue;
		}
		// 先根据文件名重新排列
		sortByFileNumber(fileList);
		FOR_VECTOR_J(fileList)
		{
			string folderName = getFolderName(fileList[j]);
			string curFilePath = getFilePath(fileList[j]) + "/";
			string suffix = getFileSuffix(fileList[j]);
			renameImageWithPosition(fileList[j], curFilePath + folderName + "_" + intToString(j) + "." + suffix);
		}
		END(fileList);
	}
	END(folderList);
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
	FOR_I(posCount)
	{
		string posStr = intToString(posList[i].x) + "," + intToString(posList[i].y);
		writeFile(filePath + "/" + intToString(i) + ".txt", posStr);
	}
	DELETE_ARRAY(posList);
}

void ImageUtility::renameByDirection(const string& path, int directionCount, bool autoGroup)
{
	// 将目录中的所有文件先按照文件名排序,然后按照顺序分组为8个方向,再对每个方向的文件重命名
	myVector<string> folderList;
	findFolders(path, folderList, true);
	FOR_VECTOR(folderList)
	{
		myVector<string> fileList;
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
		FOR_J(fileCount)
		{
			int imageDir = j / actionFrameCount;
			int index = j % actionFrameCount;
			// 把文件移动到一个新建文件夹中
			string curPath = getFilePath(fileList[j]) + "/";
			if (autoGroup)
			{
				string destPath = getFilePath(curPath) + "/";
				string destFileName = getFolderName(fileList[j]) + "_dir" + intToString(imageDir) + "_" + intToString(index) + "." + getFileSuffix(fileList[j]);
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
	END(folderList);
	// 删除空的目录
	deleteEmptyFolder(path);
}

void ImageUtility::sortByFileNumber(myVector<string>& fileList, bool fileNameIsNumber)
{
	// 根据文件名的数字进行排序
	int count = fileList.size();
	if (fileNameIsNumber)
	{
		myMap<int, string> sortedList;
		FOR_I(count)
		{
			sortedList.insert(stringToInt(getFileNameNoSuffix(fileList[i], true)), fileList[i]);
		}
		if (sortedList.size() != fileList.size())
		{
			return;
		}
		valueToList(sortedList, fileList);
	}
	else
	{
		myMap<string, myMap<int, string>> sortedList;
		FOR_I(count)
		{
			string fileName = getFileNameNoSuffix(fileList[i], true);
			string preName = fileName.substr(0, fileName.find_last_of('_') + 1);
			if (!sortedList.contains(preName))
			{
				sortedList.insert(preName, myMap<int, string>());
			}
			string suffixNumber = fileName.substr(fileName.find_last_of('_') + 1);
			sortedList[preName].insert(stringToInt(suffixNumber), fileList[i]);
		}
		fileList.clear();
		FOREACH(iter, sortedList)
		{
			valueToList(iter->second, fileList, false);
		}
		END(sortedList);
	}
}

void ImageUtility::autoMoveFile(const string& path, int groupSize)
{
	myVector<string> fileList;
	findFiles(path, fileList, ".png");
	sortByFileNumber(fileList);
	FOR_VECTOR(fileList)
	{
		int groupIndex = i / groupSize;
		string destFolderName = intToString(groupIndex, 3);
		string destPath = getFilePath(fileList[i]) + "/" + destFolderName + "/";
		moveImageWithPosition(fileList[i], destPath + getFileName(fileList[i]));
	}
	END(fileList);
}

bool ImageUtility::getActionInfo(ActionInfo* actionInfo, int actionCount, int index, string& actionName, int& dir, int& frameIndex)
{
	int actionIndex = 0;
	while (true)
	{
		if (index - actionInfo[actionIndex].mActionImageCount < 0)
		{
			break;
		}
		index -= actionInfo[actionIndex].mActionImageCount;
		++actionIndex;
	}
	if (actionIndex >= actionCount)
	{
		return false;
	}
	// 因为一组动作资源包含了8个方向上的所有动作,所以可以根据下标计算出方向和序列帧下标,前提是保留了空图片作为填充位置
	dir = index / actionInfo[actionIndex].mMaxFrame;
	frameIndex = index % actionInfo[actionIndex].mMaxFrame;
	actionName = actionInfo[actionIndex].mName;
	return true;
}

void ImageUtility::moveImageWithPosition(const string& fullFileName, const string& destFullFileName)
{
	string sourceFileNameNoSuffix = getFileNameNoSuffix(fullFileName, true);
	string destFileNameNoSuffix = getFileNameNoSuffix(destFullFileName, true);
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
	string sourceFileNameNoSuffix = getFileNameNoSuffix(fullFileName, true);
	string destFileNameNoSuffix = getFileNameNoSuffix(destFullFileName, true);
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
	string sourceFileNameNoSuffix = getFileNameNoSuffix(fullFileName, true);
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
	myVector<string> fileList;
	findFiles(path, fileList, ".png");
	sortByFileNumber(fileList);
	FOR_VECTOR(fileList)
	{
		if (!isInvalidImage(fileList[i]))
		{
			deleteImageWithPosition(fileList[i]);
		}
	}
	END(fileList);
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
	string posFileName = getFilePath(imageFullPath) + "/" + getFileNameNoSuffix(imageFullPath, true) + ".txt";
	string posFile = openTxtFile(posFileName, false);
	myVector<int> posValue;
	stringToInts(posFile, posValue);
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

void ImageUtility::texturePackerAll(const string& texturePath)
{
	myVector<string> folderList;
	folderList.push_back(texturePath);
	findFolders(texturePath, folderList, true);
	FOR_VECTOR(folderList)
	{
		myVector<string> temp;
		findFiles(folderList[i], temp, ".png", false);
		if (temp.size() > 0)
		{
			string fullPathNoMedia = removeStartString(folderList[i], "../media/");
			string rootFolderName = getFirstFolderName(fullPathNoMedia);
			string newFullPath = "../media/" + rootFolderName + "_atlas" + "/" + removeFirstPath(fullPathNoMedia);
			packAtlas(getFilePath(newFullPath), getFileName(newFullPath), folderList[i]);
			if (isFileExist(newFullPath + ".png"))
			{
				cout << "已打包:" << i + 1 << "/" << folderListCount << ",打包成功" << endl;
			}
			else
			{
				cout << "已打包:" << i + 1 << "/" << folderListCount << ",打包失败" << endl;
			}
		}
		else
		{
			cout << "跳过空目录" << endl;
		}
	}
	END(folderList);
}

void ImageUtility::writeAnimFrameSQLite()
{
	NEW_SQLITE(SQLiteImagePositionAnimation, imagePosTable, "ImagePositionAnimation");
	myVector<string> folders;
	findFolders("../media", folders, true);
	FOR_VECTOR(folders)
	{
		myVector<string> pngFiles;
		findFiles(folders[i], pngFiles, ".png", false);
		if (pngFiles.size() == 0)
		{
			continue;
		}
		// 1个Atlas中包含多个动画,一个动画有多个帧
		myMap<string, pair<myMap<int, int>, myMap<int, int>>> animationInfo;
		int pngCount = pngFiles.size();
		FOR_VECTOR_J(pngFiles)
		{
			string& fullFileName = pngFiles[j];
			string fileNameNoSuffix = getFileNameNoSuffix(fullFileName, true);
			string animation = fileNameNoSuffix.substr(0, getFileNameNoSuffix(fullFileName, true).find_last_of('_'));
			if (!animationInfo.contains(animation))
			{
				animationInfo.insert(animation, pair<myMap<int, int>, myMap<int, int>>());
			}
			string posString = openTxtFile(getFileNameNoSuffix(fullFileName, false) + ".txt", false);
			replaceAll(posString, "\r", "");
			int posIndex = getLastNumber(getFileNameNoSuffix(fullFileName, true));
			myVector<int> pos;
			stringToInts(posString, pos);
			if (pos.size() != 2)
			{
				pos.clear();
				stringToInts(posString, pos, "\n");
			}
			if (pos.size() == 2)
			{
				animationInfo[animation].first.insert(posIndex, pos[0]);
				animationInfo[animation].second.insert(posIndex, pos[1]);
			}
			else
			{
				cout << "位置文件错误或者找不到此图片的同名位置文件：" << getFileNameNoSuffix(fullFileName, false) + ".txt" << endl;
			}
		}
		END(pngFiles);
		FOREACH(iter, animationInfo)
		{
			TDImagePositionAnimation animationData;
			animationData.mID = 0;
			// 此处多一层目录是因为一般一个图集都会在单独的目录中,且此目录与图集同名
			animationData.mAtlas = folders[i].substr(strlen("../media/")) + ".png";
			animationData.mAnimation = iter->first;
			valueToList(iter->second.first, animationData.mPosX);
			valueToList(iter->second.second, animationData.mPosY);
			TDImagePositionAnimation existData;
			imagePosTable.query(animationData.mAtlas, animationData.mAnimation, existData);
			if (existData.mID > 0)
			{
				imagePosTable.update(animationData);
			}
			else
			{
				animationData.mID = imagePosTable.getMaxID() + 1;
				imagePosTable.insert(animationData);
			}
		}
		END(animationInfo);
	}
	END(folders);
}

int ImageUtility::tileIndexToTileX(int index, int mapHeight)
{
	return index / mapHeight;
}
int ImageUtility::tileIndexToTileY(int index, int mapHeight)
{
	return index % mapHeight;
}
Vector2Int ImageUtility::tileIndexToTilePos(int index, int mapHeight)
{
	return Vector2Int(index / mapHeight, index % mapHeight);
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
Vector2Int ImageUtility::pixelPosToTilePos(Vector2 pixelPos, int mapHeight)
{
	return Vector2Int((int)floor(pixelPos.x * (1.0f / TILE_WIDTH)), (int)floor((mapHeight * TILE_HEIGHT - pixelPos.y) * (1.0f / TILE_HEIGHT)));
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
		return TILE_TRIANGLE::MAX;
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
				return TILE_TRIANGLE::INNER_LEFT_BOTTOM;
			}
			else
			{
				return TILE_TRIANGLE::LEFT_BOTTOM;
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
				return TILE_TRIANGLE::LEFT_TOP;
			}
			else
			{
				return TILE_TRIANGLE::INNER_LEFT_TOP;
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
				return TILE_TRIANGLE::INNER_RIGHT_BOTTOM;
			}
			else
			{
				return TILE_TRIANGLE::RIGHT_BOTTOM;
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
				return TILE_TRIANGLE::RIGHT_TOP;
			}
			else
			{
				return TILE_TRIANGLE::INNER_RIGHT_TOP;
			}
		}
	}
}
// 获取指定位置上的三角形三个顶点的坐标,坐标是相对于所属地砖左下角
void ImageUtility::getTrianglePoints(TILE_TRIANGLE pos, Vector2& point0, Vector2& point1, Vector2& point2)
{
	if (pos == TILE_TRIANGLE::LEFT_TOP)
	{
		point0 = Vector2(0.0f, TILE_HEIGHT);
		point1 = Vector2(TILE_WIDTH * 0.5f, TILE_HEIGHT);
		point2 = Vector2(0.0f, TILE_HEIGHT * 0.5f);
	}
	else if (pos == TILE_TRIANGLE::RIGHT_TOP)
	{
		point0 = Vector2(TILE_WIDTH * 0.5f, TILE_HEIGHT);
		point1 = Vector2(TILE_WIDTH, TILE_HEIGHT);
		point2 = Vector2(TILE_WIDTH, TILE_HEIGHT * 0.5f);
	}
	else if (pos == TILE_TRIANGLE::RIGHT_BOTTOM)
	{
		point0 = Vector2(TILE_WIDTH, TILE_HEIGHT * 0.5f);
		point1 = Vector2(TILE_WIDTH, 0.0f);
		point2 = Vector2(TILE_WIDTH * 0.5f, 0.0f);
	}
	else if (pos == TILE_TRIANGLE::LEFT_BOTTOM)
	{
		point0 = Vector2(TILE_WIDTH * 0.5f, 0.0f);
		point1 = Vector2(0.0f, 0.0f);
		point2 = Vector2(0.0f, TILE_HEIGHT * 0.5f);
	}
	else if (pos == TILE_TRIANGLE::INNER_LEFT_TOP)
	{
		point0 = Vector2(0.0f, TILE_HEIGHT * 0.5f);
		point1 = Vector2(TILE_WIDTH * 0.5f, TILE_HEIGHT);
		point2 = Vector2(TILE_WIDTH * 0.5f, TILE_HEIGHT * 0.5f);
	}
	else if (pos == TILE_TRIANGLE::INNER_RIGHT_TOP)
	{
		point0 = Vector2(TILE_WIDTH * 0.5f, TILE_HEIGHT * 0.5f);
		point1 = Vector2(TILE_WIDTH * 0.5f, TILE_HEIGHT);
		point2 = Vector2(TILE_WIDTH, TILE_HEIGHT * 0.5f);
	}
	else if (pos == TILE_TRIANGLE::INNER_RIGHT_BOTTOM)
	{
		point0 = Vector2(TILE_WIDTH * 0.5f, TILE_HEIGHT * 0.5f);
		point1 = Vector2(TILE_WIDTH, TILE_HEIGHT * 0.5f);
		point2 = Vector2(TILE_WIDTH * 0.5f, 0.0f);
	}
	else if (pos == TILE_TRIANGLE::INNER_LEFT_BOTTOM)
	{
		point0 = Vector2(TILE_WIDTH * 0.5f, TILE_HEIGHT * 0.5f);
		point1 = Vector2(TILE_WIDTH * 0.5f, 0.0f);
		point2 = Vector2(0.0f, TILE_HEIGHT * 0.5f);
	}
}

void ImageUtility::generateAllUnreachFile(const string& path)
{
	myVector<string> fileList;
	findFiles(path, fileList, ".map");
	for (const string& file : fileList)
	{
		MapData mapData;
		mapData.readFile(file);
		mapData.writeUnreachFile();
		print("完成计算阻挡区域:" + getFileName(file));
	}
}

void ImageUtility::processAllShadow(const string& path)
{
	myVector<string> fileList;
	findFiles(path, fileList, ".png");
	FOR_VECTOR(fileList)
	{
		processShadowVertical(fileList[i]);
		processShadowHorizontal(fileList[i]);
		cout << "已处理文件数:" << i + 1 << "/" << fileListCount << endl;
	}
	END(fileList);
}

void ImageUtility::setPixel(BYTE* pixelData, int bpp, const RGBQUAD& rgb)
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

RGBQUAD ImageUtility::getPixel(BYTE* pixelData, int bpp, RGBQUAD* palette)
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

bool ImageUtility::isBlack(const RGBQUAD& rgb)
{
	if (rgb.rgbReserved == 127)
	{
		return rgb.rgbBlue == 0 && rgb.rgbGreen == 0 && rgb.rgbRed == 0;
	}
	if (rgb.rgbReserved > 0)
	{
		return (rgb.rgbBlue == 0 && rgb.rgbGreen == 4 && rgb.rgbRed == 0 || 
				rgb.rgbBlue == 8 && rgb.rgbGreen == 8 && rgb.rgbRed == 8 ||
				rgb.rgbBlue == 8 && rgb.rgbGreen == 8 && rgb.rgbRed == 16 ||
				rgb.rgbBlue == 0 && rgb.rgbGreen == 0 && rgb.rgbRed == 8 || 
				rgb.rgbBlue == 16 && rgb.rgbGreen == 24 && rgb.rgbRed == 33 ||
				rgb.rgbBlue == 24 && rgb.rgbGreen == 24 && rgb.rgbRed == 29 ||
				rgb.rgbBlue == 9 && rgb.rgbGreen == 9 && rgb.rgbRed == 15);
	}
	return false;
}

bool ImageUtility::isEmpty(const RGBQUAD& rgb)
{
	return rgb.rgbReserved == 0;
}

RGBQUAD ImageUtility::getColor(FIBITMAP* bitmap, int x, int y)
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

void ImageUtility::setColor(FIBITMAP* bitmap, int x, int y, const RGBQUAD& rgb)
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
	FOR_Y(height)
	{
		bool lastShadowMark = false;
		int startIndex = -1;
		myVector<pair<int, int>> shadowBlockList;
		FOR_X(width)
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
		FOR_J(width)
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
		FOR_VECTOR_J(shadowBlockList)
		{
			int start = shadowBlockList[j].first;
			int end = shadowBlockList[j].second;
			for (int k = start; k <= end; ++k)
			{
				setColor(newBitmap, k, y, halfTransparent);
			}
		}
		END(shadowBlockList);
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
	FOR_X(width)
	{
		bool lastShadowMark = false;
		int startIndex = -1;
		myVector<pair<int, int>> shadowBlockList;
		FOR_Y(height)
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
		FOR_J(height)
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
		FOR_VECTOR(shadowBlockList)
		{
			int start = shadowBlockList[i].first;
			int end = shadowBlockList[i].second;
			for (int k = start; k <= end; ++k)
			{
				setColor(newBitmap, x, k, halfTransparent);
			}
		}
		END(shadowBlockList);
	}
	FreeImage_Save(format, newBitmap, filePath.c_str());
	FreeImage_Unload(bitmap);
	FreeImage_Unload(newBitmap);
	FreeImage_DeInitialise();
}

void ImageUtility::writeMapEffectToSQLite(int mapFileID, const string& mapFileName)
{
	NEW_SQLITE(SQLiteMapEffect, mapEffectTable, "MapEffect");
	int maxID = mapEffectTable.getMaxID();

	// 检查所有地图文件,找出所有用到了特效的地方,然后保存到表格中
	myVector<TDMapEffect*> mapEffectList;
	MapData mapData;
	mapData.readFile("../media/" + mapFileName);
	int tileCount = mapData.mHeader->mWidth * mapData.mHeader->mHeight;
	FOR_J(tileCount)
	{
		const MapTile& tile = mapData.mTileList[j];
		if (tile.mAniFrame > 0 && tile.mObjImgIdx > 0)
		{
			TDMapEffect* effectData = NEW(TDMapEffect, effectData);
			effectData->mID = ++maxID;
			effectData->mMapFileID = mapFileID;
			effectData->mTilePosition = tileIndexToTilePos(j, mapData.mHeader->mHeight);
			effectData->mEffectAtlas = intToString(tile.mObjImgIdx - 1) + ".png";
			effectData->mSpeed = 0.5f;
			effectData->mBlendType = 0;
			mapEffectList.push_back(effectData);
		}
	}

	FOR_VECTOR(mapEffectList)
	{
		mapEffectTable.insert(*mapEffectList[i]);
	}
	END(mapEffectList);
}

struct AnimInfo
{
	string mFileName;
	string mAtlas;
	int mIndex;
	Vector2Int mOffset;
	AnimInfo(const string& fileName, const string& atlas, int index, const Vector2Int& offset)
	{
		mFileName = fileName;
		mAtlas = atlas;
		mIndex = index;
		mOffset = offset;
	}
};

void ImageUtility::generateGroupImage(const string& filePath)
{
	myVector<string> EmptyList;
	myVector<string> folders;
	findFolders(filePath, folders, true);
	FOR_VECTOR(folders)
	{
		myVector<string> files;
		findFiles(folders[i], files, ".png", false);
		if (files.size() == 0)
		{
			continue;
		}
		if (getFileName(folders[i]).find_first_of('_') != -1)
		{
			continue;
		}
		// 先将所有图片按照序列帧放入列表中
		myMap<string, myVector<string>> animList;
		FOR_VECTOR_J(files)
		{
			string fileName = files[j];
			string fileNameNoSuffix = getFileNameNoSuffix(fileName, true);
			string animationWithDir = fileNameNoSuffix.substr(0, findStringPos(fileNameNoSuffix, "_"));
			string animationNoDir = animationWithDir.substr(0, findStringPos(animationWithDir, "_"));
			animList.tryInsert(animationNoDir, EmptyList).push_back(fileName);
		}

		// 再对序列帧列表进行处理
		FOREACH(iterAnim, animList)
		{
			auto& list = iterAnim->second;
			FOR_VECTOR_J(list)
			{
				string path = getFilePath(list[j]) + "/" + iterAnim->first + "/";
				moveFile(list[j], path + getFileName(list[j]));
			}
			END(list);
		}
		END(animList);
	}
	END(folders);
}

void ImageUtility::generateGroupMonsterImage(const string& filePath)
{
	myVector<string> EmptyList;
	myVector<string> folders;
	findFolders(filePath, folders, true);
	FOR_VECTOR(folders)
	{
		myVector<string> files;
		findFiles(folders[i], files, ".png", false);
		if (files.size() == 0)
		{
			continue;
		}
		if (getFileName(folders[i]).find_first_of('_') != -1)
		{
			continue;
		}
		
		// 先将所有图片按照序列帧放入列表中,first是atlas,second是图集中的带路径的图片名字
		myMap<string, myVector<string>> animList;
		FOR_VECTOR_J(files)
		{
			string fileName = files[j];
			string fileNameNoSuffix = getFileNameNoSuffix(fileName, true);
			
			// 带方向的
			string atlas = "";
			int dirCharIndex = findStringPos(fileNameNoSuffix, "_dir");
			if (dirCharIndex != -1)
			{
				int seperateIndexAfterDir = findStringPos(fileNameNoSuffix, "_", dirCharIndex + strlen("_dir"));
				atlas = fileNameNoSuffix.substr(0, seperateIndexAfterDir);
			}
			// 不带方向
			else
			{
				atlas = fileNameNoSuffix.substr(0, findStringPos(fileNameNoSuffix, "_"));
			}
			animList.tryInsert(atlas, EmptyList).push_back(fileName);
		}

		// 再对序列帧列表进行处理
		FOREACH(iterAnim, animList)
		{
			auto& list = iterAnim->second;
			FOR_VECTOR_J(list)
			{
				string path = getFilePath(getFilePath(list[j])) + "/" + iterAnim->first + "/";
				moveFile(list[j], path + getFileName(list[j]));
			}
			END(list);
		}
		END(animList);
	}
	END(folders);
	deleteEmptyFolder(filePath);
}

void ImageUtility::generateAllOffsetedImage(const string& filePath)
{
	myMap<int, AnimInfo> EmptyList;
	NEW_SQLITE(SQLiteImagePositionAnimation, animationPosTable, "ImagePositionAnimation");
	myVector<string> folders;
	findFolders(filePath, folders, true);
	FOR_VECTOR(folders)
	{
		myVector<string> files;
		findFiles(folders[i], files, ".png", false);
		if (files.size() == 0)
		{
			continue;
		}

		cout << "处理目录:" << folders[i] << endl;
		// 先将所有图片按照序列帧放入列表中
		myMap<string, myMap<int, AnimInfo>> animList;
		FOR_VECTOR_J(files)
		{
			string fileName = files[j];
			string path = removeStartString(getFilePath(fileName), "../media/");
			string atlas = path + "/" + getFileName(path);
			string fileNameNoSuffix = getFileNameNoSuffix(fileName, true);
			string animationWithDir = fileNameNoSuffix.substr(0, findStringPos(fileNameNoSuffix, "_", 0, false));
			int index = stringToInt(fileNameNoSuffix.substr(findStringPos(fileNameNoSuffix, "_", 0, false) + 1));
			TDImagePositionAnimation data;
			animationPosTable.query(atlas, animationWithDir, data);
			if (data.mID == 0)
			{
				cout << "找不到图片, atlas:" << atlas << ", animation:" << animationWithDir << endl;
				break;
			}
			animList.tryInsert(animationWithDir, EmptyList).insert(index, AnimInfo(files[j], atlas, index, Vector2Int(data.mPosX[index], -data.mPosY[index])));
		}

		// 再对序列帧列表进行处理
		FOREACH(iterAnim, animList)
		{
			Vector2Int maxSize;
			FOREACH(iter0, iterAnim->second)
			{
				Vector2Int newSize = generateImageSizeWithOffset(iter0->second.mFileName, iter0->second.mOffset);
				maxSize.x = getMax(newSize.x, maxSize.x);
				maxSize.y = getMax(newSize.y, maxSize.y);
			}
			FOREACH(iter1, iterAnim->second)
			{
				string fullPathNoMedia = removeStartString(iter1->second.mFileName, "../media/");
				string rootFolderName = getFirstFolderName(fullPathNoMedia);
				string newFullPath = "../media/" + rootFolderName + "_offseted" + "/" + removeFirstPath(fullPathNoMedia);
				createFolder(getFilePath(newFullPath));
				generateOffsetedImage(iter1->second.mFileName, newFullPath, maxSize, iter1->second.mOffset);
			}
		}
	}
	END(folders);
}

void ImageUtility::generateAllIconTo36(const string& filePath)
{
	myMap<int, AnimInfo> EmptyList;
	myVector<string> files;
	findFiles(filePath, files, ".png", false);
	FOR_VECTOR(files)
	{
		string fileName = files[i];
		string fullPathNoMedia = removeStartString(fileName, "../media/");
		string rootFolderName = getFirstFolderName(fullPathNoMedia);
		string newFullPath = "../media/" + rootFolderName + "_offseted" + "/" + removeFirstPath(fullPathNoMedia);
		createFolder(getFilePath(newFullPath));
		generateExpandImage(fileName, newFullPath, Vector2Int(36, 36));
	}
	string fullPathNoMedia = removeStartString(filePath, "../media/");
	string rootFolderName = getFirstFolderName(fullPathNoMedia);
	string newFullPath = "../media/" + rootFolderName + "_offseted" + "/" + removeFirstPath(fullPathNoMedia);
	cout << "已输出至:" << newFullPath << endl;
}

void ImageUtility::trimAllImage(const string& filePath, const int minAlpha)
{
	myMap<int, string> EmptyList;
	myVector<string> folders;
	folders.push_back(filePath);
	findFolders(filePath, folders, true);
	FOR_VECTOR(folders)
	{
		myVector<string> files;
		findFiles(folders[i], files, ".png", false);
		if (files.size() == 0)
		{
			continue;
		}
		cout << "处理目录:" << folders[i] << endl;

		// 先将所有图片按照序列帧放入列表中
		myMap<string, myMap<int, string>> animList;
		FOR_VECTOR_J(files)
		{
			const string& fileName = files[j];
			string fileNameNoSuffix = getFileNameNoSuffix(fileName, true);
			int index0 = findStringPos(fileNameNoSuffix, "_", 0, false);
			if (index0 < 0)
			{
				cout << "图片名字不是序列帧" << endl;
				return;
			}
			string animationWithDir = fileNameNoSuffix.substr(0, index0);
			int frameIndex = stringToInt(fileNameNoSuffix.substr(index0 + 1));
			animList.tryInsert(animationWithDir, EmptyList).insert(frameIndex, fileName);
		}

		FOREACH(iterAnim, animList)
		{
			Vector4Int minRect(9999, 9999, 0, 0);
			FOREACH(iter0, iterAnim->second)
			{
				Vector4Int rect = generateMinRect(iter0->second, minAlpha);
				if (rect.x >= 0)
				{
					minRect.x = getMin(rect.x, minRect.x);
				}
				if (rect.y >= 0)
				{
					minRect.y = getMin(rect.y, minRect.y);
				}
				if (rect.z >= 0)
				{
					minRect.z = getMax(rect.z, minRect.z);
				}
				if (rect.w >= 0)
				{
					minRect.w = getMax(rect.w, minRect.w);
				}
			}
			Vector2Int minSize(minRect.z - minRect.x, minRect.w - minRect.y);
			Vector2Int center((minRect.z + minRect.x) >> 1, (minRect.w + minRect.y) >> 1);
			FOREACH(iter1, iterAnim->second)
			{
				string fullPathNoMedia = removeStartString(iter1->second, "../media/");
				string rootFolderName = getFirstFolderName(fullPathNoMedia);
				string newFullPath = "../media/" + rootFolderName + "_trim" + "/" + removeFirstPath(fullPathNoMedia);
				createFolder(getFilePath(newFullPath));
				trimImage(iter1->second, newFullPath, minSize, center);
			}
		}
		string fullPathNoMedia = removeStartString(folders[i], "../media/");
		string rootFolderName = getFirstFolderName(fullPathNoMedia);
		string newFullPath = "../media/" + rootFolderName + "_trim" + "/" + removeFirstPath(fullPathNoMedia);
		cout << "处理完毕,已输出到:" << newFullPath << endl;
	}
}

void ImageUtility::fullImageToMinimal(const string& path)
{
	myVector<string> files;
	findFiles(path, files, ".png", true);
	FOR_VECTOR(files)
	{
		string fullPathNoMedia = removeStartString(files[i], "../media/");
		string rootFolderName = getFirstFolderName(fullPathNoMedia);
		string newFullPath = "../media/" + rootFolderName + "_removeEmpty" + "/" + removeFirstPath(fullPathNoMedia);
		createFolder(getFilePath(newFullPath));
		Vector2Int offset;
		generateMinimalImage(files[i], newFullPath, offset);
		writeFile(replaceSuffix(newFullPath, ".txt"), vector2IntToString(offset));
	}
	END(files);
}

void ImageUtility::checkMapTile(const string& path)
{
	NEW_SQLITE(SQLiteTileImageObject, objTileTable, "TileImageObject");
	myMap<int, myMap<int, TDTileImageObject*>> tileImageDataList;
	// key是FileIndex,value每个图集的终止图片下标
	const auto& allList = objTileTable.queryAll();
	FOREACH_CONST(iter, allList)
	{
		TDTileImageObject* data = iter->second;
		auto& ptr = tileImageDataList.tryInsert(data->mFileIndex, myMap<int, TDTileImageObject*>());
		ptr.insert(data->mImageName, data);
	}

	myMap<int, mySet<int>> missingList;
	myVector<string> files;
	findFiles(path, files, ".bytes", true);
	FOR_VECTOR(files)
	{
		const string& file = files[i];
		if (endWith(file, "_unreach.bytes"))
		{
			continue;
		}
		cout << "开始检查" << file << endl;
		MapDataSimple data;
		data.readFile(file);
		FOR_J(data.mTileCount)
		{
			const MapTileSimple& tile = data.mTileList[j];
			if (tile.mObjImgIdx == 0)
			{
				continue;
			}
			auto* ptr = tileImageDataList.get(tile.mObjFileIdx);
			if (ptr == nullptr || !ptr->contains(tile.mObjImgIdx))
			{
				auto* missionPtr = missingList.get(tile.mObjFileIdx);
				// 不再打印重复的丢失信息
				if (missionPtr != nullptr && missionPtr->contains(tile.mObjImgIdx))
				{
					continue;
				}
				missingList.tryInsert(tile.mObjFileIdx, mySet<int>()).insert(tile.mObjImgIdx);
				Vector2Int tilePos = tileIndexToTilePos(j, data.mHeader->mHeight);
				cout << "找不到地砖信息:tile.mObjFileIdx:" << (int)tile.mObjFileIdx << ", tile.mObjImgIdx:" << tile.mObjImgIdx << ", 地图:" << file << ",地砖坐标:" << tilePos.x << "," << tilePos.y << endl;
			}
		}
	}
	END(files);
}

void ImageUtility::autoFillHumanAnimationTable(const string& clothName, int startID)
{
	string animationName[HUMAN_ACTION_COUNT]{"攻击", "释放技能", "跑步", "走路", "死亡", "被击", "站立", "挖矿", "搜寻", "跳跃攻击", "野蛮冲撞"};
	int frameCount[HUMAN_ACTION_COUNT]{6, 6, 6, 6, 4, 3, 4, 6, 2, 8, 6};
	bool loop[HUMAN_ACTION_COUNT]{false, false, true, true, false, false, true, true, true, false, true};
	float animationSpeed[HUMAN_ACTION_COUNT]{0.35f, 0.35f, 0.35f, 0.35f, 0.35f, 0.35f, 0.35f, 0.35f, 0.35f, 0.35f, 0.7f};
	int startIDOffset[HUMAN_ACTION_COUNT]{0 * DIRECTION_COUNT, 8 * DIRECTION_COUNT, 6 * DIRECTION_COUNT, 10 * DIRECTION_COUNT,
						  1 * DIRECTION_COUNT, 3 * DIRECTION_COUNT, 9 * DIRECTION_COUNT, 2 * DIRECTION_COUNT,
						  7 * DIRECTION_COUNT, 4 * DIRECTION_COUNT, 10 * DIRECTION_COUNT };
	NEW_SQLITE(SQLiteAnimation, animationTable, "Animation");
	int curAnimationID = animationTable.getMaxID();
	FOR_I(HUMAN_ACTION_COUNT)
	{
		TDAnimation data;
		data.mID = ++curAnimationID;
		data.mDescription = clothName + "的" + animationName[i];
		data.mFrameCount = frameCount[i];
		data.mLoop = loop[i] ? 1 : 0;
		data.mAnimationSpeed = animationSpeed[i];
		data.mDirectionCount = DIRECTION_COUNT;
		FOR_J(DIRECTION_COUNT)
		{
			data.mAnimationPosition.push_back(startID + startIDOffset[i] + j);
		}
		animationTable.insert(data);
	}
}

void ImageUtility::autoFillMonsterAnimationTable(const string& monsterName, int startID)
{
	string animationName[MONSTER_ACTION_COUNT]{ "攻击", "死亡", "被击", "站立", "走路" };
	int frameCount[MONSTER_ACTION_COUNT]{ 6, 10, 2, 4, 6 };
	bool loop[MONSTER_ACTION_COUNT]{ false, false, false, true, true };
	float animationSpeed[MONSTER_ACTION_COUNT]{ 0.2f, 0.3f, 0.08f, 0.08f, 0.16f };
	int startIDOffset[MONSTER_ACTION_COUNT]{ 0 * DIRECTION_COUNT, 1 * DIRECTION_COUNT, 2 * DIRECTION_COUNT, 3 * DIRECTION_COUNT, 4 * DIRECTION_COUNT };
	NEW_SQLITE(SQLiteAnimation, animationTable, "Animation");
	int curAnimationID = animationTable.getMaxID();
	FOR_I(MONSTER_ACTION_COUNT)
	{
		TDAnimation data;
		data.mID = ++curAnimationID;
		data.mDescription = monsterName + "的" + animationName[i];
		data.mFrameCount = frameCount[i];
		data.mLoop = loop[i] ? 1 : 0;
		data.mAnimationSpeed = animationSpeed[i];
		data.mDirectionCount = DIRECTION_COUNT;
		FOR_J(DIRECTION_COUNT)
		{
			data.mAnimationPosition.push_back(startID + startIDOffset[i] + j);
		}
		animationTable.insert(data);
	}
}

void ImageUtility::dumpMapFile(const string& fileName)
{
	if (endWith(fileName, ".bytes"))
	{
		MapDataSimple mapData;
		mapData.readFile(fileName);
		string dumpString;
		FOR_I(mapData.mTileCount)
		{
			const MapTileSimple& tile = mapData.mTileList[i];
			if (tile.mBngImgIdx == 0 && tile.mMidImgIdx == 0 && tile.mObjImgIdx == 0 && tile.mObjFileIdx == 0)
			{
				continue;
			}
			dumpString += "index:" + intToString(i) + "\tBngImg:" + intToString(tile.mBngImgIdx) + "\tMidImg:" + intToString(tile.mMidImgIdx) + "\tObjImg:" + intToString(tile.mObjImgIdx) + "\tObjFile:" + intToString(tile.mObjFileIdx) + "\n";
		}
		writeFile(fileName + ".txt", dumpString);
	}
	else if (endWith(fileName, ".map"))
	{
		MapData mapData;
		mapData.readFile(fileName);
		string dumpString;
		FOR_I(mapData.mTileCount)
		{
			const MapTile& tile = mapData.mTileList[i];
			if (tile.mBngImgIdx == 0 && tile.mMidImgIdx == 0 && tile.mObjImgIdx == 0 && tile.mObjFileIdx == 0)
			{
				continue;
			}
			dumpString += "index:" + intToString(i) + "\tBngImg:" + intToString(tile.mBngImgIdx) + "\tMidImg:" + intToString(tile.mMidImgIdx) + "\tObjImg:" + intToString(tile.mObjImgIdx) + "\tObjFile:" + intToString(tile.mObjFileIdx) + "\n";
		}
		writeFile(fileName + ".txt", dumpString);
	}
}

void ImageUtility::makeAtlasGroup(const string& filePath)
{
	myVector<string> files;
	findFiles(filePath, files, ".png", false);
	// 根据数字进行排序
	sortByFileNumber(files);
	int groupIndex = 0;
	int totalIndex = 0;
	while (totalIndex < (int)files.size())
	{
		int curCount = 0;
		while (true)
		{
			// 每次最多增加300张图片
			const int batchCount = getMin((int)files.size() - curCount - totalIndex, 300);
			// 将文件拷贝到临时目录
			const string tempPath = filePath + "/temp/";
			FOR_I(batchCount)
			{
				const string& curFile = files[totalIndex + curCount + i];
				moveFile(curFile, tempPath + getFileName(curFile));
			}
			// 移动后就尝试打包
			const string outputPath = getFilePath(tempPath) + "/tempAtlas";
			const string tempFileName = "temp";
			packAtlas(outputPath, tempFileName, tempPath);
			bool packResult = isFileExist(outputPath + "/" + tempFileName + ".png");
			deleteFolder(outputPath);
			// 本次打包失败,则说明上一次的数量已经达到上限,临时目录就可以直接改成一个可单独打图集的目录
			// 打包成功,但是已经没有剩余的图片了,则当前图集已经完成,可以跳转分组下一个图集了
			if (curCount + totalIndex + batchCount >= (int)files.size() || !packResult)
			{
				// 打包失败,将多余的这部分图片再移回到原来的位置去
				if (!packResult)
				{
					FOR_I(batchCount)
					{
						const string& curFile = files[totalIndex + curCount + i];
						moveFile(tempPath + getFileName(curFile), curFile);
					}
					// 第一次尝试就已经失败,就不再继续处理了
					if (curCount == 0)
					{
						cout << "分组失败,原因可能是图片过大导致无法打出最小数量图片的图集,将跳过,请手动处理" << endl;
						// 直接跳出外层循环,不再处理
						totalIndex = (int)files.size();
						break;
					}
				}
				else
				{
					curCount += batchCount;
				}

				string newPath = filePath + "/" + getFolderName(filePath + "/") + "_" + intToString(groupIndex++);
				renameFolder(tempPath, newPath);
				cout << "已分组目录:" << newPath << ",图片数量:" << curCount << endl;
				totalIndex += curCount;
				break;
			}
			curCount += batchCount;
		}
	}
}

void ImageUtility::makeAtlasGroupAll()
{
	myVector<string> folders;
	findFolders("../media", folders);
	int index = 0;
	for (const string& folder : folders)
	{
		myVector<string> files;
		findFiles(folder, files, ".png", false);
		if (files.size() > 0)
		{
			makeAtlasGroup(folder);
		}
		++index;
		cout << "已处理:" << index << "/" << folders.size() << endl;
	}
}

void ImageUtility::generateAtlasInfoFile(const string& filePath)
{
	txSerializer writer;
	myVector<string> folders;
	findFolders(filePath, folders);
	writer.write((int)folders.size());
	for (const string& folder : folders)
	{
		myVector<string> files;
		findFiles(folder, files, ".png", false);
		// 根据数字进行排序
		sortByFileNumber(files);
		writer.writeString((folder.substr(filePath.length() + 1) + ".png").c_str());
		writer.write((int)files.size());
		for (const string& file : files)
		{
			string fileNameNoSuffix = getFileNameNoSuffix(file, true);
			int fileIndex = stringToInt(fileNameNoSuffix);
			if (fileIndex == 0 && fileNameNoSuffix != "0")
			{
				cout << "图片名不是一个整数:" << file << endl;
				system("pause");
				continue;
			}
			Vector2Int imageSize = getImageSize(file);
			writer.write(fileIndex);
			writer.write((ushort)imageSize.x);
			writer.write((ushort)imageSize.y);
		}
	}
	writer.writeToFile(filePath + "_AtlasInfo.bytes");
}

void ImageUtility::printMapSize(const string& filePath)
{
	myVector<string> files;
	findFiles(filePath, files, ".map");
	for (const string& file : files)
	{
		MapData mapData;
		mapData.readFile(file);
		cout << getFileName(file) << ":宽:" << mapData.mHeader->mWidth << ",高:" << mapData.mHeader->mHeight << endl;
	}
}

void ImageUtility::mapFileToTxt(const string& filePath)
{
	MapData mapData;
	if (endWith(filePath, ".map"))
	{
		mapData.readFile(filePath);
	}
	else
	{
		mapData.readFile(filePath + ".map");
	}
	string str = mapData.mHeader->toString();
	str += "\n";
	FOR_I(mapData.mTileCount)
	{
		Vector2Int tilePos = tileIndexToTilePos(i, mapData.mHeader->mHeight);
		str += intToString(tilePos.x) + "," + intToString(tilePos.y) + ":";
		str += mapData.mTileList[i].toString();
		str += "\n";
	}
	writeFile(filePath + "_info.txt", str);
}

void ImageUtility::txtToMapFile(const string& filePath)
{
	myVector<string> lines;
	if (endWith(filePath, ".txt"))
	{
		openTxtFileLines(filePath, lines, false);
	}
	else
	{
		openTxtFileLines(filePath + ".txt", lines, false);
	}
	if (lines.size() == 0)
	{
		cout << "找不到文件:" << filePath << endl;
		return;
	}
	MapData mapData;
	mapData.mHeader->parseString(lines[0]);
	mapData.createTilesByHeader();
	FOR_I(mapData.mTileCount)
	{
		const string& line = lines[i + 1];
		myVector<string> vec;
		split(line.c_str(), ":", vec);
		if (vec.size() != 2)
		{
			cout << "错误:文本格式的地图地砖行错误,下标:" << i << endl;
			continue;
		}
		mapData.mTileList[i].parseString(vec[1]);
	}
	mapData.writeFile(filePath.substr(0, filePath.length() - strlen("_info.txt")));
}

void ImageUtility::packAtlas(const string& outputPath, const string& outputFileName, const string& sourcePath)
{
	createFolder(outputPath);
	string cmdLine;
	cmdLine += "--data " + outputPath + "/" + outputFileName + ".tpsheet ";
	cmdLine += "--sheet " + outputPath + "/" + outputFileName + ".png ";
	cmdLine += "--format unity-texture2d ";
	cmdLine += "--alpha-handling KeepTransparentPixels ";
	cmdLine += "--maxrects-heuristics Best ";
	cmdLine += "--disable-rotation ";
	cmdLine += "--size-constraints POT ";
	cmdLine += "--max-size 2048 ";
	cmdLine += "--trim-mode None ";
	cmdLine += "--extrude 0 ";
	cmdLine += "--shape-padding 1 ";
	cmdLine += "--border-padding 1 ";
	cmdLine += sourcePath;

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

void ImageUtility::trimImage(const string& filePath, const string& newFilePath, const Vector2Int size, const Vector2Int center)
{
	FreeImage_Initialise();
	FREE_IMAGE_FORMAT format = FreeImage_GetFileType(filePath.c_str());
	FIBITMAP* oldBitmap = FreeImage_Load(format, filePath.c_str());
	int width = FreeImage_GetWidth(oldBitmap);
	int height = FreeImage_GetHeight(oldBitmap);
	int xInOld = ((width - size.x) >> 1) + (center.x - (width >> 1));
	int yInOld = ((height - size.y) >> 1) + (center.y - (height >> 1));
	if (xInOld >= 0 && yInOld >= 0)
	{
		FIBITMAP* newBitmap = FreeImage_Allocate(size.x, size.y, 32);
		FOR_Y(size.y)
		{
			BYTE* newLine = FreeImage_GetScanLine(newBitmap, y);
			// 原始图片的底部在新图片中的y坐标,图片中的坐标是底部为0,向上为正
			BYTE* oldLine = FreeImage_GetScanLine(oldBitmap, y + yInOld);
			memcpy(newLine, oldLine + 4 * xInOld, size.x * 4);
		}
		FreeImage_Save(format, newBitmap, newFilePath.c_str());
		FreeImage_Unload(newBitmap);
	}
	FreeImage_Unload(oldBitmap);
	FreeImage_DeInitialise();
}

Vector2Int ImageUtility::getImageSize(const string& fileName)
{
	FreeImage_Initialise();
	FIBITMAP* oldBitmap = FreeImage_Load(FreeImage_GetFileType(fileName.c_str()), fileName.c_str());
	int width = FreeImage_GetWidth(oldBitmap);
	int height = FreeImage_GetHeight(oldBitmap);
	FreeImage_Unload(oldBitmap);
	FreeImage_DeInitialise();
	return Vector2Int(width, height);
}

Vector2Int ImageUtility::generateImageSizeWithOffset(const string& fileName, Vector2Int offset)
{
	FreeImage_Initialise();
	FREE_IMAGE_FORMAT format = FreeImage_GetFileType(fileName.c_str());
	FIBITMAP* oldBitmap = FreeImage_Load(format, fileName.c_str());
	int width = FreeImage_GetWidth(oldBitmap);
	int height = FreeImage_GetHeight(oldBitmap);
	int minX = offset.x;
	int maxX = width + offset.x;
	int minY = offset.y - height;
	int maxY = offset.y;
	FreeImage_Unload(oldBitmap);
	FreeImage_DeInitialise();
	return Vector2Int(getMax(abs(minX), abs(maxX)) << 1, getMax(abs(minY), abs(maxY)) << 1);
}

// 计算图片的最小尺寸,并且返回出该图片的偏移量,以确保渲染位置不变
void ImageUtility::generateMinimalImage(const string& fileName, const string& newFileName, Vector2Int& offset)
{
	FreeImage_Initialise();
	FREE_IMAGE_FORMAT format = FreeImage_GetFileType(fileName.c_str());
	FIBITMAP* oldBitmap = FreeImage_Load(format, fileName.c_str());

	int oldWidth = FreeImage_GetWidth(oldBitmap);
	int oldHeight = FreeImage_GetHeight(oldBitmap);
	// 从上往下找top
	int top = 9999;
	FOR_X(oldWidth)
	{
		int columnTop = 9999;
		FOR_Y(oldHeight)
		{
			RGBQUAD color = getColor(oldBitmap, x, y);
			if (color.rgbReserved > 5)
			{
				columnTop = (int)y;
				break;
			}
		}
		top = getMin(top, columnTop);
	}
	int bottom = 0;
	FOR_X(oldWidth)
	{
		int columnBottom = 0;
		FOR_Y(oldHeight)
		{
			RGBQUAD color = getColor(oldBitmap, x, oldHeight - y - 1);
			if (color.rgbReserved > 5)
			{
				columnBottom = (int)(oldHeight - y - 1);
				break;
			}
		}
		bottom = getMax(bottom, columnBottom);
	}
	int left = 9999;
	FOR_Y(oldHeight)
	{
		int lineLeft = 9999;
		FOR_X(oldWidth)
		{
			RGBQUAD color = getColor(oldBitmap, x, y);
			if (color.rgbReserved > 5)
			{
				lineLeft = (int)x;
				break;
			}
		}
		left = getMin(left, lineLeft);
	}
	int right = 0;
	FOR_Y(oldHeight)
	{
		int lineRight = 0;
		FOR_X(oldWidth)
		{
			RGBQUAD color = getColor(oldBitmap, oldWidth - x - 1, y);
			if (color.rgbReserved > 5)
			{
				lineRight = (int)(oldWidth - x - 1);
				break;
			}
		}
		right = getMax(right, lineRight);
	}

	int newWidth = right - left + 1;
	int newHeight = bottom - top + 1;
	if (newWidth < 0 || newHeight < 0)
	{
		newWidth = oldWidth;
		newHeight = oldHeight;
		left = 0;
		top = 0;
	}
	FIBITMAP* newBitmap = FreeImage_Allocate(newWidth, newHeight, 32);
	FOR_Y(newHeight)
	{
		FOR_X(newWidth)
		{
			setColor(newBitmap, x, y, getColor(oldBitmap, x + left, y + top));
		}
	}

	offset.x = left - (oldWidth >> 1);
	offset.y = top - ((oldHeight >> 1) - newHeight);
	FreeImage_Save(format, newBitmap, newFileName.c_str());
	FreeImage_Unload(newBitmap);
	FreeImage_Unload(oldBitmap);
	FreeImage_DeInitialise();
}

void ImageUtility::generateExpandImage(const string& fileName, const string& newFileName, Vector2Int size)
{
	FreeImage_Initialise();
	FREE_IMAGE_FORMAT format = FreeImage_GetFileType(fileName.c_str());
	FIBITMAP* oldBitmap = FreeImage_Load(format, fileName.c_str());
	int oldWidth = FreeImage_GetWidth(oldBitmap);
	int oldHeight = FreeImage_GetHeight(oldBitmap);
	int yInOld = (size.y - oldHeight) >> 1;
	int copyPixels = getMin(oldWidth, size.x);
	int srcPixelOffset = 0;
	int xInOld = (size.x - oldWidth) >> 1;
	if (size.x < oldWidth)
	{
		srcPixelOffset = (oldWidth - size.x) >> 1;
		xInOld = 0;
	}
	FIBITMAP* newBitmap = FreeImage_Allocate(size.x, size.y, 32);
	int bpp = FreeImage_GetBPP(oldBitmap);
	FOR_Y(size.y)
	{
		BYTE* newLine = FreeImage_GetScanLine(newBitmap, y);
		if (bpp == 32)
		{
			memset(newLine, 0, size.x * 4);
		}
		else if (bpp == 24)
		{
			FOR_I(size.x)
			{
				newLine[i * 4 + 0] = 0;
				newLine[i * 4 + 1] = 0;
				newLine[i * 4 + 2] = 0;
				newLine[i * 4 + 3] = 255;
			}
		}
		// 原始图片的底部在新图片中的y坐标,图片中的坐标是底部为0,向上为正
		int indexY = y - yInOld;
		if (indexY >= 0 && indexY < oldHeight)
		{
			BYTE* oldLine = FreeImage_GetScanLine(oldBitmap, indexY);
			if (bpp == 32)
			{
				memcpy(newLine + 4 * xInOld, oldLine + srcPixelOffset * 4, copyPixels * 4);
			}
			else if (bpp == 24)
			{
				FOR_I(copyPixels)
				{
					memcpy(newLine + 4 * xInOld + 4 * i, oldLine + 3 * i + 3 * srcPixelOffset, 3);
				}
			}
		}
	}

	FreeImage_Save(format, newBitmap, newFileName.c_str());
	FreeImage_Unload(oldBitmap);
	FreeImage_Unload(newBitmap);
	FreeImage_DeInitialise();
}

void ImageUtility::generateOffsetedImage(const string& fileName, const string& newFileName, Vector2Int maxSize, Vector2Int offset)
{
	FreeImage_Initialise();
	FREE_IMAGE_FORMAT format = FreeImage_GetFileType(fileName.c_str());
	FIBITMAP* oldBitmap = FreeImage_Load(format, fileName.c_str());
	int oldWidth = FreeImage_GetWidth(oldBitmap);
	int oldHeight = FreeImage_GetHeight(oldBitmap);
	int xInOld = (int)(offset.x + maxSize.x / 2);
	int yInOld = (int)(offset.y - oldHeight + maxSize.y / 2);
	FIBITMAP* newBitmap = FreeImage_Allocate(maxSize.x, maxSize.y, 32);
	FOR_Y(maxSize.y)
	{
		BYTE* newLine = FreeImage_GetScanLine(newBitmap, y);
		memset(newLine, 0, maxSize.x * 4);
		// 原始图片的底部在新图片中的y坐标,图片中的坐标是底部为0,向上为正
		int indexY = y - yInOld;
		if(indexY >= 0 && indexY < oldHeight)
		{
			BYTE* oldLine = FreeImage_GetScanLine(oldBitmap, indexY);
			memcpy(newLine + 4 * xInOld, oldLine, oldWidth * 4);
		}
	}

	FreeImage_Save(format, newBitmap, newFileName.c_str());
	FreeImage_Unload(oldBitmap);
	FreeImage_Unload(newBitmap);
	FreeImage_DeInitialise();
}

Vector4Int ImageUtility::generateMinRect(const string& fileName, const int minAlpha)
{
	FreeImage_Initialise();
	FREE_IMAGE_FORMAT format = FreeImage_GetFileType(fileName.c_str());
	FIBITMAP* bitmap = FreeImage_Load(format, fileName.c_str());
	uint width = FreeImage_GetWidth(bitmap);
	uint height = FreeImage_GetHeight(bitmap);
	int minX = -1;
	int maxX = -1;
	int minY = -1;
	int maxY = -1;
	FOR_Y(height)
	{
		BYTE* newLine = FreeImage_GetScanLine(bitmap, y);
		int minXInLine = -1;
		int maxXInLine = -1;
		FOR_X(width)
		{
			RGBQUAD color = getColor(bitmap, x, y);
			if (color.rgbReserved > minAlpha)
			{
				if (minXInLine < 0)
				{
					minXInLine = x;
				}
				else
				{
					minXInLine = getMin(minXInLine, (int)x);
				}
				if (maxXInLine < 0)
				{
					maxXInLine = x;
				}
				else
				{
					maxXInLine = getMax(maxXInLine, (int)x);
				}
			}
		}
		// 存在有效值,表示这一行不是空白的
		if (minXInLine >= 0 || maxXInLine >= 0)
		{
			if (minY < 0)
			{
				minY = y;
			}
			else
			{
				minY = getMin(minY, (int)y);
			}
			if (maxY < 0)
			{
				maxY = y;
			}
			else
			{
				maxY = getMax(maxY, (int)y);
			}
			if (minX < 0)
			{
				minX = minXInLine;
			}
			else
			{
				minX = getMin(minX, minXInLine);
			}
			if (maxX < 0)
			{
				maxX = maxXInLine;
			}
			else
			{
				maxX = getMax(maxX, maxXInLine);
			}
		}
	}
	FreeImage_Unload(bitmap);
	FreeImage_DeInitialise();
	return Vector4Int(minX, minY, maxX, maxY);
}

void ImageUtility::removeAllBackground(const string& filePath)
{
	myVector<string> files;
	findFiles(filePath, files, ".png");
	FOR_VECTOR(files)
	{
		string fullPathNoMedia = removeStartString(files[i], "../media/");
		string rootFolderName = getFirstFolderName(fullPathNoMedia);
		string newFullPath = "../media/" + rootFolderName + "_transparent" + "/" + removeFirstPath(fullPathNoMedia);
		createFolder(getFilePath(newFullPath));
		removeBackground(files[i], newFullPath);
	}
	END(files);
}

void ImageUtility::removeBackground(const string& fileName, const string& newFileName)
{
	FreeImage_Initialise();
	FREE_IMAGE_FORMAT format = FreeImage_GetFileType(fileName.c_str());
	FIBITMAP* oldBitmap = FreeImage_Load(format, fileName.c_str());
	uint width = FreeImage_GetWidth(oldBitmap);
	uint height = FreeImage_GetHeight(oldBitmap);

	FIBITMAP* newBitmap = FreeImage_Allocate(width, height, 32);
	FOR_Y(height)
	{
		FOR_X(width)
		{
			RGBQUAD color = getColor(oldBitmap, x, y);
			RGBQUAD newColor{};
			newColor.rgbRed = color.rgbRed;
			newColor.rgbGreen = color.rgbGreen;
			newColor.rgbBlue = color.rgbBlue;
			newColor.rgbReserved = getMax(color.rgbRed, color.rgbGreen, color.rgbBlue);
			setColor(newBitmap, x, y, newColor);
		}
	}

	FreeImage_Save(format, newBitmap, newFileName.c_str());
	FreeImage_Unload(oldBitmap);
	FreeImage_Unload(newBitmap);
	FreeImage_DeInitialise();
}

void ImageUtility::convertMapFile(const string& filePath)
{
	myVector<string> files;
	findFiles(filePath, files, ".map");
	FOR_VECTOR(files)
	{
		MapData data;
		data.readFile(files[i]);
		data.convertToSimple(replaceSuffix(files[i], ".bytes"));
	}
	END(files);
	deleteEmptyFolder(filePath);
}

void ImageUtility::writeTileObjectImageSizeSQLite(const string& filePath, int mapFileID)
{
	// 物体层图片
	NEW_SQLITE(SQLiteTileImageObject, objectTileImage, "TileImageObject");
	int maxIDObj = objectTileImage.getMaxID();
	myVector<string> fileList;
	findFiles(filePath, fileList, ".png");
	objectTileImage.executeNonQuery("BEGIN TRANSACTION");
	const int fileCount = fileList.size();
	cout << "总共" << fileCount << "张图片" << endl;
	FOR_I(fileCount)
	{
		const string& filePath = fileList[i];
		string folderName = getFolderName(filePath);
		if (!startWith(folderName, "Objects_"))
		{
			continue;
		}
		myVector<string> elements;
		split(folderName.c_str(), "_", elements);
		TDTileImageObject imageData;
		imageData.mID = maxIDObj + i + 1;
		Vector2Int imageSize = getImageSize(filePath);
		imageData.mImageSizeX = imageSize.x;
		imageData.mImageSizeY = imageSize.y;
		imageData.mAtlasName = folderName + ".png";
		imageData.mImageName = stringToInt(getFileNameNoSuffix(filePath, true));
		imageData.mMapFileID = mapFileID;
		if (elements.size() == 3)
		{
			imageData.mFileIndex = stringToInt(elements[1]);
		}
		else
		{
			imageData.mFileIndex = 0;
		}
		objectTileImage.insert(imageData);
		
		if ((i + 1) % 1000 == 0)
		{
			cout << "已写入" << i + 1 << "个数据" << endl;
		}
	}
	objectTileImage.executeNonQuery("COMMIT");

	// 大地砖图片
	NEW_SQLITE(SQLiteTileImageBig, bigTileImage, "TileImageBig");
	int maxIDBig = bigTileImage.getMaxID();
	bigTileImage.executeNonQuery("BEGIN TRANSACTION");
	FOR_I(fileCount)
	{
		const string& filePath = fileList[i];
		string folderName = getFolderName(filePath);
		if (!startWith(folderName, "Tiles_"))
		{
			continue;
		}
		Vector2Int imageSize = getImageSize(filePath);
		TDTileImageBig imageData;
		imageData.mID = maxIDBig + i + 1;
		imageData.mAtlasName = folderName + ".png";
		imageData.mImageName = stringToInt(getFileNameNoSuffix(filePath, true));
		imageData.mMapFileID = mapFileID;
		bigTileImage.insert(imageData);
		if ((i + 1) % 1000 == 0)
		{
			cout << "已写入" << i + 1 << "个数据" << endl;
		}
	}
	bigTileImage.executeNonQuery("COMMIT");

	// 中间层的小地砖的图片
	NEW_SQLITE(SQLiteTileImageMid, midTileImage, "TileImageMid");
	int maxIDMid = midTileImage.getMaxID();
	midTileImage.executeNonQuery("BEGIN TRANSACTION");
	FOR_I(fileCount)
	{
		const string& filePath = fileList[i];
		string folderName = getFolderName(filePath);
		if (!startWith(folderName, "SmTiles_"))
		{
			continue;
		}
		Vector2Int imageSize = getImageSize(filePath);
		TDTileImageMid imageData;
		imageData.mID = maxIDMid + i + 1;
		imageData.mAtlasName = folderName + ".png";
		imageData.mImageName = stringToInt(getFileNameNoSuffix(filePath, true));
		imageData.mMapFileID = mapFileID;
		midTileImage.insert(imageData);
		if ((i + 1) % 1000 == 0)
		{
			cout << "已写入" << i + 1 << "个数据" << endl;
		}
	}
	midTileImage.executeNonQuery("COMMIT");
}

void ImageUtility::scaleTexture(const string& path, float scale)
{
	myVector<string> fileList;
	findFiles(path, fileList, ".png");
	FOR_VECTOR(fileList)
	{
		const string& fileName = fileList[i];
		FreeImage_Initialise();
		FREE_IMAGE_FORMAT format = FreeImage_GetFileType(fileName.c_str());
		FIBITMAP* oldBitmap = FreeImage_Load(format, fileName.c_str());
		uint width = FreeImage_GetWidth(oldBitmap);
		uint height = FreeImage_GetHeight(oldBitmap);
		uint newWidth = (uint)(width * scale);
		uint newHeight = (uint)(height * scale);
		FIBITMAP* newBitmap = FreeImage_Allocate(newWidth, newHeight, 32);
		FOR_Y(newHeight)
		{
			FOR_X(newWidth)
			{
				int mappingOldX = (int)(((float)x / newWidth) * width);
				int mappingOldY = (int)(((float)y / newHeight) * height);
				RGBQUAD color = getColor(oldBitmap, mappingOldX, mappingOldY);
				setColor(newBitmap, x, y, color);
			}
		}

		FreeImage_Save(format, newBitmap, fileName.c_str());
		FreeImage_Unload(oldBitmap);
		FreeImage_Unload(newBitmap);
		FreeImage_DeInitialise();
		scalePosition(getFileNameNoSuffix(fileName, false) + ".txt", scale);
	}
	END(fileList);
}

void ImageUtility::scalePosition(const string& fileName, float scale)
{
	string posString = openTxtFile(fileName, false);
	replaceAll(posString, "\r", "");
	myVector<int> pos;
	stringToInts(posString, pos);
	if (pos.size() != 2)
	{
		pos.clear();
		stringToInts(posString, pos, "\n");
	}
	if (pos.size() == 2)
	{
		writeFile(fileName, intToString((int)(pos[0] * scale)) + "," + intToString((int)(pos[1] * scale)));
	}
}
#ifndef _IMAGE_UTILITY_H_
#define _IMAGE_UTILITY_H_

#include "ImageDefine.h"
#include "FreeImage.h"
#include "WeaponAction.h"
#include "HumanAction.h"
#include "MonsterAction.h"
#include "EffectFrame.h"
#include "NPCAction.h"

class SQLite;
class ImageUtility
{
public:
	static void encodePNG(const string& path, unsigned char* color, int width, int height, FREE_IMAGE_FORMAT format);
	static bool readWixFile(const string& filePath, WIXFileImageInfo& info);
	static bool readWilHeader(const string& filePath, WILFileHeader& header);
	static void wixWilToPNG(const string& wixFileName, const string& wilFileName, const string& outputPath);
	static void allWixWilToPNG(const string& sourcePath);
	static void writePositionFile(const string& positionFile, POINT* posList, int posCount);
	static POINT* readPositionFile(const string& positionFile, int& posCount);
	static void renameImage(const string& path);
	static void renameImageToAnim(const string& path);
	static void splitPositionFile(const string& path, bool pathWithFileName = false);
	static void renameByDirection(const string& path, int directionCount, bool autoGroup = true);
	static void sortByFileNumber(txVector<string>& fileList, bool fileNameIsNumber = true);
	static void autoMoveFile(const string& path, int groupSize);
	static void autoGroupHumanImage(const string& path);
	static void autoGroupWeaponImage(const string& path);
	static void autoGroupMonsterImage0(const string& path);
	static void autoGroupMonsterImage1(const string& path);
	static void autoGroupEffectImage(const string& path);
	static void autoGroupNPCImage(const string& path);
	static bool getHumanActionInfo(int index, string& actionName, int& dir, int& frameIndex);
	static bool getNPCActionInfo(int index, string& actionName, int& dir, int& frameIndex);
	static void moveImageWithPosition(const string& fullFileName, const string& destFullFileName);
	static void renameImageWithPosition(const string& fullFileName, const string& destFullFileName);
	static void deleteImageWithPosition(const string& fullFileName);
	static void moveFileWithMeta(const string& fullFileName, const string& destFullFileName);
	static void renameFileWithMeta(const string& fullFileName, const string& destFullFileName);
	static void deleteFileWithMeta(const string& fullFileName);
	static void deleteInvalidImage(const string& path);
	static bool isInvalidImage(const string& fileName);
	static POINT getImagePosition(const string& imagePath);
	static void collectMapTexture(const string& fileName);
	static void groupAtlas(const string& filePath, int countInAltas);
	static void texturePacker(const string& texturePath);
	static void texturePackerAll(const string& texturePath);
	static void convertMapFile(const string& fileName);
	static void convertAllMapFile(const string& filePath);
	static void readAtlasIndexFile(const string& fileName, txMap<int, int>& indexMap);
	static void writeAnimFrameSQLite(bool updateOnly);
	static void writeImagePosSQLite(const string& path);
};

#endif
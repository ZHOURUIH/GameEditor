#ifndef _IMAGE_UTILITY_H_
#define _IMAGE_UTILITY_H_

#include "ImageDefine.h"
#include "FreeImage.h"
#include "WeaponAction.h"
#include "HumanAction.h"
#include "MonsterAction.h"
#include "EffectFrame.h"

class SQLite;
class ImageUtility
{
public:
	static void encodePNG(const std::string& path, unsigned char* color, int width, int height, FREE_IMAGE_FORMAT format);
	static bool readWixFile(const std::string& filePath, WIXFileImageInfo& info);
	static bool readWilHeader(const std::string& filePath, WILFileHeader& header);
	static void wixWilToPNG(const std::string& wixFileName, const std::string& wilFileName, const std::string& outputPath);
	static void allWixWilToPNG(const std::string& sourcePath);
	static void writePositionFile(const std::string& positionFile, POINT* posList, int posCount);
	static POINT* readPositionFile(const std::string& positionFile, int& posCount);
	static void saveFrameInfo(const std::string& path, IMAGE_TYPE imageType, SQLite* sqlite);
	static void renameImage(const std::string& path);
	static void renameImageToAnim(const std::string& path);
	static void splitPositionFile(const std::string& path);
	static void renameByDirection(const std::string& path);
	static void sortByFileNumber(txVector<std::string>& fileList);
	static void autoMoveFile(const std::string& path, int groupSize);
	static void writeSQLite(txMap<std::string, WeaponActionSet>& actionSetList, SQLite* sqlite);
	static void writeSQLite(txMap<std::string, HumanActionSet>& actionSetList, SQLite* sqlite);
	static void writeSQLite(txMap<std::string, MonsterActionSet>& actionSetList, SQLite* sqlite);
	static void writeSQLite(txMap<std::string, EffectSet>& actionSetList, SQLite* sqlite);
	static void autoGroupHumanImage(const std::string& path);
	static void autoGroupWeaponImage(const std::string& path);
	static void autoGroupMonsterImage0(const std::string& path);
	static void autoGroupMonsterImage1(const std::string& path);
	static void autoGroupEffectImage(const std::string& path);
	static bool getHumanActionInfo(int index, std::string& actionName, int& dir, int& frameIndex);
	static void moveImageWithPosition(const std::string& fullFileName, const std::string& destFullFileName);
	static void renameImageWithPosition(const std::string& fullFileName, const std::string& destFullFileName);
	static void deleteImageWithPosition(const std::string& fullFileName);
	static void moveFileWithMeta(const std::string& fullFileName, const std::string& destFullFileName);
	static void renameFileWithMeta(const std::string& fullFileName, const std::string& destFullFileName);
	static void deleteFileWithMeta(const std::string& fullFileName);
	static void deleteInvalidImage(const std::string& path);
	static bool isInvalidImage(const std::string& fileName);
	static POINT getImagePosition(const std::string& imagePath);
	static void collectMapTexture(const std::string& fileName);
	static void groupAtlas(const std::string& filePath, int countInAltas);
	static void texturePacker(const std::string& texturePath);
	static void texturePackerAll(const std::string& texturePath);
	static void convertMapFile(const std::string& fileName);
	static void convertAllMapFile(const std::string& filePath);
	static void readAtlasIndexFile(const std::string& fileName, txMap<int, int>& indexMap);
};

#endif
#ifndef _IMAGE_UTILITY_H_
#define _IMAGE_UTILITY_H_

#include "ImageDefine.h"
#include "FreeImage.h"
#include "HumanAction.h"
#include "MonsterAction.h"
#include "EffectFrame.h"
#include "NPCAction.h"
#include "FrameUtility.h"

#define NEW_SQLITE(classType, tableVar, tableName)\
classType tableVar;	\
tableVar.setTableName(tableName);\
tableVar.init(string("../media/DataBase/") + tableName + ".db");\

class SQLite;
class ImageUtility : public FrameUtility
{
public:
	static void encodePNG(const string& path, unsigned char* color, int width, int height, FREE_IMAGE_FORMAT format);
	static bool readWixFile(const string& filePath, WIXFileImageInfo& info);
	static bool readWilHeader(const string& filePath, WILFileHeader& header);
	static void wixWilToPNG(const string& wixFileName, const string& wilFileName, const string& outputPath);
	static void allWixWilToPNG(const string& sourcePath);
	static void writePositionFile(const string& positionFile, POINT* posList, int posCount);
	static POINT* readPositionFile(const string& positionFile, int& posCount);
	static void renameImageToNumber(const string& path);
	static void renameImageToAnim(const string& path);
	static void splitPositionFile(const string& path, bool pathWithFileName = false);
	static void renameByDirection(const string& path, int directionCount, bool autoGroup = true);
	static void sortByFileNumber(myVector<string>& fileList, bool fileNameIsNumber = true);
	static void autoMoveFile(const string& path, int groupSize);
	static void autoGroupHumanImage(const string& path);
	static void autoGroupMonsterImage(const string& path);
	static void autoGroupEffectImage(const string& path);
	static void autoGroupNPCImage(const string& path);
	static void moveImageWithPosition(const string& fullFileName, const string& destFullFileName);
	static void renameImageWithPosition(const string& fullFileName, const string& destFullFileName);
	static void deleteImageWithPosition(const string& fullFileName);
	static void moveFileWithMeta(const string& fullFileName, const string& destFullFileName);
	static void renameFileWithMeta(const string& fullFileName, const string& destFullFileName);
	static void deleteFileWithMeta(const string& fullFileName);
	static void deleteInvalidImage(const string& path);
	static bool isInvalidImage(const string& fileName);
	static POINT getImagePosition(const string& imagePath);
	static void texturePackerAll(const string& texturePath);
	static void writeAnimFrameSQLite();
	static int tileIndexToTileX(int index, int mapHeight);
	static int tileIndexToTileY(int index, int mapHeight);
	static Vector2Int tileIndexToTilePos(int index, int mapHeight);
	static int tilePosToTileIndex(int x, int y, int mapHeight);
	static int pixelPosToTileX(Vector2 pixelPos, int mapHeight, int mapWidth);
	static int pixelPosToTileY(Vector2 pixelPos, int mapHeight);
	static int pixelPosToTileIndex(Vector2 pixelPos, int mapHeight, int mapWidth);
	static Vector2Int pixelPosToTilePos(Vector2 pixelPos, int mapHeight);
	static Vector2 tilePosToPixelPos(int x, int y, int mapHeight);
	static Vector2 tileIndexToPixelPos(int index, int mapHeight);
	static TILE_TRIANGLE pixelPosToTriangleIndex(Vector2 pos);
	static void getTrianglePoints(TILE_TRIANGLE pos, Vector2& point0, Vector2& point1, Vector2& point2);
	static void generateAllUnreachFile(const string& path);
	static void processAllShadow(const string& path);
	static void processShadowHorizontal(const string& filePath);
	static void processShadowVertical(const string& filePath);
	static void writeMapEffectToSQLite(int mapFileID, const string& mapFileName);
	static void generateAllOffsetedImage(const string& filePath);
	static void generateAllIconTo36(const string& filePath);
	static void generateGroupImage(const string& filePath);
	static void generateGroupMonsterImage(const string& filePath);
	static void trimAllImage(const string& filePath, const int minAlpha);
	static void removeAllBackground(const string& filePath);
	static void convertMapFile(const string& filePath);
	static void writeTileObjectImageSizeSQLite(const string& filePath, int mapFileID);
	static void scaleTexture(const string& path, float scale);
	static void scalePosition(const string& fileName, float scale);
	static void fullImageToMinimal(const string& path, int alphaThreashold);
	static void checkMapTile(const string& path);
	static void autoFillHumanAnimationTable(const string& clothName, int startID);
	static void autoFillMonsterAnimationTable(const string& monsterName, int startID);
	static void dumpMapFile(const string& fileName);
	static void makeAtlasGroup(const string& filePath);
	static void makeAtlasGroupAll();
	static void generateAtlasInfoFile(const string& filePath);
	static void printMapSize(const string& filePath);
	static void mapFileToTxt(const string& filePath);
	static void txtToMapFile(const string& filePath);
	static void groupAtlasByAnimationName(const string& filePath);
	static void packAtlas(const string& outputPath, const string& outputFileName, const string& sourcePath);
protected:
	static void generateMinimalImage(const string& fileName, const string& newFileName, Vector2Int& offset, int alphaThreashold);
	static void generateExpandImage(const string& fileName, const string& newFileName, Vector2Int size);
	static Vector2Int getImageSize(const string& fileName);
	static void removeBackground(const string& fileName, const string& newFileName);
	static void trimImage(const string& filePath, const string& newFilePath, Vector2Int size, Vector2 center);
	static Vector2Int generateImageSizeWithOffset(const string& fileName, Vector2Int offset);
	static void generateOffsetedImage(const string& fileName, const string& newFileName, Vector2Int maxSize, Vector2Int offset);
	static Vector4Int generateMinRect(const string& fileName, const int minAlpha);
	static void setPixel(BYTE* pixelData, int bpp, const RGBQUAD& rgb);
	static RGBQUAD getPixel(BYTE* pixelData, int bpp, RGBQUAD* palette);
	static bool isBlack(const RGBQUAD& rgb);
	static bool isEmpty(const RGBQUAD& rgb);
	static RGBQUAD getColor(FIBITMAP* bitmap, int x, int y);
	static void setColor(FIBITMAP* bitmap, int x, int y, const RGBQUAD& rgb);
	static bool getActionInfo(ActionInfo* actionInfo, int actionCount, int index, string& actionName, int& dir, int& frameIndex);
};

#endif
#include "SQLiteImagePositionAnimation.h"
#include "Utility.h"
#include "SQLiteDataReader.h"
#include "SQLite.h"

string ImagePositionAnimationData::COL_ID = "ID";
string ImagePositionAnimationData::COL_ATLAS = "Atlas";
string ImagePositionAnimationData::COL_ANIMATION = "Animation";
string ImagePositionAnimationData::COL_FRAME_COUNT = "FrameCount";
string ImagePositionAnimationData::COL_POSX = "PosX";
string ImagePositionAnimationData::COL_POSY = "PosY";

void SQLiteImagePositionAnimation::query(const string& atlas, const string& animation, txVector<ImagePositionAnimationData*>& dataList)
{
	string conditionString;
	StringUtility::appendConditionString(conditionString, ImagePositionAnimationData::COL_ATLAS, atlas, " and ");
	StringUtility::appendConditionString(conditionString, ImagePositionAnimationData::COL_ANIMATION, animation, "");
	doSelect(dataList, conditionString);
}
bool SQLiteImagePositionAnimation::updateData(const ImagePositionAnimationData& data)
{
	string updateString;
	StringUtility::appendUpdateInt(updateString, ImagePositionAnimationData::COL_FRAME_COUNT, data.mFrameCount);
	StringUtility::appendUpdateIntArray(updateString, ImagePositionAnimationData::COL_POSX, data.mPosX);
	StringUtility::appendUpdateIntArray(updateString, ImagePositionAnimationData::COL_POSY, data.mPosY);
	StringUtility::removeLastComma(updateString);
	string conditionString;
	StringUtility::appendConditionString(conditionString, ImagePositionAnimationData::COL_ATLAS, data.mAtlas, " and ");
	StringUtility::appendConditionString(conditionString, ImagePositionAnimationData::COL_ANIMATION, data.mAnimation, "");
	return doUpdate(updateString, conditionString);
}
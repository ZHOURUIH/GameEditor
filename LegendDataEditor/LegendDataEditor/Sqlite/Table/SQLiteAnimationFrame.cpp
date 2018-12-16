#include "SQLiteAnimationFrame.h"
#include "Utility.h"
#include "SQLiteDataReader.h"
#include "SQLite.h"

string AnimationFrameData::COL_ATLAS = "Atlas";
string AnimationFrameData::COL_ANIMATION = "Animation";
string AnimationFrameData::COL_FRAME_COUNT = "FrameCount";
string AnimationFrameData::COL_POSX = "PosX";
string AnimationFrameData::COL_POSY = "PosY";

void SQLiteAnimationFrame::query(const string& atlas, const string& animation, txVector<AnimationFrameData>& dataList)
{
	string conditionString;
	StringUtility::appendConditionString(conditionString, AnimationFrameData::COL_ATLAS, atlas, " and ");
	StringUtility::appendConditionString(conditionString, AnimationFrameData::COL_ANIMATION, animation, "");
	doSelect(dataList, conditionString);
}
bool SQLiteAnimationFrame::updateData(const AnimationFrameData& data)
{
	string updateString;
	StringUtility::appendUpdateInt(updateString, AnimationFrameData::COL_FRAME_COUNT, data.mFrameCount);
	StringUtility::appendUpdateIntArray(updateString, AnimationFrameData::COL_POSX, data.mPosX);
	StringUtility::appendUpdateIntArray(updateString, AnimationFrameData::COL_POSY, data.mPosY);
	StringUtility::removeLastComma(updateString);
	string conditionString;
	StringUtility::appendConditionString(conditionString, AnimationFrameData::COL_ATLAS, data.mAtlas, " and ");
	StringUtility::appendConditionString(conditionString, AnimationFrameData::COL_ANIMATION, data.mAnimation, "");
	return doUpdate(updateString, conditionString);
}

bool SQLiteAnimationFrame::insert(const AnimationFrameData& data)
{
	string valueString;
	data.insert(valueString);
	StringUtility::removeLastComma(valueString);
	return doInsert(valueString);
}
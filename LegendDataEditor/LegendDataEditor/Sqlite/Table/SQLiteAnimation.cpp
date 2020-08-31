#include "SQLiteAnimation.h"
#include "Utility.h"
#include "SQLiteDataReader.h"
#include "SQLite.h"

string AnimationData::COL_ID = "ID";
string AnimationData::COL_DESCRIPTION = "Description";
string AnimationData::COL_ATLAS = "Atlas";
string AnimationData::COL_ANIMATION = "Animation";
string AnimationData::COL_FRAME_COUNT = "FrameCount";
string AnimationData::COL_LOOP = "Loop";
string AnimationData::COL_ANIMATION_SPEED = "AnimationSpeed";
string AnimationData::COL_DIRCTION_COUNT = "DirectionCount";
string AnimationData::COL_ANIMATION_POSITION = "AnimationPosition";

bool SQLiteAnimation::updateData(const AnimationData& data)
{
	string updateString;
	StringUtility::appendUpdateIntArray(updateString, AnimationData::COL_ANIMATION_POSITION, data.mAnimationPosition);
	StringUtility::removeLastComma(updateString);
	string conditionString;
	StringUtility::appendConditionInt(conditionString, AnimationData::COL_ID, data.mID, "");
	return doUpdate(updateString, conditionString);
}
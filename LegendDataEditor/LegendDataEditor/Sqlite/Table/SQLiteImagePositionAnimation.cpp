#include "SQLiteImagePositionAnimation.h"

void SQLiteImagePositionAnimation::query(const string& atlas, const string& animation, TDImagePositionAnimation& data)
{
	array<char, 128> condition{ 0 };
	appendConditionString(condition, TDImagePositionAnimation::Atlas, atlas.c_str(), false, " and ");
	appendConditionString(condition, TDImagePositionAnimation::Animation, animation.c_str(), false);
	doSelect(data, condition.data());
}
bool SQLiteImagePositionAnimation::updateData(const TDImagePositionAnimation& data)
{
	array<char, 128> updateString{ 0 };
	appendUpdateInt(updateString, TDImagePositionAnimation::FrameCount, data.mFrameCount);
	appendUpdateInts(updateString, TDImagePositionAnimation::PosX, data.mPosX.data(), data.mPosX.size());
	appendUpdateInts(updateString, TDImagePositionAnimation::PosY, data.mPosY.data(), data.mPosY.size(), false);
	array<char, 128> conditionString{ 0 };
	appendConditionString(conditionString, TDImagePositionAnimation::Atlas, data.mAtlas.c_str(), false, " and ");
	appendConditionString(conditionString, TDImagePositionAnimation::Animation, data.mAnimation.c_str(), false);
	return doUpdate(updateString.data(), conditionString.data());
}
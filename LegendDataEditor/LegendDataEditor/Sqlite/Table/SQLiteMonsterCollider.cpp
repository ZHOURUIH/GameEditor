#include "SQLiteMonsterCollider.h"
#include "Utility.h"
#include "SQLiteDataReader.h"
#include "SQLite.h"

string MonsterColliderData::COL_ATLAS = "Atlas";
string MonsterColliderData::COL_ANIMATION = "Animation";
string MonsterColliderData::COL_FRAME_COUNT = "FrameCount";
string MonsterColliderData::COL_CENTER_X = "CenterX";
string MonsterColliderData::COL_CENTER_Y = "CenterY";
string MonsterColliderData::COL_WIDTH = "Width";
string MonsterColliderData::COL_HEIGHT = "Height";

void SQLiteMonsterCollider::query(const string& atlas, const string& animation, txVector<MonsterColliderData>& dataList)
{
	string conditionString;
	StringUtility::appendConditionString(conditionString, MonsterColliderData::COL_ATLAS, atlas, " and ");
	StringUtility::appendConditionString(conditionString, MonsterColliderData::COL_ANIMATION, animation, "");
	doSelect(dataList, conditionString);
}
bool SQLiteMonsterCollider::updateData(const MonsterColliderData& data)
{
	string updateString;
	StringUtility::appendUpdateInt(updateString, MonsterColliderData::COL_FRAME_COUNT, data.mFrameCount);
	StringUtility::appendUpdateIntArray(updateString, MonsterColliderData::COL_CENTER_X, data.mCenterX);
	StringUtility::appendUpdateIntArray(updateString, MonsterColliderData::COL_CENTER_Y, data.mCenterY);
	StringUtility::appendUpdateIntArray(updateString, MonsterColliderData::COL_WIDTH, data.mWidth);
	StringUtility::appendUpdateIntArray(updateString, MonsterColliderData::COL_HEIGHT, data.mHeight);
	StringUtility::removeLastComma(updateString);
	string conditionString;
	StringUtility::appendConditionString(conditionString, MonsterColliderData::COL_ATLAS, data.mAtlas, " and ");
	StringUtility::appendConditionString(conditionString, MonsterColliderData::COL_ANIMATION, data.mAnimation, "");
	return doUpdate(updateString, conditionString);
}

bool SQLiteMonsterCollider::insert(const MonsterColliderData& data)
{
	string valueString;
	data.insert(valueString);
	StringUtility::removeLastComma(valueString);
	return doInsert(valueString);
}
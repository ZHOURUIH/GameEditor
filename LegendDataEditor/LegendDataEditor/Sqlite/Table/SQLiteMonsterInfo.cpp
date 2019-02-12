#include "SQLiteMonsterInfo.h"
#include "Utility.h"
#include "SQLiteDataReader.h"
#include "SQLite.h"

string MonsterInfoData::COL_NAME = "Name";
string MonsterInfoData::COL_RACE = "Race";
string MonsterInfoData::COL_RACE_IMAGE = "RaceImage";
string MonsterInfoData::COL_APPERANCE = "Apperance";
string MonsterInfoData::COL_LEVEL = "Level";
string MonsterInfoData::COL_UNDEAD = "Undead";
string MonsterInfoData::COL_COOL_EYE = "CoolEye";
string MonsterInfoData::COL_HP = "HP";
string MonsterInfoData::COL_MP = "MP";
string MonsterInfoData::COL_ARMOR_CLASS = "ArmorClass";
string MonsterInfoData::COL_MAGIC_ARMOR_CLASS = "MagicArmorClass";
string MonsterInfoData::COL_DAMAGE_CLASS = "DamageClass";
string MonsterInfoData::COL_MAX_DAMAGE_CLASS = "MaxDamageClass";
string MonsterInfoData::COL_MAGIC_CLASS = "MagicClass";
string MonsterInfoData::COL_SC = "SC";
string MonsterInfoData::COL_SPEED = "Speed";
string MonsterInfoData::COL_HIT = "Hit";
string MonsterInfoData::COL_WALK_SPEED = "WalkSpeed";
string MonsterInfoData::COL_WALK_STEP = "WalkStep";
string MonsterInfoData::COL_WALK_WAIT = "WalkWait";
string MonsterInfoData::COL_ATTACK_SPEED = "AttackSpeed";
string MonsterInfoData::COL_EXP = "Exp";

void SQLiteMonsterInfo::query(const string& name, MonsterInfoData& data)
{
	string conditionString;
	StringUtility::appendConditionString(conditionString, MonsterInfoData::COL_NAME, name, "");
	doSelect(data, conditionString);
}

bool SQLiteMonsterInfo::insert(const MonsterInfoData& data)
{
	string valueString;
	data.insert(valueString);
	StringUtility::removeLastComma(valueString);
	return doInsert(valueString);
}

bool SQLiteMonsterInfo::update(const MonsterInfoData& data)
{
	string updateString;
	data.update(updateString);
	StringUtility::removeLastComma(updateString);
	string conditionStr;
	StringUtility::appendConditionString(conditionStr, MonsterInfoData::COL_NAME, data.mName, "");
	return doUpdate(updateString, conditionStr);
}
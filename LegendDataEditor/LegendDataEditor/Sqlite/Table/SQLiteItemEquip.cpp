#include "SQLiteItemEquip.h"
#include "Utility.h"
#include "SQLiteDataReader.h"
#include "SQLite.h"

string ItemEquipData::COL_ID = "ID";
string ItemEquipData::COL_NAME = "Name";
string ItemEquipData::COL_DESCRIPTION = "Description";
string ItemEquipData::COL_TYPE = "Type";
string ItemEquipData::COL_SHAPE = "Shape";
string ItemEquipData::COL_JOB = "Job";
string ItemEquipData::COL_SEX = "Sex";
string ItemEquipData::COL_LEVEL = "Level";
string ItemEquipData::COL_ANI_COUNT = "AniCount";
string ItemEquipData::COL_LOOKS = "Looks";
string ItemEquipData::COL_DURA_MAX = "DuraMax";
string ItemEquipData::COL_MIN_ARMOR = "MinArmor";
string ItemEquipData::COL_MAX_ARMOR = "MaxArmor";
string ItemEquipData::COL_MIN_MAGIC_ARMOR = "MinMagicArmor";
string ItemEquipData::COL_MAX_MAGIC_ARMOR = "MaxMagicArmor";
string ItemEquipData::COL_MIN_DAMAGE = "MinDamage";
string ItemEquipData::COL_MAX_DAMAGE = "MaxDamage";
string ItemEquipData::COL_MIN_MAGIC = "MinMagic";
string ItemEquipData::COL_MAX_MAGIC = "MaxMagic";
string ItemEquipData::COL_MIN_SORCERY = "MinSorcery";
string ItemEquipData::COL_MAX_SORCERY = "MaxSorcery";
string ItemEquipData::COL_LUCKY = "Lucky";
string ItemEquipData::COL_HIT_PROBABILITY = "HitProbability";
string ItemEquipData::COL_EVADE_PROBABILITY = "EvadeProbability";
string ItemEquipData::COL_WEIGHT = "Weight";
string ItemEquipData::COL_PRICE = "Price";

bool SQLiteItemEquip::query(int id, ItemEquipData& data)
{
	string conditionString;
	StringUtility::appendConditionInt(conditionString, ItemEquipData::COL_ID, id, "");
	return doSelect(data, conditionString);
}

bool SQLiteItemEquip::query(const string& name, ItemEquipData& data)
{
	string conditionString;
	StringUtility::appendConditionString(conditionString, ItemEquipData::COL_NAME, name, "");
	return doSelect(data, conditionString);
}

bool SQLiteItemEquip::insert(const ItemEquipData& data)
{
	string valueString;
	data.insert(valueString);
	StringUtility::removeLastComma(valueString);
	return doInsert(valueString);
}

bool SQLiteItemEquip::update(const ItemEquipData& data)
{
	string updateString;
	data.update(updateString);
	StringUtility::removeLastComma(updateString);
	string conditionStr;
	StringUtility::appendConditionInt(conditionStr, ItemEquipData::COL_ID, data.mID, "");
	return doUpdate(updateString, conditionStr);
}
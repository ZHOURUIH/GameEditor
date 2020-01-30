#include "SQLiteMonster.h"
#include "Utility.h"
#include "SQLiteDataReader.h"
#include "SQLite.h"

string MonsterData::COL_ID = "ID";
string MonsterData::COL_DESCRIBE = "Describe";
string MonsterData::COL_TEMPLATE_ID = "TemplateID";
string MonsterData::COL_LEVEL = "Level";
string MonsterData::COL_HP = "HP";
string MonsterData::COL_MIN_PHYSICS_DEFENCE = "MinPhysicsDefence";
string MonsterData::COL_MAX_PHYSICS_DEFENCE = "MaxPhysicsDefence";
string MonsterData::COL_MIN_MAGIC_DEFENCE = "MinMagicDefence";
string MonsterData::COL_MAX_MAGIC_DEFENCE = "MaxMagicDefence";
string MonsterData::COL_MIN_PHYSICS_ATTACK = "MinPhysicsAttack";
string MonsterData::COL_MAX_PHYSICS_ATTACK = "MaxPhysicsAttack";
string MonsterData::COL_MIN_MAGIC_ATTACK = "MinMagicAttack";
string MonsterData::COL_MAX_MAGIC_ATTACK = "MaxMagicAttack";
string MonsterData::COL_MIN_SORCERY_ATTACK = "MinSorceryAttack";
string MonsterData::COL_MAX_SORCERY_ATTACK = "MaxSorceryAttack";
string MonsterData::COL_MOVE_SPEED = "MoveSpeed";
string MonsterData::COL_HIT_PROBABLITY = "HitProbability";
string MonsterData::COL_EVADE_PROBABLITY = "EvadeProbability";
string MonsterData::COL_WALK_SPEED = "WalkSpeed";
string MonsterData::COL_WALK_STEP = "WalkStep";
string MonsterData::COL_WALK_WAIT = "WalkWait";
string MonsterData::COL_ATTACK_SPEED = "AttackSpeed";
string MonsterData::COL_EXP = "Exp";
string MonsterData::COL_DROP_LIST = "DropList";
string MonsterData::COL_SKILL = "Skill";
string MonsterData::COL_SKILL_LEVEL = "SkillLevel";

void SQLiteMonster::query(int id, MonsterData& data)
{
	string conditionString;
	StringUtility::appendConditionInt(conditionString, MonsterData::COL_ID, id, "");
	doSelect(data, conditionString);
}

bool SQLiteMonster::insert(const MonsterData& data)
{
	string valueString;
	data.insert(valueString);
	StringUtility::removeLastComma(valueString);
	return doInsert(valueString);
}

bool SQLiteMonster::update(const MonsterData& data)
{
	string updateString;
	data.update(updateString);
	StringUtility::removeLastComma(updateString);
	string conditionStr;
	StringUtility::appendConditionInt(conditionStr, MonsterData::COL_ID, data.mID, "");
	return doUpdate(updateString, conditionStr);
}
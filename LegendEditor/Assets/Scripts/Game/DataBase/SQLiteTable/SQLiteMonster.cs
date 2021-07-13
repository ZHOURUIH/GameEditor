using Mono.Data.Sqlite;
using System;
using System.Collections.Generic;

public class TDMonster : TableData
{
	public static string Describe = "Describe";
	public static string TemplateID = "TemplateID";
	public static string Level = "Level";
	public static string HP = "HP";
	public static string MinPhysicsDefence = "MinPhysicsDefence";
	public static string MaxPhysicsDefence = "MaxPhysicsDefence";
	public static string MinMagicDefence = "MinMagicDefence";
	public static string MaxMagicDefence = "MaxMagicDefence";
	public static string MinPhysicsAttack = "MinPhysicsAttack";
	public static string MaxPhysicsAttack = "MaxPhysicsAttack";
	public static string MinMagicAttack = "MinMagicAttack";
	public static string MaxMagicAttack = "MaxMagicAttack";
	public static string MinSorceryAttack = "MinSorceryAttack";
	public static string MaxSorceryAttack = "MaxSorceryAttack";
	public static string MoveSpeed = "MoveSpeed";
	public static string HitProbability = "HitProbability";
	public static string EvadeProbability = "EvadeProbability";
	public static string WalkSpeed = "WalkSpeed";
	public static string WalkStep = "WalkStep";
	public static string WalkWait = "WalkWait";
	public static string AttackSpeed = "AttackSpeed";
	public static string Exp = "Exp";
	public static string DropList = "DropList";
	public static string Skill = "Skill";
	public static string SkillLevel = "SkillLevel";
	public static string SearchTargetDistance = "SearchTargetDistance";
	public static string TelentBuff = "TelentBuff";
	public string mDescribe;
	public int mTemplateID;
	public int mLevel;
	public int mHP;
	public int mMinPhysicsDefence;
	public int mMaxPhysicsDefence;
	public int mMinMagicDefence;
	public int mMaxMagicDefence;
	public int mMinPhysicsAttack;
	public int mMaxPhysicsAttack;
	public int mMinMagicAttack;
	public int mMaxMagicAttack;
	public int mMinSorceryAttack;
	public int mMaxSorceryAttack;
	public float mMoveSpeed;
	public float mHitProbability;
	public float mEvadeProbability;
	public int mWalkSpeed;
	public float mWalkStep;            // 一次移动的距离,单位像素
	public float mWalkWait;            // 移动每步的等待,单位秒
	public int mAttackSpeed;
	public int mExp;
	public string mDropList;
	public List<int> mSkill = new List<int>();
	public List<int> mSkillLevel = new List<int>();
	public int mSearchTargetDistance;
	public List<int> mTelentBuff = new List<int>();
	public override void parse(SqliteDataReader reader)
	{
		base.parse(reader);
		parseParam(reader, ref mDescribe, Describe);
		parseParam(reader, ref mTemplateID, TemplateID);
		parseParam(reader, ref mLevel, Level);
		parseParam(reader, ref mHP, HP);
		parseParam(reader, ref mMinPhysicsDefence, MinPhysicsDefence);
		parseParam(reader, ref mMaxPhysicsDefence, MaxPhysicsDefence);
		parseParam(reader, ref mMinMagicDefence, MinMagicDefence);
		parseParam(reader, ref mMaxMagicDefence, MaxMagicDefence);
		parseParam(reader, ref mMinPhysicsAttack, MinPhysicsAttack);
		parseParam(reader, ref mMaxPhysicsAttack, MaxPhysicsAttack);
		parseParam(reader, ref mMinMagicAttack, MinMagicAttack);
		parseParam(reader, ref mMaxMagicAttack, MaxMagicAttack);
		parseParam(reader, ref mMinSorceryAttack, MinSorceryAttack);
		parseParam(reader, ref mMaxSorceryAttack, MaxSorceryAttack);
		parseParam(reader, ref mMoveSpeed, MoveSpeed);
		parseParam(reader, ref mHitProbability, HitProbability);
		parseParam(reader, ref mEvadeProbability, EvadeProbability);
		parseParam(reader, ref mWalkSpeed, WalkSpeed);
		parseParam(reader, ref mWalkStep, WalkStep);
		parseParam(reader, ref mWalkWait, WalkWait);
		parseParam(reader, ref mAttackSpeed, AttackSpeed);
		parseParam(reader, ref mExp, Exp);
		parseParam(reader, ref mDropList, DropList);
		parseParam(reader, ref mSkill, Skill);
		parseParam(reader, ref mSkillLevel, SkillLevel);
		parseParam(reader, ref mSearchTargetDistance, SearchTargetDistance);
		parseParam(reader, ref mTelentBuff, TelentBuff);
	}
}

public class SQLiteMonster : SQLiteTable
{
	public SQLiteMonster()
		:base(typeof(TDMonster)){}
}
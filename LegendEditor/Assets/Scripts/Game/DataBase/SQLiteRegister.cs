using UnityEngine;
using System;
using System.Collections;
using System.Collections.Generic;

public class SQLiteRegister : GameBase
{
	public static void registeAllTable()
	{
		registeTable(ref mSQLiteSound, "Sound");
		registeTable(ref mSQLiteMonster, "Monster");
		registeTable(ref mSQLiteSceneMap, "SceneMap");
		registeTable(ref mSQLiteLog, "Log");
		registeTable(ref mSQLiteImagePositionAnimation, "ImagePositionAnimation");
		registeTable(ref mSQLiteNPC, "NPC");
		registeTable(ref mSQLiteImagePositionIcon, "ImagePositionIcon");
		registeTable(ref mSQLiteImagePositionEffect, "ImagePositionEffect");
		registeTable(ref mSQLitePlayerSkill, "PlayerSkill");
		registeTable(ref mSQLiteMonsterSkill, "MonsterSkill");
		registeTable(ref mSQLiteSkillBullet, "SkillBullet");
		registeTable(ref mSQLiteSkillEffectDirection, "SkillEffectDirection");
		registeTable(ref mSQLiteItemBox, "ItemBox");
		registeTable(ref mSQLiteItemEquip, "ItemEquip");
		registeTable(ref mSQLiteItemConsumable, "ItemConsumable");
		registeTable(ref mSQLiteItemMaterial, "ItemMaterial");
		registeTable(ref mSQLiteItemSkillBook, "ItemSkillBook");
		registeTable(ref mSQLiteBuff, "Buff");
		registeTable(ref mSQLiteBuffDetail, "BuffDetail");
		registeTable(ref mSQLiteMonsterTemplate, "MonsterTemplate");
		registeTable(ref mSQLiteMapTileIndex, "MapTileIndex");
		registeTable(ref mSQLiteAnimation, "Animation");
		registeTable(ref mSQLiteSceneMapTransfer, "SceneMapTransfer");
		registeTable(ref mSQLiteMapEffectPosition, "MapEffectPosition");
	}
	//-------------------------------------------------------------------------------------------------------------
	protected static void registeTable<T>(ref T table, string name) where T : SQLiteTable, new()
	{
		table = mSQLite.registeTable<T>(name);
	}
}
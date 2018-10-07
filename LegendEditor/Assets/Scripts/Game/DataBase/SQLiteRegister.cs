using UnityEngine;
using System;
using System.Collections;
using System.Collections.Generic;

public class SQLiteRegister : GameBase
{
	public static void registeAllTable()
	{
		registeTable(ref mSQLiteCloth);
		registeTable(ref mSQLiteClothFrame);
		registeTable(ref mSQLiteEffect);
		registeTable(ref mSQLiteEffectFrame);
		registeTable(ref mSQLiteMonster);
		registeTable(ref mSQLiteMonsterFrame);
		registeTable(ref mSQLiteWeapon);
		registeTable(ref mSQLiteWeaponFrame);
	}
	//-------------------------------------------------------------------------------------------------------------
	protected static void registeTable<T>(ref T table) where T : SQLiteTable, new()
	{
		table = mSQLite.registeTable<T>();
	}
}
using Mono.Data.Sqlite;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

public class WeaponData
{
	public string mLabel;
	public int mID;
	public string mDesc;
}

public class SQLiteWeapon : SQLiteTable
{
	string COL_LABLE = "WeaponLabel";
	string COL_ID = "WeaponID";
	string COL_DESC = "Desc";
	public SQLiteWeapon(SQLite sqlite)
		:base("Weapon", sqlite)
	{}
	public void query(int weaponID, out WeaponData data)
	{
		data = new WeaponData();
		SqliteDataReader reader = mSQLite.query("SELECT * FROM " + mTableName + " WHERE " + COL_ID + " = " + StringUtility.intToString(weaponID));
		while (reader.Read())
		{
			data.mLabel = reader[COL_LABLE].ToString();
			data.mID = StringUtility.stringToInt(reader[COL_ID].ToString());
			data.mDesc = reader[COL_DESC].ToString();
		}
		reader.Close();
	}
};
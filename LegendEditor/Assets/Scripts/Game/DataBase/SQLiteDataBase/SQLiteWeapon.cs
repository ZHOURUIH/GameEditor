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
	string COL_LABEL = "WeaponLabel";
	string COL_ID = "WeaponID";
	string COL_DESC = "Desc";
	public SQLiteWeapon(SQLite sqlite)
		:base("Weapon", sqlite)
	{}
	public void query(int weaponID, out WeaponData data)
	{
		string condition = "";
		appendConditionInt(ref condition, COL_ID, weaponID, "");
		parseReader(doQuery(condition), out data);
	}
	public void insert(WeaponData data)
	{
		string valueString = "";
		appendValueString(ref valueString, data.mLabel);
		appendValueInt(ref valueString, data.mID);
		appendValueString(ref valueString, data.mDesc, true);
		doInsert(valueString);
	}
	//--------------------------------------------------------------------------------------------------------------------------
	protected void parseReader(SqliteDataReader reader, out WeaponData data)
	{
		data = new WeaponData();
		while (reader.Read())
		{
			data.mLabel = reader[COL_LABEL].ToString();
			data.mID = StringUtility.stringToInt(reader[COL_ID].ToString());
			data.mDesc = reader[COL_DESC].ToString();
			break;
		}
		reader.Close();
	}
};
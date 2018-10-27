using Mono.Data.Sqlite;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

public class WeaponData : TableData
{
	public static string COL_LABEL = "WeaponLabel";
	public static string COL_ID = "WeaponID";
	public static string COL_DESC = "Desc";
	public static string COL_OCCUPATION = "Occupation";
	public static string COL_RESOURCE = "Resource";
	public string mLabel;
	public int mID;
	public string mDesc;
	public string mOccupation;
	public string mResource;
	public override void parse(SqliteDataReader reader)
	{
		mLabel = reader[COL_LABEL].ToString();
		mID = stringToInt(reader[COL_ID].ToString());
		mDesc = reader[COL_DESC].ToString();
		mOccupation = reader[COL_OCCUPATION].ToString();
		mResource = reader[COL_RESOURCE].ToString();
	}
}

public class SQLiteWeapon : SQLiteTable
{
	public SQLiteWeapon()
		:base("Weapon")
	{}
	public void query(int id, out WeaponData data)
	{
		string condition = "";
		appendConditionInt(ref condition, WeaponData.COL_ID, id, "");
		parseReader(doQuery(condition), out data);
	}
	public void queryAll(out List<WeaponData> dataList)
	{
		parseReader(doQuery(), out dataList);
	}
	public void insert(WeaponData data)
	{
		string valueString = "";
		appendValueString(ref valueString, data.mLabel);
		appendValueInt(ref valueString, data.mID);
		appendValueString(ref valueString, data.mDesc);
		appendValueString(ref valueString, data.mOccupation);
		appendValueString(ref valueString, data.mResource, true);
		doInsert(valueString);
	}
};
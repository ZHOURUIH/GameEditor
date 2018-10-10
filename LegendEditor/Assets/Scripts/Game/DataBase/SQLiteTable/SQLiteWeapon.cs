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
	public string mOccupation;
	public string mResource;
}

public class SQLiteWeapon : SQLiteTable
{
	string COL_LABEL = "WeaponLabel";
	string COL_ID = "WeaponID";
	string COL_DESC = "Desc";
	string COL_OCCUPATION = "Occupation";
	string COL_RESOURCE = "Resource";
	public SQLiteWeapon()
		:base("Weapon")
	{}
	public void query(int id, out WeaponData data)
	{
		string condition = "";
		appendConditionInt(ref condition, COL_ID, id, "");
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
	//--------------------------------------------------------------------------------------------------------------------------
	protected void parseReader(SqliteDataReader reader, out WeaponData data)
	{
		parseReaderOne(reader, out data);
		reader.Close();
	}
	protected void parseReader(SqliteDataReader reader, out List<WeaponData> dataList)
	{
		dataList = new List<WeaponData>();
		while (reader.Read())
		{
			WeaponData data;
			parseReaderOne(reader, out data);
			dataList.Add(data);
		}
		reader.Close();
	}
	protected void parseReaderOne(SqliteDataReader reader, out WeaponData data)
	{
		data = new WeaponData();
		data.mLabel = reader[COL_LABEL].ToString();
		data.mID = StringUtility.stringToInt(reader[COL_ID].ToString());
		data.mDesc = reader[COL_DESC].ToString();
		data.mOccupation = reader[COL_OCCUPATION].ToString();
		data.mResource = reader[COL_RESOURCE].ToString();
	}
};
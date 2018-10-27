using Mono.Data.Sqlite;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

public class ClothData : TableData
{
	public static string COL_LABEL = "ClothLabel";
	public static string COL_ID = "ClothID";
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

public class SQLiteCloth : SQLiteTable
{
	public SQLiteCloth()
		:base("Cloth"){}
	public void query(int id, out ClothData data)
	{
		string condition = "";
		appendConditionInt(ref condition, ClothData.COL_ID, id, "");
		parseReader(doQuery(condition), out data);
	}
	public void queryAll(out List<ClothData> dataList)
	{
		parseReader(doQuery(), out dataList);
	}
	public void insert(ClothData data)
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
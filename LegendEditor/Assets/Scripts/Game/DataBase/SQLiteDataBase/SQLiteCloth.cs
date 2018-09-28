using Mono.Data.Sqlite;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

public class ClothData
{
	public string mLabel;
	public int mID;
	public string mDesc;
}

public class SQLiteCloth : SQLiteTable
{
	string COL_LABEL = "ClothLabel";
	string COL_ID = "ClothID";
	string COL_DESC = "Desc";
	public SQLiteCloth(SQLite sqlite)
		:base("Cloth", sqlite)
	{
		;
	}
	public void query(int clothID, out ClothData data)
	{
		string condition = "";
		appendConditionInt(ref condition, COL_ID, clothID, "");
		parseReader(doQuery(condition), out data);
	}

	public void insert(ClothData data)
	{
		string valueString = "";
		appendValueString(ref valueString, data.mLabel);
		appendValueInt(ref valueString, data.mID);
		appendValueString(ref valueString, data.mDesc, true);
		doInsert(valueString);
	}
	protected void parseReader(SqliteDataReader reader, out ClothData data)
	{
		data = new ClothData();
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
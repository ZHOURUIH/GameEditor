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
		data = new ClothData();
		SqliteDataReader reader = mSQLite.query("SELECT * FROM " + mTableName + " WHERE " + COL_ID + " = " + StringUtility.intToString(clothID));
		while (reader.Read())
		{
			data.mLabel = reader[COL_LABEL].ToString();
			data.mID = StringUtility.stringToInt(reader[COL_ID].ToString());
			data.mDesc = reader[COL_DESC].ToString();
		}
		reader.Close();
	}
};
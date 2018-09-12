using Mono.Data.Sqlite;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

public class MonsterData
{
	public string mLabel;
	public int mID;
	public string mDesc;
}

public class SQLiteMonster : SQLiteTable
{
	string COL_LABLE = "MonsterLabel";
	string COL_ID = "MonsterID";
	string COL_DESC = "Desc";
	public SQLiteMonster(SQLite sqlite)
		:base("Monster", sqlite)
	{}
	public void query(int monsterID, out MonsterData data)
	{
		data = new MonsterData();
		SqliteDataReader reader = mSQLite.query("SELECT * FROM " + mTableName + " WHERE " + COL_ID + " = " + StringUtility.intToString(monsterID));
		while (reader.Read())
		{
			data.mLabel = reader[COL_LABLE].ToString();
			data.mID = StringUtility.stringToInt(reader[COL_ID].ToString());
			data.mDesc = reader[COL_DESC].ToString();
		}
		reader.Close();
	}
};
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
	string COL_LABEL = "MonsterLabel";
	string COL_ID = "MonsterID";
	string COL_DESC = "Desc";
	public SQLiteMonster(SQLite sqlite)
		:base("Monster", sqlite)
	{}
	public void query(int monsterID, out MonsterData data)
	{
		string condition = "";
		appendConditionInt(ref condition, COL_ID, monsterID, "");
		parseReader(doQuery(condition), out data);
	}
	public void insert(MonsterData data)
	{
		string valueString = "";
		appendValueString(ref valueString, data.mLabel);
		appendValueInt(ref valueString, data.mID);
		appendValueString(ref valueString, data.mDesc, true);
		doInsert(valueString);
	}
	//----------------------------------------------------------------------------------------------------------------
	protected void parseReader(SqliteDataReader reader, out MonsterData data)
	{
		data = new MonsterData();
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
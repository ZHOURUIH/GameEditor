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
	public string mResource;
}

public class SQLiteMonster : SQLiteTable
{
	string COL_LABEL = "MonsterLabel";
	string COL_ID = "MonsterID";
	string COL_DESC = "Desc";
	string COL_RESOURCE = "Resource";
	public SQLiteMonster()
		:base("Monster")
	{}
	public void query(int id, out MonsterData data)
	{
		string condition = "";
		appendConditionInt(ref condition, COL_ID, id, "");
		parseReader(doQuery(condition), out data);
	}
	public void queryAll(out List<MonsterData> dataList)
	{
		parseReader(doQuery(), out dataList);
	}
	public void insert(MonsterData data)
	{
		string valueString = "";
		appendValueString(ref valueString, data.mLabel);
		appendValueInt(ref valueString, data.mID);
		appendValueString(ref valueString, data.mDesc);
		appendValueString(ref valueString, data.mResource, true);
		doInsert(valueString);
	}
	//----------------------------------------------------------------------------------------------------------------
	protected void parseReader(SqliteDataReader reader, out MonsterData data)
	{
		parseReaderOne(reader, out data);
		reader.Close();
	}
	protected void parseReader(SqliteDataReader reader, out List<MonsterData> dataList)
	{
		dataList = new List<MonsterData>();
		while (reader.Read())
		{
			MonsterData data;
			parseReaderOne(reader, out data);
			dataList.Add(data);
		}
		reader.Close();
	}
	protected void parseReaderOne(SqliteDataReader reader, out MonsterData data)
	{
		data = new MonsterData();
		data.mLabel = reader[COL_LABEL].ToString();
		data.mID = StringUtility.stringToInt(reader[COL_ID].ToString());
		data.mDesc = reader[COL_DESC].ToString();
		data.mResource = reader[COL_RESOURCE].ToString();
	}
};
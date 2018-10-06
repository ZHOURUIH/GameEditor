using Mono.Data.Sqlite;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

public class EffectData
{
	public string mLabel;
	public int mID;
	public string mDesc;
	public string mResource;
	public int mDirectionCount;
}

public class SQLiteEffect : SQLiteTable
{
	public string COL_LABEL = "Label";
	public string COL_ID = "ID";
	public string COL_DESC = "Desc";
	public string COL_RESOURCE = "Resource";
	public string COL_DIRECITON_COUNT = "DirectionCount";
	public SQLiteEffect(SQLite sqlite)
		:base("Effect", sqlite)
	{}
	public void query(int id, out EffectData data)
	{
		string condition = "";
		appendConditionInt(ref condition, COL_ID, id, "");
		parseReader(doQuery(condition), out data);
	}
	public void queryAll(out List<EffectData> dataList)
	{
		parseReader(doQuery(), out dataList);
	}
	public void insert(EffectData data)
	{
		string valueString = "";
		appendValueString(ref valueString, data.mLabel);
		appendValueInt(ref valueString, data.mID);
		appendValueString(ref valueString, data.mDesc);
		appendValueString(ref valueString, data.mResource);
		appendValueInt(ref valueString, data.mDirectionCount, true);
		doInsert(valueString);
	}
	//--------------------------------------------------------------------------------------------------------------------------
	protected void parseReader(SqliteDataReader reader, out EffectData data)
	{
		parseReaderOne(reader, out data);
		reader.Close();
	}
	protected void parseReader(SqliteDataReader reader, out List<EffectData> dataList)
	{
		dataList = new List<EffectData>();
		while (reader.Read())
		{
			EffectData data;
			parseReaderOne(reader, out data);
			dataList.Add(data);
		}
		reader.Close();
	}
	protected void parseReaderOne(SqliteDataReader reader, out EffectData data)
	{
		data = new EffectData();
		data.mLabel = reader[COL_LABEL].ToString();
		data.mID = StringUtility.stringToInt(reader[COL_ID].ToString());
		data.mDesc = reader[COL_DESC].ToString();
		data.mResource = reader[COL_RESOURCE].ToString();
		data.mDirectionCount = StringUtility.stringToInt(reader[COL_DIRECITON_COUNT].ToString());
	}
};
using Mono.Data.Sqlite;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

public class SceneMapData : TableData
{
	public static string COL_ID = "ID";
	public static string COL_LABEL = "Label";
	public static string COL_RESOURCE = "Resource";
	public int mID;
	public string mLabel;
	public string mResource;
	public override void parse(SqliteDataReader reader)
	{
		mID = stringToInt(reader[COL_ID].ToString());
		mLabel = reader[COL_LABEL].ToString();
		mResource = reader[COL_RESOURCE].ToString();
	}
}

public class SQLiteSceneMap : SQLiteTable
{
	public SQLiteSceneMap()
		:base("SceneMap")
	{}
	public void query(int id, out SceneMapData data)
	{
		string condition = "";
		appendConditionInt(ref condition, SceneMapData.COL_ID, id, "");
		parseReader(doQuery(condition), out data);
	}
	public void queryAll(out List<SceneMapData> dataList)
	{
		parseReader(doQuery(), out dataList);
	}
	public void insert(SceneMapData data)
	{
		string valueString = "";
		appendValueInt(ref valueString, data.mID);
		appendValueString(ref valueString, data.mLabel);
		appendValueString(ref valueString, data.mResource, true);
		doInsert(valueString);
	}
};
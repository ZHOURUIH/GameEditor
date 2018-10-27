using Mono.Data.Sqlite;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

public class MonsterFrameData : TableData
{
	public static string COL_ID = "MonsterID";
	public static string COL_LABEL = "Label";
	public static string COL_DIRECTION = "Direction";
	public static string COL_ACTION = "Action";
	public static string COL_FRAME_COUNT = "FrameCount";
	public static string COL_POSX = "PosX";
	public static string COL_POSY = "PosY";
	public int mID;
	public string mLabel;
	public int mDirection;
	public string mAction;
	public int mFrameCount;
	public List<int> mPosX;
	public List<int> mPosY;
	public override void parse(SqliteDataReader reader)
	{
		mID = stringToInt(reader[COL_ID].ToString());
		mLabel = reader[COL_LABEL].ToString();
		mDirection = stringToInt(reader[COL_DIRECTION].ToString());
		mAction = reader[COL_ACTION].ToString();
		mFrameCount = stringToInt(reader[COL_FRAME_COUNT].ToString());
		stringToIntArray(reader[COL_POSX].ToString(), ref mPosX);
		stringToIntArray(reader[COL_POSY].ToString(), ref mPosY);
	}
}

public class SQLiteMonsterFrame : SQLiteTable
{
	public SQLiteMonsterFrame()
		:base("MonsterFrame")
	{}
	public void query(int id, out List<MonsterFrameData> dataList)
	{
		string condition = "";
		appendConditionInt(ref condition, MonsterFrameData.COL_ID, id, "");
		parseReader(doQuery(condition), out dataList);
	}
	public void query(int id, int direction, out List<MonsterFrameData> dataList)
	{
		string condition = "";
		appendConditionInt(ref condition, MonsterFrameData.COL_ID, id, " and ");
		appendConditionInt(ref condition, MonsterFrameData.COL_DIRECTION, direction, "");
		parseReader(doQuery(condition), out dataList);
	}
	public void query(int id, int direction, string action, out List<MonsterFrameData> dataList)
	{
		string condition = "";
		appendConditionInt(ref condition, MonsterFrameData.COL_ID, id, " and ");
		appendConditionInt(ref condition, MonsterFrameData.COL_DIRECTION, direction, " and ");
		appendConditionString(ref condition, MonsterFrameData.COL_ACTION, action, "");
		parseReader(doQuery(condition), out dataList);
	}
	public void updateData(MonsterFrameData data)
	{
		string updateString = "";
		appendUpdateInt(ref updateString, MonsterFrameData.COL_FRAME_COUNT, data.mFrameCount);
		appendUpdateIntArray(ref updateString, MonsterFrameData.COL_FRAME_COUNT, data.mPosX);
		appendUpdateIntArray(ref updateString, MonsterFrameData.COL_FRAME_COUNT, data.mPosY, true);
		string condition = "";
		appendConditionInt(ref condition, MonsterFrameData.COL_ID, data.mID, " and ");
		appendConditionInt(ref condition, MonsterFrameData.COL_DIRECTION, data.mDirection, " and ");
		appendConditionString(ref condition, MonsterFrameData.COL_ACTION, data.mAction, "");
		doUpdate(updateString, condition);
	}
	public void insert(MonsterFrameData data)
	{
		string valueString = "";
		appendValueInt(ref valueString, data.mID);
		appendValueString(ref valueString, data.mLabel);
		appendValueInt(ref valueString, data.mDirection);
		appendValueString(ref valueString, data.mAction);
		appendValueInt(ref valueString, data.mFrameCount);
		appendValueIntArray(ref valueString, data.mPosX);
		appendValueIntArray(ref valueString, data.mPosY, true);
		doInsert(valueString);
	}
};
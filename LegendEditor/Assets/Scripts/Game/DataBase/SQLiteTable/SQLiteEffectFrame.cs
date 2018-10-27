using Mono.Data.Sqlite;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

public class EffectFrameData : TableData
{
	public static string COL_ID = "ID";
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

public class SQLiteEffectFrame : SQLiteTable
{
	public SQLiteEffectFrame()
		:base("EffectFrame")
	{}
	public void query(int ID, out List<EffectFrameData> dataList)
	{
		string condition = "";
		appendConditionInt(ref condition, EffectFrameData.COL_ID, ID, "");
		parseReader(doQuery(condition), out dataList);
	}
	public void query(int ID, int direction, out List<EffectFrameData> dataList)
	{
		string condition = "";
		appendConditionInt(ref condition, EffectFrameData.COL_ID, ID, " and ");
		appendConditionInt(ref condition, EffectFrameData.COL_DIRECTION, direction, "");
		parseReader(doQuery(condition), out dataList);
	}
	public void query(int ID, int direction, string action, out List<EffectFrameData> dataList)
	{
		string condition = "";
		appendConditionInt(ref condition, EffectFrameData.COL_ID, ID, " and ");
		appendConditionInt(ref condition, EffectFrameData.COL_DIRECTION, direction, " and ");
		appendConditionString(ref condition, EffectFrameData.COL_ACTION, action, "");
		parseReader(doQuery(condition), out dataList);
	}
	public void updateData(EffectFrameData data)
	{
		string updateString = "";
		appendUpdateInt(ref updateString, EffectFrameData.COL_FRAME_COUNT, data.mFrameCount);
		appendUpdateIntArray(ref updateString, EffectFrameData.COL_POSX, data.mPosX);
		appendUpdateIntArray(ref updateString, EffectFrameData.COL_POSY, data.mPosY, true);
		string condition = "";
		appendConditionInt(ref condition, EffectFrameData.COL_ID, data.mID, " and ");
		appendConditionInt(ref condition, EffectFrameData.COL_DIRECTION, data.mDirection, " and ");
		appendConditionString(ref condition, EffectFrameData.COL_ACTION, data.mAction, "");
		doUpdate(updateString, condition);
	}
	public void insert(EffectFrameData data)
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
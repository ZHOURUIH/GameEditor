using Mono.Data.Sqlite;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

public class ClothFrameData : TableData
{
	public static string COL_ID = "ClothID";
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

public class SQLiteClothFrame : SQLiteTable
{
	public SQLiteClothFrame()
		:base("ClothFrame"){}
	public void query(int id, out List<ClothFrameData> dataList)
	{
		string condition = "";
		appendConditionInt(ref condition, ClothFrameData.COL_ID, id, "");
		parseReader(doQuery(condition), out dataList);
	}
	public void query(int id, int direction, out List<ClothFrameData> dataList)
	{
		string condition = "";
		appendConditionInt(ref condition, ClothFrameData.COL_ID, id, " and ");
		appendConditionInt(ref condition, ClothFrameData.COL_DIRECTION, direction, "");
		parseReader(doQuery(condition), out dataList);
	}
	public void query(int id, int direction, string action, out List<ClothFrameData> dataList)
	{
		string condition = "";
		appendConditionInt(ref condition, ClothFrameData.COL_ID, id, " and ");
		appendConditionInt(ref condition, ClothFrameData.COL_DIRECTION, direction, " and ");
		appendConditionString(ref condition, ClothFrameData.COL_ACTION, action, "");
		parseReader(doQuery(condition), out dataList);
	}
	public void updateData(ClothFrameData data)
	{
		string valueString = "";
		appendUpdateInt(ref valueString, ClothFrameData.COL_FRAME_COUNT, data.mFrameCount);
		appendUpdateIntArray(ref valueString, ClothFrameData.COL_POSX, data.mPosX);
		appendUpdateIntArray(ref valueString, ClothFrameData.COL_POSY, data.mPosY);
		string condition = "";
		appendConditionInt(ref condition, ClothFrameData.COL_ID, data.mID, " and ");
		appendConditionInt(ref condition, ClothFrameData.COL_DIRECTION, data.mDirection, " and ");
		appendConditionString(ref condition, ClothFrameData.COL_ACTION, data.mAction, "");
		doUpdate(valueString, condition);
	}
	public void insert(ClothFrameData data)
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
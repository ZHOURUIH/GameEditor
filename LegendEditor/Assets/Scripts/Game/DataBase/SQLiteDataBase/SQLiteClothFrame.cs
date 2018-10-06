using Mono.Data.Sqlite;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

public class ClothFrameData
{
	public int mID;
	public string mLabel;
	public int mDirection;
	public string mAction;
	public int mFrameCount;
	public List<int> mPosX;
	public List<int> mPosY;
}

public class SQLiteClothFrame : SQLiteTable
{
	string COL_ID = "ClothID";
	string COL_LABEL = "Label";
	string COL_DIRECTION = "Direction";
	string COL_ACTION = "Action";
	string COL_FRAME_COUNT = "FrameCount";
	string COL_POSX = "PosX";
	string COL_POSY = "PosY";
	public SQLiteClothFrame(SQLite sqlite)
		:base("ClothFrame", sqlite)
	{
		;
	}
	public void query(int id, out List<ClothFrameData> dataList)
	{
		string condition = "";
		appendConditionInt(ref condition, COL_ID, id, "");
		parseReader(doQuery(condition), out dataList);
	}
	public void query(int id, int direction, out List<ClothFrameData> dataList)
	{
		string condition = "";
		appendConditionInt(ref condition, COL_ID, id, " and ");
		appendConditionInt(ref condition, COL_DIRECTION, direction, "");
		parseReader(doQuery(condition), out dataList);
	}
	public void query(int id, int direction, string action, out List<ClothFrameData> dataList)
	{
		string condition = "";
		appendConditionInt(ref condition, COL_ID, id, " and ");
		appendConditionInt(ref condition, COL_DIRECTION, direction, " and ");
		appendConditionString(ref condition, COL_ACTION, action, "");
		parseReader(doQuery(condition), out dataList);
	}
	public void updateData(ClothFrameData data)
	{
		string valueString = "";
		appendUpdateInt(ref valueString, COL_FRAME_COUNT, data.mFrameCount);
		appendUpdateIntArray(ref valueString, COL_POSX, data.mPosX);
		appendUpdateIntArray(ref valueString, COL_POSY, data.mPosY);
		string condition = "";
		appendConditionInt(ref condition, COL_ID, data.mID, " and ");
		appendConditionInt(ref condition, COL_DIRECTION, data.mDirection, " and ");
		appendConditionString(ref condition, COL_ACTION, data.mAction, "");
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
//------------------------------------------------------------------------------------------------------------------------
protected void parseReader(SqliteDataReader reader, out List<ClothFrameData> dataList)
	{
		dataList = new List<ClothFrameData>();
		while (reader.Read())
		{
			ClothFrameData data = new ClothFrameData();
			data.mID = StringUtility.stringToInt(reader[COL_ID].ToString());
			data.mLabel = reader[COL_LABEL].ToString();
			data.mDirection = StringUtility.stringToInt(reader[COL_DIRECTION].ToString());
			data.mAction = reader[COL_ACTION].ToString();
			data.mFrameCount = StringUtility.stringToInt(reader[COL_FRAME_COUNT].ToString());
			StringUtility.stringToIntArray(reader[COL_POSX].ToString(), ref data.mPosX);
			StringUtility.stringToIntArray(reader[COL_POSY].ToString(), ref data.mPosY);
			dataList.Add(data);
		}
		reader.Close();
	}
};
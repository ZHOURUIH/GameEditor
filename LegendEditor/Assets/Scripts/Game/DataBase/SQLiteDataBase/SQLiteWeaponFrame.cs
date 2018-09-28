using Mono.Data.Sqlite;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

public class WeaponFrameData
{
	public int mID;
	public int mDirection;
	public string mAction;
	public int mFrameCount;
	public List<float> mPosX;
	public List<float> mPosY;
}

public class SQLiteWeaponFrame : SQLiteTable
{
	string COL_ID = "WeaponID";
	string COL_DIRECTION = "Direction";
	string COL_ACTION = "Action";
	string COL_FRAME_COUNT = "FrameCount";
	string COL_POSX = "PosX";
	string COL_POSY = "PosY";
	public SQLiteWeaponFrame(SQLite sqlite)
		:base("WeaponFrame", sqlite)
	{}
	public void query(int weaponID, out List<WeaponFrameData> dataList)
	{
		string condition = "";
		appendConditionInt(ref condition, COL_ID, weaponID, "");
		parseReader(doQuery(condition), out dataList);
	}
	public void query(int weaponID, int direction, out List<WeaponFrameData> dataList)
	{
		string condition = "";
		appendConditionInt(ref condition, COL_ID, weaponID, " and ");
		appendConditionInt(ref condition, COL_DIRECTION, direction, "");
		parseReader(doQuery(condition), out dataList);
	}
	public void query(int weaponID, int direction, string action, out List<WeaponFrameData> dataList)
	{
		string condition = "";
		appendConditionInt(ref condition, COL_ID, weaponID, " and ");
		appendConditionInt(ref condition, COL_DIRECTION, direction, " and ");
		appendConditionString(ref condition, COL_ACTION, action, "");
		parseReader(doQuery(condition), out dataList);
	}
	public void updateData(WeaponFrameData data)
	{
		string updateString = "";
		appendUpdateInt(ref updateString, COL_FRAME_COUNT, data.mFrameCount);
		appendUpdateFloatArray(ref updateString, COL_POSX, data.mPosX);
		appendUpdateFloatArray(ref updateString, COL_POSY, data.mPosY, true);
		string condition = "";
		appendConditionInt(ref condition, COL_ID, data.mID, " and ");
		appendConditionInt(ref condition, COL_DIRECTION, data.mDirection, " and ");
		appendConditionString(ref condition, COL_ACTION, data.mAction, "");
		doUpdate(updateString, condition);
	}
	public void insert(WeaponFrameData data)
	{
		string valueString = "";
		appendValueInt(ref valueString, data.mID);
		appendValueInt(ref valueString, data.mDirection);
		appendValueString(ref valueString, data.mAction);
		appendValueInt(ref valueString, data.mFrameCount);
		appendValueFloatArray(ref valueString, data.mPosX);
		appendValueFloatArray(ref valueString, data.mPosY, true);
		doInsert(valueString);
	}
//-------------------------------------------------------------------------------------------------------------------------------------
protected void parseReader(SqliteDataReader reader, out List<WeaponFrameData> dataList)
	{
		dataList = new List<WeaponFrameData>();
		while (reader.Read())
		{
			WeaponFrameData data = new WeaponFrameData();
			data.mID = StringUtility.stringToInt(reader[COL_ID].ToString());
			data.mDirection = StringUtility.stringToInt(reader[COL_DIRECTION].ToString());
			data.mAction = reader[COL_ACTION].ToString();
			data.mFrameCount = StringUtility.stringToInt(reader[COL_FRAME_COUNT].ToString());
			StringUtility.stringToFloatArray(reader[COL_POSX].ToString(), ref data.mPosX);
			StringUtility.stringToFloatArray(reader[COL_POSY].ToString(), ref data.mPosY);
			dataList.Add(data);
		}
		reader.Close();
	}
};
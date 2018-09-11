using Mono.Data.Sqlite;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

public class WeaponFrameData
{
	public string mLabel;
	public int mID;
	public int mDirection;
	public string mAction;
	public int mFrameCount;
	public float[] mPosX;
	public float[] mPosY;
}

public class SQLiteWeaponFrame : SQLiteTable
{
	string COL_LABEL = "WeaponLabel";
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
		dataList = new List<WeaponFrameData>();
		string queryStr = "SELECT * FROM " + mTableName + " WHERE ";
		queryStr += COL_ID + " = " + StringUtility.intToString(weaponID);
		parseReader(mSQLite.query(queryStr), out dataList);
	}
	public void query(int weaponID, int direction, out List<WeaponFrameData> dataList)
	{
		dataList = new List<WeaponFrameData>();
		string queryStr = "SELECT * FROM " + mTableName + " WHERE ";
		queryStr += COL_ID + " = " + StringUtility.intToString(weaponID) + " and " +
					COL_DIRECTION + " = " + StringUtility.intToString(direction);
		parseReader(mSQLite.query(queryStr), out dataList);
	}
	public void query(int weaponID, int direction, string action, out List<WeaponFrameData> dataList)
	{
		string queryStr = "SELECT * FROM " + mTableName + " WHERE ";
		queryStr += COL_ID + " = " + StringUtility.intToString(weaponID) + " and " + 
					COL_DIRECTION + " = " + StringUtility.intToString(direction) + " and " + 
					COL_ACTION + " = " + "\"" + action + "\"";
		parseReader(mSQLite.query(queryStr), out dataList);
	}
	//-------------------------------------------------------------------------------------------------------------------------------------
	protected void parseReader(SqliteDataReader reader, out List<WeaponFrameData> dataList)
	{
		dataList = new List<WeaponFrameData>();
		while (reader.Read())
		{
			WeaponFrameData data = new WeaponFrameData();
			data.mLabel = reader[COL_LABEL].ToString();
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
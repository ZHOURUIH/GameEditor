using Mono.Data.Sqlite;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

public class ClothFrameData
{
	public string mLabel;
	public int mID;
	public int mDirection;
	public string mAction;
	public int mFrameCount;
	public float[] mPosX;
	public float[] mPosY;
}

public class SQLiteClothFrame : SQLiteTable
{
	string COL_LABEL = "ClothLabel";
	string COL_ID = "ClothID";
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
	public void query(int clothID, out List<ClothFrameData> dataList)
	{
		dataList = new List<ClothFrameData>();
		string queryStr = "SELECT * FROM " + mTableName + " WHERE ";
		queryStr += COL_ID + " = " + StringUtility.intToString(clothID);
		parseReader(mSQLite.query(queryStr), out dataList);
	}
	public void query(int clothID, int direction, out List<ClothFrameData> dataList)
	{
		dataList = new List<ClothFrameData>();
		string queryStr = "SELECT * FROM " + mTableName + " WHERE ";
		queryStr += COL_ID + " = " + StringUtility.intToString(clothID) + " and " + 
					COL_DIRECTION + " = " + StringUtility.intToString(direction);
		parseReader(mSQLite.query(queryStr), out dataList);
	}
	public void query(int clothID, int direction, string action, out List<ClothFrameData> dataList)
	{	
		string queryStr = "SELECT * FROM " + mTableName + " WHERE ";
		queryStr += COL_ID + " = " + StringUtility.intToString(clothID) + " and " +
					COL_DIRECTION + " = " + StringUtility.intToString(direction) + " and " + 
					COL_ACTION + " = " + "\"" + action + "\"";
		parseReader(mSQLite.query(queryStr), out dataList);
	}
	public void updateData(ClothFrameData data)
	{
		string posXStr = "";
		string posYStr = "";
		for (int i = 0; i < data.mFrameCount; ++i)
		{
			posXStr += data.mPosX[i];
			posYStr += data.mPosY[i];
			if (i != data.mFrameCount - 1)
			{
				posXStr += ", ";
				posYStr += ", ";
			}
		}
		string queryStr = "UPDATE " + mTableName + " SET " +
							COL_FRAME_COUNT + " = " + data.mFrameCount + ", " +
							COL_POSX + " = " + "\"" + posXStr + "\"" + ", " +
							COL_POSY + " = " + "\"" + posYStr + "\"" +
							" WHERE " + COL_ID + " = " + data.mID + " and " +
							COL_DIRECTION + " = " + data.mDirection + " and " +
							COL_ACTION + " = " + "\"" + data.mAction + "\"";
		mSQLite.update(queryStr);
	}
	//------------------------------------------------------------------------------------------------------------------------
	protected void parseReader(SqliteDataReader reader, out List<ClothFrameData> dataList)
	{
		dataList = new List<ClothFrameData>();
		while (reader.Read())
		{
			ClothFrameData data = new ClothFrameData();
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
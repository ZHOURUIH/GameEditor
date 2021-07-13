using Mono.Data.Sqlite;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

public class TDLog : TableData
{
	public static string UserID = "UserID";
	public static string LogType = "LogType";
	public static string Time = "Time";
	public static string LogInfo = "LogInfo";
	public static string GUID = "GUID";
	public static string uploaded = "uploaded";
	public string mUserID;
	public string mLogType;
	public string mTime;
	public string mLogInfo;
	public string mGUID;
	public int mUploaded;
	public override void parse(SqliteDataReader reader)
	{
		parseParam(reader, ref mUserID, UserID);
		parseParam(reader, ref mLogType, LogType);
		parseParam(reader, ref mTime, Time);
		parseParam(reader, ref mLogInfo, LogInfo);
		parseParam(reader, ref mGUID, GUID);
		parseParam(reader, ref mUploaded, uploaded);
	}
}

public class SQLiteLog : SQLiteTable
{
	public SQLiteLog()
		:base(typeof(TDLog)){}
	public void query(string guid, out TDLog data)
	{
		string condition = "";
		appendConditionString(ref condition, TDLog.GUID, guid, "");
		parseReader(doQuery(condition), out data);
	}
	public void queryAll(out List<TDLog> dataList)
	{
		parseReader(doQuery(), out dataList);
	}
	public void updateUploadedState(string guid, int uploaded)
	{
		string updateString = "";
		appendUpdateInt(ref updateString, TDLog.uploaded, uploaded);
		removeLastComma(ref updateString);
		string conditionStr = "";
		appendConditionString(ref conditionStr, TDLog.GUID, guid, "");
		doUpdate(updateString, conditionStr);
	}
}
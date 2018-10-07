using Mono.Data.Sqlite;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

public class SoundData
{
	public int mID;
	public string mFileName;
	public string mDescribe;
	public float mVolumeScale;
}

public class SQLiteSound : SQLiteTable
{
	public string COL_ID = "ID";
	public string COL_FILE_NAME = "FileName";
	public string COL_DESC = "Desc";
	public string COL_VOLUME_SCALE = "VolumeScale";
	public SQLiteSound()
		:base("Sound")
	{}
	public void query(int id, out SoundData data)
	{
		string condition = "";
		appendConditionInt(ref condition, COL_ID, id, "");
		parseReader(doQuery(condition), out data);
	}
	public void queryAll(out List<SoundData> dataList)
	{
		parseReader(doQuery(), out dataList);
	}
	public void insert(SoundData data)
	{
		string valueString = "";
		appendValueInt(ref valueString, data.mID);
		appendValueString(ref valueString, data.mFileName);
		appendValueString(ref valueString, data.mDescribe);
		appendValueFloat(ref valueString, data.mVolumeScale, true);
		doInsert(valueString);
	}
	//--------------------------------------------------------------------------------------------------------------------------
	protected void parseReader(SqliteDataReader reader, out SoundData data)
	{
		parseReaderOne(reader, out data);
		reader.Close();
	}
	protected void parseReader(SqliteDataReader reader, out List<SoundData> dataList)
	{
		dataList = new List<SoundData>();
		while (reader.Read())
		{
			SoundData data;
			parseReaderOne(reader, out data);
			dataList.Add(data);
		}
		reader.Close();
	}
	protected void parseReaderOne(SqliteDataReader reader, out SoundData data)
	{
		data = new SoundData();
		data.mID = StringUtility.stringToInt(reader[COL_ID].ToString());
		data.mFileName = reader[COL_FILE_NAME].ToString();
		data.mDescribe = reader[COL_DESC].ToString();
		data.mVolumeScale = StringUtility.stringToFloat(reader[COL_VOLUME_SCALE].ToString());
	}
};
using Mono.Data.Sqlite;
using System;
using System.Collections.Generic;

public class TDSceneMap : TableData
{
	public static string Label = "Label";
	public static string Resource = "Resource";
	public static string MiniMap = "MiniMap";
	public static string Width = "Width";
	public static string Height = "Height";
	public static string MonsterRegion = "MonsterRegion";
	public static string NPC = "NPC";
	public static string TransferPoint = "TransferPoint";
	public string mLabel;
	public string mResource;
	public int mMiniMap;
	public int mWidth;
	public int mHeight;
	public List<int> mMonsterRegion = new List<int>();
	public List<int> mNPC = new List<int>();
	public List<int> mTransferPoint = new List<int>();
	public override void parse(SqliteDataReader reader)
	{
		base.parse(reader);
		parseParam(reader, ref mLabel, Label);
		parseParam(reader, ref mResource, Resource);
		parseParam(reader, ref mMiniMap, MiniMap);
		parseParam(reader, ref mWidth, Width);
		parseParam(reader, ref mHeight, Height);
		parseParam(reader, ref mMonsterRegion, MonsterRegion);
		parseParam(reader, ref mNPC, NPC);
		parseParam(reader, ref mTransferPoint, TransferPoint);
	}
}

public class SQLiteSceneMap : SQLiteTable
{
	public SQLiteSceneMap()
		:base(typeof(TDSceneMap)){}
}
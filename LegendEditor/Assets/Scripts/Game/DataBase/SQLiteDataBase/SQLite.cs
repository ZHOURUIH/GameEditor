using UnityEngine;
using System.Collections;
using Mono.Data.Sqlite;
using System.Data;

public class SQLite : FrameComponent
{
	protected SqliteConnection mConnection;
	protected SqliteCommand mCommand;
	public SQLiteCloth mSQLiteCloth;
	public SQLiteClothFrame mSQLiteClothFrame;
	public SQLiteWeapon mSQLiteWeapon;
	public SQLiteWeaponFrame mSQLiteWeaponFrame;
	public SQLiteMonster mSQLiteMonster;
	public SQLiteMonsterFrame mSQLiteMonsterFrame;
	public SQLite(string name)
		:base(name)
	{
		string fullPath = CommonDefine.F_DATA_BASE_PATH + "MicroLegend.db";
		FileUtility.createDir(StringUtility.getFilePath(fullPath));
		mConnection = new SqliteConnection("DATA SOURCE = " + fullPath);   // 创建SQLite对象的同时，创建SqliteConnection对象  
		mConnection.Open();                         // 打开数据库链接  
		mSQLiteCloth = new SQLiteCloth(this);
		mSQLiteClothFrame = new SQLiteClothFrame(this);
		mSQLiteWeapon = new SQLiteWeapon(this);
		mSQLiteWeaponFrame = new SQLiteWeaponFrame(this);
		mSQLiteMonster = new SQLiteMonster(this);
		mSQLiteMonsterFrame = new SQLiteMonsterFrame(this);
}
	public override void init()
	{
		base.init();
		mCommand = mConnection.CreateCommand();
	}
	public override void destroy()
	{
		base.destroy();
		if(mConnection != null)
		{
			mConnection.Close();
			mConnection = null;
		}
		if(mCommand != null)
		{
			mCommand.Cancel();
			mCommand = null;
		}
	}
	public void createTable(string tableName, string format)
	{
		mCommand.CommandText = "CREATE TABLE IF NOT EXISTS " + tableName + "(" + format + ");";
		mCommand.ExecuteNonQuery();
	}
	public SqliteDataReader queryReader(string queryString)
	{
		mCommand.CommandText = queryString;
		return mCommand.ExecuteReader();
	}
	public void queryNonReader(string queryString)
	{
		mCommand.CommandText = queryString;
		mCommand.ExecuteNonQuery();
	}
}
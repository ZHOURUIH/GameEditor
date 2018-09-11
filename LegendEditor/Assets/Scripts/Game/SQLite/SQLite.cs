using UnityEngine;
using System.Collections;
using Mono.Data.Sqlite;
using System.Data;

public class SQLiteTable
{
	protected string mTableName;
	protected SQLite mSQLite;
	public SQLiteTable(string name, SQLite sqlite)
	{
		mTableName = name;
		mSQLite = sqlite;
	}
}

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
	public SqliteDataReader query(string queryString)
	{
		mCommand.CommandText = queryString;
		return mCommand.ExecuteReader();
	}
	// 增加数据
	public void insertData(string table_name, object[] values)
	{
		mCommand.CommandText = "INSERT INTO " + table_name + " VALUES (";
		int valueCount = values.Length;
		for (int i = 0; i < valueCount; ++i)
		{
			if (values[i].GetType() == typeof(string))
			{
				values[i] = "\'" + values[i] + "\'";
			}
			mCommand.CommandText += values[i].ToString();
			if (i < values.Length - 1)
			{
				mCommand.CommandText += ",";
			}
		}
		mCommand.CommandText += ")";
		mCommand.ExecuteNonQuery();
	}
	// 删除数据
	public void deleteData(string table_name, string[] conditions)
	{
		mCommand.CommandText = "DELETE FROM " + table_name + " WHERE " + conditions[0];
		for (int i = 1; i < conditions.Length; i++)
		{
			// or：表示或者  and：表示并且  
			mCommand.CommandText += " OR " + conditions[i];
		}
		mCommand.ExecuteNonQuery();
	}
	// 修改数据
	public void updateData(string table_name, string[] columns, object[] values, string[] conditions)
	{
		if (columns.Length != values.Length)
		{
			return;
		}
		mCommand.CommandText = "UPDATE " + table_name + " SET ";
		for (int i = 0; i < values.Length; ++i)
		{
			if (values[i].GetType() == typeof(string))
			{
				values[i] = "\'" + values[i] + "\'";
			}
			mCommand.CommandText += columns[i] + " = " + values[i];
			if (i < values.Length - 1)
			{
				mCommand.CommandText += ",";
			}
		}
		mCommand.CommandText += " WHERE ";
		for (int i = 0; i < conditions.Length; ++i)
		{
			mCommand.CommandText += conditions[i];
			if (i < conditions.Length - 1)
			{
				mCommand.CommandText += " OR ";
			}
		}
		mCommand.ExecuteNonQuery();
	}
	// 修改数据
	public void updateData(string table_name, object[] values, string[] conditions)
	{
		mCommand.CommandText = "UPDATE " + table_name + " SET " + values[0];
		for (int i = 1; i < values.Length; i++)
		{
			mCommand.CommandText += "," + values[i];
		}
		mCommand.CommandText += " WHERE " + conditions[0];
		for (int i = 1; i < conditions.Length; i++)
		{
			mCommand.CommandText += " OR " + conditions[i];
		}
		mCommand.ExecuteNonQuery();
	}
	// 查询数据
	public SqliteDataReader selectData(string table_name, string[] fields)
	{
		mCommand.CommandText = "SELECT " + fields[0];
		for (int i = 1; i < fields.Length; i++)
		{
			mCommand.CommandText += "," + fields[i];
		}
		mCommand.CommandText += " FROM " + table_name;
		return mCommand.ExecuteReader();
	}
	// 查询整张表的数据
	public SqliteDataReader selectFullTableData(string table_name)
	{
		mCommand.CommandText = "SELECT * FROM " + table_name;
		return mCommand.ExecuteReader();
	}
}
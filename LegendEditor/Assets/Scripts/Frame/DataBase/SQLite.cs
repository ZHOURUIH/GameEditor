using UnityEngine;
using System.Collections;
using Mono.Data.Sqlite;
using System.Data;
using System.Collections.Generic;
using System;

public class SQLite : FrameComponent
{
	protected SqliteConnection mConnection;
	protected SqliteCommand mCommand;
	protected Dictionary<Type, SQLiteTable> mTableList;
	public SQLite(string name)
		:base(name)
	{
		mTableList = new Dictionary<Type, SQLiteTable>();
		string fullPath = CommonDefine.F_DATA_BASE_PATH + CommonDefine.DATA_BASE_FILE_NAME;
		FileUtility.createDir(StringUtility.getFilePath(fullPath));
		mConnection = new SqliteConnection("DATA SOURCE = " + fullPath);   // 创建SQLite对象的同时，创建SqliteConnection对象  
		mConnection.Open();                         // 打开数据库链接
		registeTable<SQLiteSound>();
	}
	public T registeTable<T>() where T : SQLiteTable, new()
	{
		T table = new T();
		table.init(this);
		mTableList.Add(typeof(T), table);
		return table;
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
	public T getTable<T>()where T : SQLiteTable
	{
		if(mTableList.ContainsKey(typeof(T)))
		{
			return mTableList[typeof(T)] as T;
		}
		return null;
	}
}
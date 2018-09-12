using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

public class DCMonsterData
{
	public string mName;
	public int mTypeID;
	public int mInstanceID;
	public int mLevel;
	public int mHP;
	public int mMaxHP;
	public float mPosX;
	public float mPosY;
	public int mDirection;
}

// 从服务器传过来的怪物信息
// 不能在本地修改数据
public class DCMonsterInfo : GameBase
{
	public Dictionary<int, DCMonsterData> mMonsterDataList;	// 以怪物实例GUID为索引的列表
	public DCMonsterInfo()
	{
		mMonsterDataList = new Dictionary<int, DCMonsterData>();
	}
	public void addData(DCMonsterData data)
	{
		if(mMonsterDataList.ContainsKey(data.mInstanceID))
		{
			mMonsterDataList[data.mInstanceID] = data;
		}
		else
		{
			mMonsterDataList.Add(data.mInstanceID, data);
		}
	}
	public DCMonsterData getData(int monsterID)
	{
		if(mMonsterDataList.ContainsKey(monsterID))
		{
			return mMonsterDataList[monsterID];
		}
		return null;
	}
	public Dictionary<int, DCMonsterData> getAllData() { return mMonsterDataList; }
}
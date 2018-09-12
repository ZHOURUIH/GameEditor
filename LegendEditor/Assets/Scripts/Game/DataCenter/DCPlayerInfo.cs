using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

// DC前缀表示DataCenter
public class DCMyselfAccount
{
	public int mAccountGUID;
	public int[] mCharGUID;
	public int mCurCharGUID;
	public DCMyselfAccount()
	{
		mCharGUID = new int[2];
	}
}

public class DCCharData
{
	public string mName;
	public int mAccountGUID;
	public int mCharGUID;
	public int mLevel;
	public int mOccupation;
	public int mMoney;
	public short mCloth;
	public short mWeapon;
	public float mSpeed;
}

// 从服务器传过来的角色信息,包含所有玩家的数据,仅做数据存储
// 不能在本地修改数据
public class DCPlayerInfo : GameBase
{
	public DCMyselfAccount mMyselfAccount;
	public Dictionary<int, DCCharData> mCharDataList;	// 以角色GUID为索引的列表
	public DCPlayerInfo()
	{
		mMyselfAccount = new DCMyselfAccount();
		mCharDataList = new Dictionary<int, DCCharData>();
	}
	public void addData(DCCharData data)
	{
		if(mCharDataList.ContainsKey(data.mCharGUID))
		{
			mCharDataList[data.mCharGUID] = data;
		}
		else
		{
			mCharDataList.Add(data.mCharGUID, data);
		}
	}
	public DCCharData getData(int charID)
	{
		if(mCharDataList.ContainsKey(charID))
		{
			return mCharDataList[charID];
		}
		return null;
	}
}
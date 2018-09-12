using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using UnityEngine;

public class ScriptScene : LayoutScript
{
	protected Dictionary<int, HumanAvatar> mHumanList;
	protected Dictionary<int, MonsterAvatar> mMonsterList;
	protected txNGUIText mInfo;
	protected int mCurID;
	protected int mCurDirection;
	protected string[] mAnimation;
	protected int mAnimIndex;
	public ScriptScene(string name, GameLayout layout)
		:
		base(name, layout)
	{
		mHumanList = new Dictionary<int, HumanAvatar>();
		mMonsterList = new Dictionary<int, MonsterAvatar>();
		mAnimation = new string[] { "attack", "death", "hit", "run", "skill", "stand" };
	}
	public override void assignWindow()
	{
		newObject(out mInfo, "Info");
	}
	public override void init()
	{
		;
	}
	public override void onReset()
	{
		;
	}
	public override void onShow(bool immediately, string param)
	{
		;
	}
	public override void onHide(bool immediately, string param)
	{
		;
	}
	public override void update(float elapsedTime)
	{
		// 切换方向
		bool changed = false;
		if(mInputManager.getKeyCurrentDown(KeyCode.Q))
		{
			mCurDirection = (++mCurDirection) % GameDefine.DIRECTION_COUNT;
			changed = true;
		}
		if (mInputManager.getKeyCurrentDown(KeyCode.W))
		{
			mAnimIndex = (++mAnimIndex) % mAnimation.Length;
			changed = true;
		}
		if(changed)
		{
			mInfo.setLabel("当前动作:dir" + mCurDirection + "_" + mAnimation[mAnimIndex]);
			List<ClothFrameData> frameData = null;
			mSQLite.mSQLiteClothFrame.query(1, mCurDirection, mAnimation[mAnimIndex], out frameData);
			int frameCount = frameData[0].mFrameCount;
			if(mCurID < frameCount)
			{
				for(int i = 0; i < frameCount - mCurID; ++i)
				{
					createHumanAvatar(mCurID + i);
				}
			}
			else if(mCurID > frameCount)
			{
				for (int i = 0; i < mCurID - frameCount; ++i)
				{
					destroyHumanAvatar(mCurID - i - 1);
				}
			}
			mCurID = frameCount;
		}
		// A键添加预设
		if (mInputManager.getKeyCurrentDown(KeyCode.A))
		{
			createHumanAvatar(mCurID++);
		}
		// D键删除预设
		else if (mInputManager.getKeyCurrentDown(KeyCode.D))
		{
			if(mCurID > 0)
			{
				destroyHumanAvatar(--mCurID);
			}
		}
	}
	public HumanAvatar createHumanAvatar(int charGUID)
	{
		string avatarName = "HumanAvatar" + charGUID;
		instantiateObject(mRoot, "HumanAvatar", avatarName);
		HumanAvatar avatar = new HumanAvatar(this);
		avatar.assignWindow(avatarName);
		avatar.init(charGUID);
		mHumanList.Add(charGUID, avatar);
		return avatar;
	}
	public void destroyHumanAvatar(int charGUID)
	{
		HumanAvatar avatar = mHumanList[charGUID];
		mHumanList.Remove(charGUID);
		destroyObject(avatar.mHumanRoot);
	}
	public MonsterAvatar createMonsterAvatar(int instanceID, int typeID, string prefabName, string label)
	{
		string avatarName = label + instanceID;
		instantiateObject(mRoot, prefabName, avatarName);
		MonsterAvatar avatar = new MonsterAvatar(this);
		avatar.assignWindow(avatarName);
		avatar.init(instanceID, typeID, label);
		mMonsterList.Add(instanceID, avatar);
		return avatar;
	}
	public void destroyMonsterAvatar(int instanceID)
	{
		MonsterAvatar avatar = mMonsterList[instanceID];
		mMonsterList.Remove(instanceID);
		destroyObject(avatar.mMonsterRoot);
	}
	public MonsterAvatar getMonster(int instanceID)
	{
		if(mMonsterList.ContainsKey(instanceID))
		{
			return mMonsterList[instanceID];
		}
		return null;
	}
}
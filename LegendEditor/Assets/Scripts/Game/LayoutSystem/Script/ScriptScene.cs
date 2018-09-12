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
	protected int mClothID;
	protected int mOccupation;
	protected int mWeaponID;
	public ScriptScene(string name, GameLayout layout)
		:
		base(name, layout)
	{
		mHumanList = new Dictionary<int, HumanAvatar>();
		mMonsterList = new Dictionary<int, MonsterAvatar>();
		mAnimation = new string[] { "attack", "death", "hit", "run", "skill", "stand" };
		mClothID = 1;
		mOccupation = 3;
		mWeaponID = 1;
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
		else if (mInputManager.getKeyCurrentDown(KeyCode.W))
		{
			mAnimIndex = (++mAnimIndex) % mAnimation.Length;
			changed = true;
		}
		// 按S键保存
		else if (mInputManager.getKeyCurrentDown(KeyCode.S))
		{
			WeaponFrameData weaponData = new WeaponFrameData();
			weaponData.mID = mWeaponID;
			weaponData.mDirection = mCurDirection;
			weaponData.mAction = mAnimation[mAnimIndex];
			weaponData.mFrameCount = mCurID;
			weaponData.mPosX = new float[mCurID];
			weaponData.mPosY = new float[mCurID];
			for (int i = 0; i < mCurID; ++i)
			{
				weaponData.mPosX[i] = mHumanList[i].mWeapon.getPosition().x;
				weaponData.mPosY[i] = mHumanList[i].mWeapon.getPosition().y;
			}
			mSQLite.mSQLiteWeaponFrame.updateData(weaponData);
		}
		if (changed)
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
			// 设置每一帧的图片
			for(int i = 0; i < frameCount; ++i)
			{
				// 人物
				string humanAnimName = "human_occup" + mOccupation + "_dir" + mCurDirection + "_cloth" + mClothID + "_" + mAnimation[mAnimIndex];
				mHumanList[i].mHuman.setTextureSet(humanAnimName);
				mHumanList[i].mHuman.mControl.setCurFrameIndex(i);
				Texture curHumanTexture = mHumanList[i].mHuman.getTexture();
				mHumanList[i].mHuman.setWindowSize(new Vector2(curHumanTexture.width, curHumanTexture.height));
				mHumanList[i].mHuman.setLocalPosition(new Vector3(frameData[0].mPosX[i], frameData[0].mPosY[i], 0.0f));
				// 武器
				string weaponAnimName = "weapon_id" + mWeaponID + "_dir" + mCurDirection + "_" + mAnimation[mAnimIndex];
				mHumanList[i].mWeapon.setTextureSet(weaponAnimName);
				mHumanList[i].mWeapon.mControl.setCurFrameIndex(i);
				Texture curWeaponTexture = mHumanList[i].mWeapon.getTexture();
				mHumanList[i].mWeapon.setWindowSize(new Vector2(curWeaponTexture.width, curWeaponTexture.height));
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
using UnityEngine;
using System.Collections;
using System.Collections.Generic;
using System;

public enum HUMAN_ACTION
{
	HA_STAND,
	HA_WALK,
	HA_RUN,
	HA_ATTACK,
	HA_DIG,
	HA_JUMP_ATTACK,
	HA_SKILL,
	HA_SEARCH,
	HA_HIT,
	HA_DIE,
}

public class HumanAnimInfo
{
	public bool mLoop;
	public int mDirection;
	public string mName;
	public string mHumanAnim;
	public string mWeaponAnim;
	public int mHumanFrameCount;
	public int mWeaponFrameCount;
	public List<Vector2> mWeaponPos;
	public List<Vector2> mHumanPos;
}

public class HumanAnimation : GameComponent
{
	protected CharacterOther mPlayer;
	protected CharacterData mData;
	protected HumanAvatar mAvatar;
	protected Dictionary<HUMAN_ACTION, List<HumanAnimInfo>> mAnimationList;   // 第一个Key是行为类型,第二个List的下标是Direction
	public HumanAnimation(Type type, string name)
		:base(type, name)
	{
		mAnimationList = new Dictionary<HUMAN_ACTION, List<HumanAnimInfo>>();
	}
	public override void init(ComponentOwner owner)
	{
		base.init(owner);
		mPlayer = owner as CharacterOther;
		mData = mPlayer.getCharacterData();
		mAvatar = mPlayer.getAvatar();
	}
	public int getFrameCount(HUMAN_ACTION action)
	{
		return mAnimationList[action][0].mHumanFrameCount;
	}
	public void playAnimation(HUMAN_ACTION action, float animSpeed = GameDefine.NORMAL_ANIM_SPEED)
	{
		playAnimation(mAnimationList[action][mData.mDirection], animSpeed);
	}
	// 播放当前动作
	public void playAnimation(HumanAnimInfo animInfo, float animSpeed = GameDefine.NORMAL_ANIM_SPEED)
	{
		mAvatar.play(animInfo, null, null, animInfo.mLoop, animSpeed);
	}
	// 武器或外观改变时需要重新注册
	public void registeAllAnimation()
	{
		// 根据当前外观和武器,设置资源图集
		string clothResource = CommonDefine.R_ATLAS_TEXTURE_ANIM_PATH + "Human/" + GameUtility.getClothResource(mData.mClothID);
		string weaponResource = CommonDefine.R_ATLAS_TEXTURE_ANIM_PATH + "Weapon/" + GameUtility.getWeaponResource(mData.mWeaponID);
		mAvatar.getHuman().setAtlas(mResourceManager.loadResource<UIAtlas>(clothResource, true));
		mAvatar.getWeapon().setAtlas(mResourceManager.loadResource<UIAtlas>(weaponResource, true));
		registeAnimation(HUMAN_ACTION.HA_STAND, "stand", true);
		registeAnimation(HUMAN_ACTION.HA_WALK, "walk", true);
		registeAnimation(HUMAN_ACTION.HA_RUN, "run", true);
		registeAnimation(HUMAN_ACTION.HA_ATTACK, "attack");
		registeAnimation(HUMAN_ACTION.HA_DIG, "dig", true);
		registeAnimation(HUMAN_ACTION.HA_JUMP_ATTACK, "jumpAttack");
		registeAnimation(HUMAN_ACTION.HA_SKILL, "skill");
		registeAnimation(HUMAN_ACTION.HA_SEARCH, "search", true);
		registeAnimation(HUMAN_ACTION.HA_HIT, "hit");
		registeAnimation(HUMAN_ACTION.HA_DIE, "die");
	}
	//---------------------------------------------------------------------------------------------------------------------------------------------
	protected override bool isType(Type type) { return type == typeof(HumanAnimation); }
	protected override void setBaseType()
	{
		mBaseType = typeof(HumanAnimation);
	}
	protected void registeAnimation(HUMAN_ACTION action, string name, bool loop = false)
	{
		if (mAnimationList.ContainsKey(action))
		{
			mAnimationList[action].Clear();
		}
		else
		{
			mAnimationList.Add(action, new List<HumanAnimInfo>());
		}
		for (int i = 0; i < GameDefine.DIRECTION_COUNT; ++i)
		{
			// 根据当前角色数据从数据库查询本地数据
			List<WeaponFrameData> weaponDataList;
			mSQLite.mSQLiteWeaponFrame.query(mData.mWeaponID, i, name, out weaponDataList);
			if (weaponDataList.Count != 1)
			{
				logError("weapon info error!");
				return;
			}
			List<ClothFrameData> clothDataList;
			mSQLite.mSQLiteClothFrame.query(mData.mClothID, i, name, out clothDataList);
			if (clothDataList.Count != 1)
			{
				logError("cloth info error!");
				return;
			}
			List<Vector2> weaponPosList = new List<Vector2>();
			for (int j = 0; j < weaponDataList[0].mFrameCount; ++j)
			{
				weaponPosList.Add(new Vector2(weaponDataList[0].mPosX[j], weaponDataList[0].mPosY[j]));
			}
			List<Vector2> humanPosList = new List<Vector2>();
			for (int j = 0; j < clothDataList[0].mFrameCount; ++j)
			{
				humanPosList.Add(new Vector2(clothDataList[0].mPosX[j], clothDataList[0].mPosY[j]));
			}
			HumanAnimInfo info = new HumanAnimInfo();
			info.mDirection = i;
			info.mLoop = loop;
			info.mName = name;
			info.mHumanAnim = name + "_dir" + i;
			info.mWeaponAnim = name + "_dir" + i;
			info.mHumanFrameCount = clothDataList[0].mFrameCount;
			info.mWeaponFrameCount = weaponDataList[0].mFrameCount;
			info.mWeaponPos = weaponPosList;
			info.mHumanPos = humanPosList;
			mAnimationList[action].Add(info);
		}
	}
}
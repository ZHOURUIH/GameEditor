//using UnityEngine;
//using System.Collections;
//using System;
//using System.Collections.Generic;

//public class HumanAnimInfo
//{
//	public bool mLoop;
//	public int mDirection;
//	public string mName;
//	public List<string> mHumanFrame;
//	public List<string> mWeaponFrame;
//	public List<Vector2> mWeaponPos;
//	public List<Vector2> mHumanPos;
//}

//public class CharacterControlMyself : CharacterControl
//{
//	CharacterMyself mMyself;
//	CharacterData mData;
//	ATTACK_STATE mAttackState;
//	int mAttackableCount;       // 一次攻击中击中怪物的剩余次数
//	HUMAN_ACTION mCurAction;
//	bool mRunning;
//	CharacterAvatar mAvatar;
//	Dictionary<HUMAN_ACTION, List<HumanAnimInfo>> mAnimationList;	// 第一个Key是行为类型,第二个List的下标是Direction
//	public CharacterControlMyself(Type type, string name)
//		:
//		base(type, name)
//	{
//		mAnimationList = new Dictionary<HUMAN_ACTION, List<HumanAnimInfo>>();
//		mAttackState = ATTACK_STATE.AS_DONE;
//	}
//	public override void init(ComponentOwner owner)
//	{
//		base.init(owner);
//		mMyself = owner as CharacterMyself;
//		mData = mMyself.getCharacterData();
//		mAvatar = mMyself.getFirstActiveComponent<CharacterAvatar>();
//		mAttackableCount = 1;
//		// 当武器和衣服改变时,需要重新注册动作
//		registeAllAnimation();
//	}
//	public override void destroy()
//	{
//		base.destroy();
//	}
//	public override void update(float elapsedTime)
//	{
//		base.update(elapsedTime);
//		keyProcess(elapsedTime);
//	}
//	public override void setActive(bool active)
//	{
//		base.setActive(active);
//		if(active)
//		{
//			stand();
//		}
//	}
//	public void setDirection(int direction)
//	{
//		mData.mDirection = direction;
//		// 方向改变,需要刷新状态,武器和衣服改变时同样需要刷新状态
//		updateState();
//	}
//	// 普通攻击
//	public void attack()
//	{
//		if(mAttackState != ATTACK_STATE.AS_DONE)
//		{
//			return;
//		}
//		setAttackState(ATTACK_STATE.AS_ATTACKING);
//		setAction(HUMAN_ACTION.HA_ATTACK);
//	}
//	public void stand()
//	{
//		setAction(HUMAN_ACTION.HA_STAND);
//	}
//	public void hit()
//	{
//		setAction(HUMAN_ACTION.HA_HIT);
//	}
//	public void death()
//	{
//		setAction(HUMAN_ACTION.HA_DEATH);
//	}
//	public void skill()
//	{
//		setAction(HUMAN_ACTION.HA_SKILL);
//	}
//	public void run(Vector2 dir, float elapsedTime)
//	{
//		setAction(HUMAN_ACTION.HA_RUN);
//		Vector2 delta = dir * mData.mSpeed * elapsedTime;
//		mMyself.move(delta);
//	}
//	public void notifyAttacked()
//	{
//		--mAttackableCount;
//	}
//	public bool canAttack()
//	{
//		return mAttackableCount > 0;
//	}
//	//-----------------------------------------------------------------------------------------------------------------------
//	protected override bool isType(Type type) { return type == typeof(CharacterControlMyself); }
//	// 当方向或者衣服,武器改变时,需要刷新状态
//	protected void updateState()
//	{
//		HumanAnimInfo info = mAnimationList[mCurAction][mData.mDirection];
//		mAvatar.mHumanAvatar.play(info, onActionEnd, onActioning, this, info.mLoop);
//	}
//	protected List<string> getHumanTextureList(string action, int frameCount)
//	{
//		string textureName = "human_occup" + mData.mOccupation + "_dir" + mData.mDirection + "_cloth" + mData.mClothID + "_" + action;
//		List<string> humanList = new List<string>();
//		for (int i = 0; i < frameCount; ++i)
//		{
//			humanList.Add(textureName + "_" + i);
//		}
//		return humanList;
//	}
//	protected List<string> getWeaponTextureList(string action, int frameCount)
//	{
//		string textureName = "weapon_id" + mData.mWeaponID + "_dir" + mData.mDirection + "_" + action;
//		List<string> weaponList = new List<string>();
//		for (int i = 0; i < frameCount; ++i)
//		{
//			weaponList.Add(textureName + "_" + i);
//		}
//		return weaponList;
//	}
//	protected void setAction(HUMAN_ACTION action)
//	{
//		mCurAction = action;
//		// 当前行为改变,刷新状态
//		updateState();
//	}
//	protected void onActionEnd(bool isBreak)
//	{
//		// 如果是站立序列帧结束,则在播放站立动作
//		if(mCurAction == HUMAN_ACTION.HA_STAND)
//		{
//			setAction(mCurAction);
//		}
//		else if(mCurAction == HUMAN_ACTION.HA_ATTACK)
//		{
//			setAttackState(ATTACK_STATE.AS_DONE);
//		}
//		// 当前在跑动过程中,继续播放跑动动作
//		else if(mRunning)
//		{
//			setAction(HUMAN_ACTION.HA_RUN);
//		}
//	}
//	protected void onActioning(int frame)
//	{
//		if(mCurAction == HUMAN_ACTION.HA_ATTACK)
//		{
//			// 第3到第4帧时可以攻击怪物
//			mAvatar.mHumanAvatar.enableWeaponBoxCollider(frame >= 3 && frame <= 4);
//		}
//	}
//	protected void setAttackState(ATTACK_STATE state)
//	{
//		mAttackState = state;
//		if(mAttackState == ATTACK_STATE.AS_DONE)
//		{
//			mAttackState = ATTACK_STATE.AS_DONE;
//			mAttackableCount = 1;
//		}
//	}
//	protected void keyProcess(float elapsedTime)
//	{
//		// 处理移动相关
//		processDirection(elapsedTime);
//		// 处理攻击相关
//		processAttack(elapsedTime);
//	}
//	protected void processDirection(float elapsedTime)
//	{
//		Vector2 moveDir = Vector2.zero;
//		int direction = 0;
//		if (getKeyDown(KeyCode.UpArrow))
//		{
//			moveDir += Vector2.up;
//		}
//		else if (getKeyDown(KeyCode.DownArrow))
//		{
//			moveDir += Vector2.down;
//		}
//		if (getKeyDown(KeyCode.LeftArrow))
//		{
//			moveDir += Vector2.left;
//		}
//		else if (getKeyDown(KeyCode.RightArrow))
//		{
//			moveDir += Vector2.right;
//		}
//		mRunning = !isVectorZero(moveDir);
//		if (mRunning)
//		{
//			moveDir = normalize(moveDir);
//			float angle = getAngleFromVectorToVector(moveDir, Vector2.up);
//			float angleBetweenDirection = 2 * Mathf.PI / GameDefine.DIRECTION_COUNT;
//			direction = (int)((angle + angleBetweenDirection) / angleBetweenDirection + 0.5f);
//			setDirection(direction);
//			// 通过整数的方向,重新计算方向向量,避免当输入方式修改为移动轴时会出现的错误
//			moveDir = getVectorFromAngle(angleBetweenDirection * direction);
//			run(moveDir, elapsedTime);
//		}
//	}
//	protected void processAttack(float elapsedTime)
//	{
//		if(getKeyCurrentDown(KeyCode.I))
//		{
//			attack();
//		}
//	}
//	protected void registeAllAnimation()
//	{
//		registeAnimation(HUMAN_ACTION.HA_ATTACK, false, "attack");
//		registeAnimation(HUMAN_ACTION.HA_DEATH, false, "death");
//		registeAnimation(HUMAN_ACTION.HA_HIT, false, "hit");
//		registeAnimation(HUMAN_ACTION.HA_RUN, true, "run");
//		registeAnimation(HUMAN_ACTION.HA_SKILL, false, "skill");
//		registeAnimation(HUMAN_ACTION.HA_STAND, true, "stand");
//	}

//	protected void registeAnimation(HUMAN_ACTION action, bool loop, string name)
//	{
//		mAnimationList.Clear();
//		for (int i = 0; i < GameDefine.DIRECTION_COUNT; ++i)
//		{
//			// 根据当前角色数据从数据库查询本地数据
//			List<WeaponFrameData> weaponDataList;
//			mSQLite.mSQLiteWeaponFrame.query(mData.mWeaponID, i, name, out weaponDataList);
//			if(weaponDataList.Count != 1)
//			{
//				logError("weapon info error!");
//				return;
//			}
//			List<ClothFrameData> clothDataList;
//			mSQLite.mSQLiteClothFrame.query(mData.mClothID, i, name, out clothDataList);
//			if (clothDataList.Count != 1)
//			{
//				logError("cloth info error!");
//				return;
//			}
//			List<Vector2> weaponPosList = new List<Vector2>();
//			for (int j = 0; j < weaponDataList[0].mFrameCount; ++j)
//			{
//				weaponPosList.Add(new Vector2(weaponDataList[0].mPosX[j], weaponDataList[0].mPosY[j]));
//			}
//			List<Vector2> humanPosList = new List<Vector2>();
//			for (int j = 0; j < weaponDataList[0].mFrameCount; ++j)
//			{
//				humanPosList.Add(new Vector2(clothDataList[0].mPosX[j], clothDataList[0].mPosY[j]));
//			}
//			HumanAnimInfo info = new HumanAnimInfo();
//			info.mDirection = i;
//			info.mLoop = loop;
//			info.mName = name;
//			info.mHumanFrame = getHumanTextureList(name, clothDataList[0].mFrameCount);
//			info.mWeaponFrame = getWeaponTextureList(name, weaponDataList[0].mFrameCount);
//			info.mWeaponPos = weaponPosList;
//			info.mHumanPos = humanPosList;
//			if (!mAnimationList.ContainsKey(action))
//			{
//				mAnimationList.Add(action, new List<HumanAnimInfo>());
//			}
//			mAnimationList[action].Add(info);
//		}
//	}
//}

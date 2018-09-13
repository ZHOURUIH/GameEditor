using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using UnityEngine;

public enum EDIT_TYPE
{
	ET_WEAPON,
	ET_HUMAN,
	ET_MONSTER,
}

public class ScriptScene : LayoutScript
{
	protected Dictionary<int, HumanAvatar> mHumanList;
	protected Dictionary<int, MonsterAvatar> mMonsterList;
	protected txUIObject mEditorRoot;
	protected txNGUIText mTypeInfo;
	protected txNGUIText mInfo;
	protected txNGUIText mFrameCountLabel;
	protected txNGUIButton mWeaponEditor;
	protected txNGUIButton mHumanEditor;
	protected txNGUIButton mMonsterEditor;
	protected txNGUIButton mSave;
	protected txNGUIButton mLastID;
	protected txNGUIEditbox mCurIDEdit;
	protected txNGUIButton mNextID;
	protected txNGUIButton mLastOccupation;
	protected txNGUIEditbox mCurOccupationEdit;
	protected txNGUIButton mNextOccupation;
	protected txNGUIButton mLastDirection;
	protected txNGUIEditbox mCurDirectionEdit;
	protected txNGUIButton mNextDirection;
	protected txNGUIButton mLastAction;
	protected txNGUIEditbox mCurActionEdit;
	protected txNGUIButton mNextAction;
	protected txNGUIButton mLastHumanID;
	protected txNGUIEditbox mCurHumanIDEdit;
	protected txNGUIButton mNextHumanID;
	protected EDIT_TYPE mEditType;
	protected int mID;
	protected int mDirection;
	protected string[] mHumanAnimation;
	protected string[] mMonsterAnimation;
	protected int mAnimIndex;
	protected string mAction;
	protected int mHumanID;
	protected int mOccupation;
	protected int mFrameCount;
	public ScriptScene(string name, GameLayout layout)
		:
		base(name, layout)
	{
		mHumanList = new Dictionary<int, HumanAvatar>();
		mMonsterList = new Dictionary<int, MonsterAvatar>();
		mHumanAnimation = new string[] { "attack", "death", "hit", "run", "skill", "stand" };
		mMonsterAnimation = new string[] { "attack", "death", "hit", "stand" };
	}
	public override void assignWindow()
	{
		newObject(out mEditorRoot, "EditorRoot");
		newObject(out mTypeInfo, mEditorRoot, "TypeInfo");
		newObject(out mInfo, mEditorRoot, "Info");
		newObject(out mFrameCountLabel, mEditorRoot, "FrameCountLabel");
		newObject(out mWeaponEditor, mEditorRoot, "WeaponEditor");
		newObject(out mHumanEditor, mEditorRoot, "HumanEditor");
		newObject(out mMonsterEditor, mEditorRoot, "MonsterEditor");
		newObject(out mSave, mEditorRoot, "Save");
		newObject(out mLastID, mEditorRoot, "LastID");
		newObject(out mCurIDEdit, mEditorRoot, "CurIDEdit");
		newObject(out mNextID, mEditorRoot, "NextID");
		newObject(out mLastOccupation, mEditorRoot, "LastOccupation");
		newObject(out mCurOccupationEdit, mEditorRoot, "CurOccupationEdit");
		newObject(out mNextOccupation, mEditorRoot, "NextOccupation");
		newObject(out mLastDirection, mEditorRoot, "LastDirection");
		newObject(out mCurDirectionEdit, mEditorRoot, "CurDirectionEdit");
		newObject(out mNextDirection, mEditorRoot, "NextDirection");
		newObject(out mLastAction, mEditorRoot, "LastAction");
		newObject(out mCurActionEdit, mEditorRoot, "CurActionEdit");
		newObject(out mNextAction, mEditorRoot, "NextAction");
		newObject(out mLastHumanID, mEditorRoot, "LastHumanID");
		newObject(out mCurHumanIDEdit, mEditorRoot, "CurHumanIDEdit");
		newObject(out mNextHumanID, mEditorRoot, "NextHumanID");
	}
	public override void init()
	{
		mAnimIndex = 0;
		mHumanID = 1;
		setFrameCount(0);
		setEditType(EDIT_TYPE.ET_HUMAN, false);
		setID(1, false);
		setOccupation(3, false);
		setAction(mHumanAnimation[0], false);
		setDirection(2, true);
		registeBoxColliderNGUI(mWeaponEditor, onWeaponClick);
		registeBoxColliderNGUI(mHumanEditor, onHumanClick);
		registeBoxColliderNGUI(mMonsterEditor, onMonsterClick);
		registeBoxColliderNGUI(mSave, onSaveClick);
		registeBoxColliderNGUI(mLastID, onLastID);
		registeBoxColliderNGUI(mNextID, onNextID);
		registeBoxColliderNGUI(mLastOccupation, onLastOccupation);
		registeBoxColliderNGUI(mNextOccupation, onNextOccupation);
		registeBoxColliderNGUI(mLastDirection, onLastDirection);
		registeBoxColliderNGUI(mNextDirection, onNextDirection);
		registeBoxColliderNGUI(mLastAction, onLastAction);
		registeBoxColliderNGUI(mNextAction, onNextAction);
		registeBoxColliderNGUI(mLastHumanID, onLastHumanID);
		registeBoxColliderNGUI(mNextHumanID, onNextHumanID);
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
		if(mInputManager.getKeyCurrentDown(KeyCode.Q))
		{
			;
		}
		else if (mInputManager.getKeyCurrentDown(KeyCode.W))
		{
			;
		}
		// 按S键保存
		else if (mInputManager.getKeyCurrentDown(KeyCode.S))
		{
			
		}
	}
	// 物体相关
	//---------------------------------------------------------------------------------------------------------------------------
	// 刷新当前编辑的物体
	public void updateEdit()
	{
		if (mEditType == EDIT_TYPE.ET_HUMAN)
		{
			string humanAnimName = "human_occup" + mOccupation + "_dir" + mDirection + "_cloth" + mID + "_" + mAction;
			mInfo.setLabel(humanAnimName);
			List<ClothFrameData> frameData = null;
			mSQLite.mSQLiteClothFrame.query(mID, mDirection, mAction, out frameData);
			if (frameData.Count == 0)
			{
				logError("frame count is 0");
				return;
			}
			int frameCount = frameData[0].mFrameCount;
			int curFrameCount = mHumanList.Count;
			if (curFrameCount < frameCount)
			{
				for (int i = 0; i < frameCount - curFrameCount; ++i)
				{
					createHumanAvatar(curFrameCount + i);
				}
			}
			else if (curFrameCount > frameCount)
			{
				for (int i = 0; i < curFrameCount - frameCount; ++i)
				{
					destroyHumanAvatar(curFrameCount - i - 1);
				}
			}
			setFrameCount(frameCount);
			// 设置每一帧的图片
			for (int i = 0; i < frameCount; ++i)
			{
				// 人物
				mHumanList[i].mHuman.setTextureSet(humanAnimName);
				mHumanList[i].mHuman.mControl.setCurFrameIndex(i);
				Texture curHumanTexture = mHumanList[i].mHuman.getTexture();
				mHumanList[i].mHuman.setWindowSize(new Vector2(curHumanTexture.width, curHumanTexture.height));
			}
		}
		else if (mEditType == EDIT_TYPE.ET_WEAPON)
		{
			string weaponAnimName = "weapon_id" + mID + "_dir" + mDirection + "_" + mAction;
			string humanAnimName = "human_occup" + mOccupation + "_dir" + mDirection + "_cloth" + mHumanID + "_" + mAction;
			List<ClothFrameData> frameData = null;
			mSQLite.mSQLiteClothFrame.query(mHumanID, mDirection, mAction, out frameData);
			if (frameData.Count == 0)
			{
				logError("frame count is 0");
				return;
			}
			int frameCount = frameData[0].mFrameCount;
			int curFrameCount = mHumanList.Count;
			if (curFrameCount < frameCount)
			{
				for (int i = 0; i < frameCount - curFrameCount; ++i)
				{
					createHumanAvatar(curFrameCount + i);
				}
			}
			else if (curFrameCount > frameCount)
			{
				for (int i = 0; i < curFrameCount - frameCount; ++i)
				{
					destroyHumanAvatar(curFrameCount - i - 1);
				}
			}
			setFrameCount(frameCount);
			// 设置每一帧的图片
			for (int i = 0; i < frameCount; ++i)
			{
				// 人物
				mHumanList[i].mHuman.setTextureSet(humanAnimName);
				mHumanList[i].mHuman.mControl.setCurFrameIndex(i);
				Texture curHumanTexture = mHumanList[i].mHuman.getTexture();
				mHumanList[i].mHuman.setWindowSize(new Vector2(curHumanTexture.width, curHumanTexture.height));
				mHumanList[i].mHuman.setLocalPosition(new Vector3(frameData[0].mPosX[i], frameData[0].mPosY[i], 0.0f));
				// 武器
				mHumanList[i].mWeapon.setTextureSet(weaponAnimName);
				mHumanList[i].mWeapon.mControl.setCurFrameIndex(i);
				Texture curWeaponTexture = mHumanList[i].mWeapon.getTexture();
				mHumanList[i].mWeapon.setWindowSize(new Vector2(curWeaponTexture.width, curWeaponTexture.height));
			}
		}
		else if (mEditType == EDIT_TYPE.ET_MONSTER)
		{
			List<MonsterFrameData> frameData = null;
			mSQLite.mSQLiteMonsterFrame.query(mID, mDirection, mAction, out frameData);
			if (frameData.Count == 0)
			{
				logError("frame count is 0");
				return;
			}
			int frameCount = frameData[0].mFrameCount;
			int curFrameCount = mMonsterList.Count;
			if (curFrameCount < frameCount)
			{
				for (int i = 0; i < frameCount - curFrameCount; ++i)
				{
					createMonsterAvatar(curFrameCount + i, frameData[0].mID, frameData[0].mPrefab, frameData[0].mPrefab + curFrameCount + i);
				}
			}
			else if (curFrameCount > frameCount)
			{
				for (int i = 0; i < curFrameCount - frameCount; ++i)
				{
					destroyMonsterAvatar(curFrameCount - i - 1);
				}
			}
			setFrameCount(frameCount);
			string monsterAnimName = "monster_id" + mID + "_dir" + mDirection + "_" + mAction;
			// 设置每一帧的图片
			for (int i = 0; i < frameCount; ++i)
			{
				mMonsterList[i].mFrame.setTextureSet(monsterAnimName);
				mMonsterList[i].mFrame.mControl.setCurFrameIndex(i);
				Texture curMonsterTexture = mMonsterList[i].mFrame.getTexture();
				mMonsterList[i].mFrame.setWindowSize(new Vector2(curMonsterTexture.width, curMonsterTexture.height));
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
		destroyObject(avatar.mHumanRoot, true);
	}
	public void clearHuman()
	{
		Dictionary<int, HumanAvatar> list = new Dictionary<int, HumanAvatar>(mHumanList);
		foreach(var item in list)
		{
			destroyHumanAvatar(item.Key);
		}
	}
	public void clearMonster()
	{
		Dictionary<int, MonsterAvatar> list = new Dictionary<int, MonsterAvatar>(mMonsterList);
		foreach (var item in list)
		{
			destroyMonsterAvatar(item.Key);
		}
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
		destroyObject(avatar.mMonsterRoot, true);
	}
	public MonsterAvatar getMonster(int instanceID)
	{
		if(mMonsterList.ContainsKey(instanceID))
		{
			return mMonsterList[instanceID];
		}
		return null;
	}
	// 编辑相关
	//--------------------------------------------------------------------------------------------------------------------
	protected void setEditType(EDIT_TYPE type, bool refresh = true)
	{
		mEditType = type;
		disableEditType(mEditType);
		mAnimIndex = 0;
		if (mEditType == EDIT_TYPE.ET_HUMAN)
		{
			mTypeInfo.setLabel("角色");
			enableOccupation(true);
			enableHumanID(false);
			setAction(mHumanAnimation[mAnimIndex], refresh);
		}
		else if (mEditType == EDIT_TYPE.ET_MONSTER)
		{
			mTypeInfo.setLabel("怪物");
			enableOccupation(false);
			enableHumanID(false);
			setAction(mMonsterAnimation[mAnimIndex], refresh);
		}
		else if (mEditType == EDIT_TYPE.ET_WEAPON)
		{
			mTypeInfo.setLabel("武器");
			enableOccupation(true);
			enableHumanID(true);
			setAction(mHumanAnimation[mAnimIndex], refresh);
		}
		// 只要切换了编辑模式,就需要把所有物体清空
		clearHuman();
		clearMonster();
		if (refresh)
		{
			updateEdit();
		}
	}
	protected void setID(int id, bool refresh = true)
	{
		mID = id;
		mCurIDEdit.setText(StringUtility.intToString(mID));
		if (refresh)
		{
			updateEdit();
		}
	}
	protected void setOccupation(int occupation, bool refresh = true)
	{
		mOccupation = occupation;
		mCurOccupationEdit.setText(StringUtility.intToString(mOccupation));
		if (refresh)
		{
			updateEdit();
		}
	}
	protected void setDirection(int direction, bool refresh = true)
	{
		mDirection = direction;
		mCurDirectionEdit.setText(StringUtility.intToString(mDirection));
		if (refresh)
		{
			updateEdit();
		}
	}
	protected void setAction(string action, bool refresh = true)
	{
		mAction = action;
		mCurActionEdit.setText(mAction);
		if (refresh)
		{
			updateEdit();
		}
	}
	protected void setFrameCount(int frameCount)
	{
		mFrameCount = frameCount;
		mFrameCountLabel.setLabel(StringUtility.intToString(mFrameCount));
	}
	protected void setHumanID(int humanID)
	{
		mHumanID = humanID;
		mCurHumanIDEdit.setText(StringUtility.intToString(mHumanID));
	}
	protected void disableEditType(EDIT_TYPE type)
	{
		LayoutTools.ACTIVE_WINDOW(mHumanEditor, type != EDIT_TYPE.ET_HUMAN);
		LayoutTools.ACTIVE_WINDOW(mWeaponEditor, type != EDIT_TYPE.ET_WEAPON);
		LayoutTools.ACTIVE_WINDOW(mMonsterEditor, type != EDIT_TYPE.ET_MONSTER);
	}
	protected void enableOccupation(bool enable)
	{
		LayoutTools.ACTIVE_WINDOW(mLastOccupation, enable);
		LayoutTools.ACTIVE_WINDOW(mCurOccupationEdit, enable);
		LayoutTools.ACTIVE_WINDOW(mNextOccupation, enable);
	}
	protected void enableHumanID(bool enable)
	{
		LayoutTools.ACTIVE_WINDOW(mLastHumanID, enable);
		LayoutTools.ACTIVE_WINDOW(mCurHumanIDEdit, enable);
		LayoutTools.ACTIVE_WINDOW(mNextHumanID, enable);
	}
	protected void save()
	{
		if (mEditType == EDIT_TYPE.ET_WEAPON)
		{
			WeaponFrameData weaponData = new WeaponFrameData();
			weaponData.mID = mID;
			weaponData.mDirection = mDirection;
			weaponData.mAction = mAction;
			weaponData.mFrameCount = mFrameCount;
			weaponData.mPosX = new float[mFrameCount];
			weaponData.mPosY = new float[mFrameCount];
			for (int i = 0; i < mFrameCount; ++i)
			{
				weaponData.mPosX[i] = mHumanList[i].mWeapon.getPosition().x;
				weaponData.mPosY[i] = mHumanList[i].mWeapon.getPosition().y;
			}
			mSQLite.mSQLiteWeaponFrame.updateData(weaponData);
		}
		else if(mEditType == EDIT_TYPE.ET_HUMAN)
		{
			ClothFrameData clothData = new ClothFrameData();
			clothData.mID = mID;
			clothData.mDirection = mDirection;
			clothData.mAction = mAction;
			clothData.mFrameCount = mFrameCount;
			clothData.mPosX = new float[mFrameCount];
			clothData.mPosY = new float[mFrameCount];
			for (int i = 0; i < mFrameCount; ++i)
			{
				clothData.mPosX[i] = mHumanList[i].mWeapon.getPosition().x;
				clothData.mPosY[i] = mHumanList[i].mWeapon.getPosition().y;
			}
			mSQLite.mSQLiteClothFrame.updateData(clothData);
		}
		else if(mEditType == EDIT_TYPE.ET_MONSTER)
		{
			MonsterFrameData monsterData = new MonsterFrameData();
			monsterData.mID = mID;
			monsterData.mDirection = mDirection;
			monsterData.mAction = mAction;
			monsterData.mFrameCount = mFrameCount;
			monsterData.mPosX = new float[mFrameCount];
			monsterData.mPosY = new float[mFrameCount];
			for (int i = 0; i < mFrameCount; ++i)
			{
				monsterData.mPosX[i] = mMonsterList[i].mFrame.getPosition().x;
				monsterData.mPosY[i] = mMonsterList[i].mFrame.getPosition().y;
			}
			mSQLite.mSQLiteMonsterFrame.updateData(monsterData);
		}
	}
	// 回调
	//---------------------------------------------------------------------------------------------------------------
	protected void onWeaponClick(GameObject obj)
	{
		setEditType(EDIT_TYPE.ET_WEAPON);
	}
	protected void onHumanClick(GameObject obj)
	{
		setEditType(EDIT_TYPE.ET_HUMAN);
	}
	protected void onMonsterClick(GameObject obj)
	{
		setEditType(EDIT_TYPE.ET_MONSTER);
	}
	protected void onSaveClick(GameObject obj)
	{
		save();
	}
	protected void onLastID(GameObject obj)
	{
		int id = --mID;
		MathUtility.clampMin(ref id, 1);
		setID(id);
	}
	protected void onNextID(GameObject obj)
	{
		setID(++mID);
	}
	protected void onLastOccupation(GameObject obj)
	{
		setOccupation((mOccupation - 1 + GameDefine.MAX_OCCUPATION) % GameDefine.MAX_OCCUPATION);
	}
	protected void onNextOccupation(GameObject obj)
	{
		setOccupation((mOccupation + 1) % GameDefine.MAX_OCCUPATION);
	}
	protected void onLastDirection(GameObject obj)
	{
		setDirection((mDirection - 1 + GameDefine.DIRECTION_COUNT) % GameDefine.DIRECTION_COUNT);
	}
	protected void onNextDirection(GameObject obj)
	{
		setDirection((mDirection + 1) % GameDefine.DIRECTION_COUNT);
	}
	protected void onLastAction(GameObject obj)
	{
		if(mEditType == EDIT_TYPE.ET_HUMAN)
		{
			mAnimIndex = (mAnimIndex - 1 + mHumanAnimation.Length) % mHumanAnimation.Length;
			setAction(mHumanAnimation[mAnimIndex]);
		}
		else if(mEditType == EDIT_TYPE.ET_WEAPON)
		{
			mAnimIndex = (mAnimIndex - 1 + mHumanAnimation.Length) % mHumanAnimation.Length;
			setAction(mHumanAnimation[mAnimIndex]);
		}
		else if(mEditType == EDIT_TYPE.ET_MONSTER)
		{
			mAnimIndex = (mAnimIndex - 1 + mMonsterAnimation.Length) % mMonsterAnimation.Length;
			setAction(mMonsterAnimation[mAnimIndex]);
		}
	}
	protected void onNextAction(GameObject obj)
	{
		if (mEditType == EDIT_TYPE.ET_HUMAN)
		{
			mAnimIndex = (mAnimIndex + 1) % mHumanAnimation.Length;
			setAction(mHumanAnimation[mAnimIndex]);
		}
		else if (mEditType == EDIT_TYPE.ET_WEAPON)
		{
			mAnimIndex = (mAnimIndex + 1) % mHumanAnimation.Length;
			setAction(mHumanAnimation[mAnimIndex]);
		}
		else if (mEditType == EDIT_TYPE.ET_MONSTER)
		{
			mAnimIndex = (mAnimIndex + 1) % mMonsterAnimation.Length;
			setAction(mMonsterAnimation[mAnimIndex]);
		}
	}
	protected void onLastHumanID(GameObject obj)
	{
		int humanID = --mHumanID;
		MathUtility.clampMin(ref humanID, 1);
		setHumanID(humanID);
	}
	protected void onNextHumanID(GameObject obj)
	{
		setHumanID(++mHumanID);
	}
}
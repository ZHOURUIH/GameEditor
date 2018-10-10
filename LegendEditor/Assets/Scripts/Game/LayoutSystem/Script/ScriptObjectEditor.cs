//using System;
//using System.Collections.Generic;
//using System.Linq;
//using System.Text;
//using UnityEngine;

//public enum EDIT_TYPE
//{
//	ET_WEAPON,
//	ET_HUMAN,
//	ET_MONSTER,
//}

//public class ScriptObjectEditor : LayoutScript
//{
//	protected Dictionary<int, HumanAvatar> mHumanList;
//	//protected Dictionary<int, MonsterAvatar> mMonsterList;
//	protected txUIObject mEditorRoot;
//	protected txNGUIText mTypeInfo;
//	protected txNGUIText mInfo;
//	protected txNGUIText mFrameCountLabel;
//	protected txNGUIText mEditTip;
//	protected txNGUIButton mWeaponEditor;
//	protected txNGUIButton mHumanEditor;
//	protected txNGUIButton mMonsterEditor;
//	protected txNGUIButton mEffectEditor;
//	protected txNGUIButton mLastID;
//	protected txNGUIEditbox mCurIDEdit;
//	protected txNGUIButton mNextID;
//	protected txNGUIButton mLastOccupation;
//	protected txNGUIEditbox mCurOccupationEdit;
//	protected txNGUIButton mNextOccupation;
//	protected txNGUIButton mLastDirection;
//	protected txNGUIEditbox mCurDirectionEdit;
//	protected txNGUIButton mNextDirection;
//	protected txNGUIButton mLastAction;
//	protected txNGUIEditbox mCurActionEdit;
//	protected txNGUIButton mNextAction;
//	protected txNGUIButton mLastHumanID;
//	protected txNGUIEditbox mCurHumanIDEdit;
//	protected txNGUIButton mNextHumanID;
//	protected EDIT_TYPE mEditType;
//	protected int mID;
//	protected int mDirection;
//	protected string[] mHumanAnimation;
//	//protected string[] mMonsterAnimation;
//	protected int mAnimIndex;
//	protected string mAction;
//	protected int mHumanID;
//	protected int mOccupation;
//	protected int mFrameCount;
//	public ScriptObjectEditor(string name, GameLayout layout)
//		:
//		base(name, layout)
//	{
//		mHumanList = new Dictionary<int, HumanAvatar>();
//		//mMonsterList = new Dictionary<int, MonsterAvatar>();
//		mHumanAnimation = new string[] { "stand", "walk", "run", "attack", "dig", "jumpAttack", "skill", "search", "hit", "die" };
//		//mMonsterAnimation = new string[] { "attack", "death", "hit", "stand" };
//	}
//	public override void assignWindow()
//	{
//		newObject(out mEditorRoot, "EditorRoot");
//		newObject(out mTypeInfo, mEditorRoot, "TypeInfo");
//		newObject(out mInfo, mEditorRoot, "Info");
//		newObject(out mFrameCountLabel, mEditorRoot, "FrameCountLabel");
//		newObject(out mEditTip, mEditorRoot, "EditTip");
//		newObject(out mWeaponEditor, mEditorRoot, "WeaponEditor");
//		newObject(out mHumanEditor, mEditorRoot, "HumanEditor");
//		newObject(out mMonsterEditor, mEditorRoot, "MonsterEditor");
//		newObject(out mEffectEditor, mEditorRoot, "EffectEditor");
//		newObject(out mLastID, mEditorRoot, "LastID");
//		newObject(out mCurIDEdit, mEditorRoot, "CurIDEdit");
//		newObject(out mNextID, mEditorRoot, "NextID");
//		newObject(out mLastOccupation, mEditorRoot, "LastOccupation");
//		newObject(out mCurOccupationEdit, mEditorRoot, "CurOccupationEdit");
//		newObject(out mNextOccupation, mEditorRoot, "NextOccupation");
//		newObject(out mLastDirection, mEditorRoot, "LastDirection");
//		newObject(out mCurDirectionEdit, mEditorRoot, "CurDirectionEdit");
//		newObject(out mNextDirection, mEditorRoot, "NextDirection");
//		newObject(out mLastAction, mEditorRoot, "LastAction");
//		newObject(out mCurActionEdit, mEditorRoot, "CurActionEdit");
//		newObject(out mNextAction, mEditorRoot, "NextAction");
//		newObject(out mLastHumanID, mEditorRoot, "LastHumanID");
//		newObject(out mCurHumanIDEdit, mEditorRoot, "CurHumanIDEdit");
//		newObject(out mNextHumanID, mEditorRoot, "NextHumanID");
//	}
//	public override void init()
//	{
//		mAnimIndex = 0;
//		mHumanID = 1;
//		setFrameCount(0);
//		setEditType(EDIT_TYPE.ET_HUMAN, false);
//		setID(1, false);
//		setOccupation(3, false);
//		setAction(mHumanAnimation[0], false);
//		setDirection(2, true);
//		registeBoxColliderNGUI(mWeaponEditor, onWeaponClick);
//		registeBoxColliderNGUI(mHumanEditor, onHumanClick);
//		registeBoxColliderNGUI(mMonsterEditor, onMonsterClick);
//		registeBoxColliderNGUI(mLastID, onLastID);
//		registeBoxColliderNGUI(mNextID, onNextID);
//		registeBoxColliderNGUI(mLastOccupation, onLastOccupation);
//		registeBoxColliderNGUI(mNextOccupation, onNextOccupation);
//		registeBoxColliderNGUI(mLastDirection, onLastDirection);
//		registeBoxColliderNGUI(mNextDirection, onNextDirection);
//		registeBoxColliderNGUI(mLastAction, onLastAction);
//		registeBoxColliderNGUI(mNextAction, onNextAction);
//		registeBoxColliderNGUI(mLastHumanID, onLastHumanID);
//		registeBoxColliderNGUI(mNextHumanID, onNextHumanID);
//	}
//	public override void onReset()
//	{
//		;
//	}
//	public override void onShow(bool immediately, string param)
//	{
//		;
//	}
//	public override void onHide(bool immediately, string param)
//	{
//		;
//	}
//	public override void update(float elapsedTime)
//	{
//		;
//	}
//	// 物体相关
//	//---------------------------------------------------------------------------------------------------------------------------
//	public HumanAvatar createHumanAvatar(int charGUID)
//	{
//		string avatarName = "HumanAvatar" + charGUID;
//		instantiateObject(mRoot, "HumanAvatar", avatarName);
//		HumanAvatar avatar = new HumanAvatar(this);
//		avatar.assignWindow(avatarName);
//		avatar.init(charGUID);
//		mHumanList.Add(charGUID, avatar);
//		return avatar;
//	}
//	public void destroyHumanAvatar(int charGUID)
//	{
//		HumanAvatar avatar = mHumanList[charGUID];
//		mHumanList.Remove(charGUID);
//		destroyObject(avatar.mHumanRoot, true);
//	}
//	public void clearHuman()
//	{
//		Dictionary<int, HumanAvatar> list = new Dictionary<int, HumanAvatar>(mHumanList);
//		foreach(var item in list)
//		{
//			destroyHumanAvatar(item.Key);
//		}
//	}
//	//public void clearMonster()
//	//{
//	//	Dictionary<int, MonsterAvatar> list = new Dictionary<int, MonsterAvatar>(mMonsterList);
//	//	foreach (var item in list)
//	//	{
//	//		destroyMonsterAvatar(item.Key);
//	//	}
//	//}
//	//public MonsterAvatar createMonsterAvatar(int instanceID, int typeID, string prefabName, string label = "")
//	//{
//	//	if(label == "")
//	//	{
//	//		label = prefabName;
//	//	}
//	//	string avatarName = label + instanceID;
//	//	instantiateObject(mRoot, prefabName, avatarName);
//	//	MonsterAvatar avatar = new MonsterAvatar(this);
//	//	avatar.assignWindow(avatarName);
//	//	avatar.init(instanceID, typeID, label);
//	//	mMonsterList.Add(instanceID, avatar);
//	//	return avatar;
//	//}
//	//public void destroyMonsterAvatar(int instanceID)
//	//{
//	//	MonsterAvatar avatar = mMonsterList[instanceID];
//	//	mMonsterList.Remove(instanceID);
//	//	destroyObject(avatar.mMonsterRoot, true);
//	//}
//	//public MonsterAvatar getMonster(int instanceID)
//	//{
//	//	if(mMonsterList.ContainsKey(instanceID))
//	//	{
//	//		return mMonsterList[instanceID];
//	//	}
//	//	return null;
//	//}
//	// 编辑相关
//	//--------------------------------------------------------------------------------------------------------------------
//	protected void setEditType(EDIT_TYPE type, bool refresh = true)
//	{
//		mEditType = type;
//		disableEditType(mEditType);
//		mAnimIndex = 0;
//		if (mEditType == EDIT_TYPE.ET_HUMAN)
//		{
//			mTypeInfo.setLabel("角色");
//			enableOccupation(true);
//			enableHumanID(false);
//			setAction(mHumanAnimation[mAnimIndex], refresh);
//		}
//		//else if (mEditType == EDIT_TYPE.ET_MONSTER)
//		//{
//		//	mTypeInfo.setLabel("怪物");
//		//	enableOccupation(false);
//		//	enableHumanID(false);
//		//	setAction(mMonsterAnimation[mAnimIndex], refresh);
//		//}
//		else if (mEditType == EDIT_TYPE.ET_WEAPON)
//		{
//			mTypeInfo.setLabel("武器");
//			enableOccupation(true);
//			enableHumanID(true);
//			setAction(mHumanAnimation[mAnimIndex], refresh);
//		}
//		//else if(mEditType == EDIT_TYPE.ET_EFFECT)
//		//{
//		//	mTypeInfo.setLabel("特效");
//		//	enableOccupation(false);
//		//	enableHumanID(false);
//		//	// 读取Magic目录中的所有序列帧,加入到下拉列表中
//		//	List<string> folderList = new List<string>();
//		//	FileUtility.findDirectory(CommonDefine.F_TEXTURE_ANIM_PATH + "Magic", ref folderList, false);
//		//	mResourceList.clearAllItem();
//		//	int folderCount = folderList.Count;
//		//	for(int i = 0; i < folderCount && i < 100; ++i)
//		//	{
//		//		int startIndex = (CommonDefine.F_TEXTURE_ANIM_PATH + "Magic/").Length;
//		//		mResourceList.appendItem(folderList[i].Substring(startIndex, folderList[i].Length - startIndex));
//		//	}
//		//}
//		// 只要切换了编辑模式,就需要把所有物体清空
//		clearHuman();
//		//clearMonster();
//	}
//	protected void setID(int id, bool refresh = true)
//	{
//		mID = id;
//		mCurIDEdit.setText(StringUtility.intToString(mID));
//	}
//	protected void setOccupation(int occupation, bool refresh = true)
//	{
//		mOccupation = occupation;
//		mCurOccupationEdit.setText(StringUtility.intToString(mOccupation));
//	}
//	protected void setDirection(int direction, bool refresh = true)
//	{
//		mDirection = direction;
//		mCurDirectionEdit.setText(StringUtility.intToString(mDirection));
//	}
//	protected void setAction(string action, bool refresh = true)
//	{
//		mAction = action;
//		mCurActionEdit.setText(mAction);
//	}
//	protected void setFrameCount(int frameCount)
//	{
//		mFrameCount = frameCount;
//		mFrameCountLabel.setLabel(StringUtility.intToString(mFrameCount));
//	}
//	protected void setHumanID(int humanID)
//	{
//		mHumanID = humanID;
//		mCurHumanIDEdit.setText(StringUtility.intToString(mHumanID));
//	}
//	protected void disableEditType(EDIT_TYPE type)
//	{
//		LayoutTools.ACTIVE_WINDOW(mHumanEditor, type != EDIT_TYPE.ET_HUMAN);
//		LayoutTools.ACTIVE_WINDOW(mWeaponEditor, type != EDIT_TYPE.ET_WEAPON);
//		LayoutTools.ACTIVE_WINDOW(mMonsterEditor, type != EDIT_TYPE.ET_MONSTER);
//	}
//	protected void enableOccupation(bool enable)
//	{
//		LayoutTools.ACTIVE_WINDOW(mLastOccupation, enable);
//		LayoutTools.ACTIVE_WINDOW(mCurOccupationEdit, enable);
//		LayoutTools.ACTIVE_WINDOW(mNextOccupation, enable);
//	}
//	protected void enableHumanID(bool enable)
//	{
//		LayoutTools.ACTIVE_WINDOW(mLastHumanID, enable);
//		LayoutTools.ACTIVE_WINDOW(mCurHumanIDEdit, enable);
//		LayoutTools.ACTIVE_WINDOW(mNextHumanID, enable);
//	}
//	// 回调
//	//---------------------------------------------------------------------------------------------------------------
//	protected void onWeaponClick(GameObject obj)
//	{
//		setEditType(EDIT_TYPE.ET_WEAPON);
//	}
//	protected void onHumanClick(GameObject obj)
//	{
//		setEditType(EDIT_TYPE.ET_HUMAN);
//	}
//	protected void onMonsterClick(GameObject obj)
//	{
//		//setEditType(EDIT_TYPE.ET_MONSTER);
//	}
//	protected void onLastID(GameObject obj)
//	{
//		int id = --mID;
//		MathUtility.clampMin(ref id, 1);
//		setID(id);
//	}
//	protected void onNextID(GameObject obj)
//	{
//		setID(++mID);
//	}
//	protected void onLastOccupation(GameObject obj)
//	{
//		setOccupation((mOccupation - 1 + GameDefine.MAX_OCCUPATION) % GameDefine.MAX_OCCUPATION);
//	}
//	protected void onNextOccupation(GameObject obj)
//	{
//		setOccupation((mOccupation + 1) % GameDefine.MAX_OCCUPATION);
//	}
//	protected void onLastDirection(GameObject obj)
//	{
//		setDirection((mDirection - 1 + GameDefine.DIRECTION_COUNT) % GameDefine.DIRECTION_COUNT);
//	}
//	protected void onNextDirection(GameObject obj)
//	{
//		setDirection((mDirection + 1) % GameDefine.DIRECTION_COUNT);
//	}
//	protected void onLastAction(GameObject obj)
//	{
//		if(mEditType == EDIT_TYPE.ET_HUMAN)
//		{
//			mAnimIndex = (mAnimIndex - 1 + mHumanAnimation.Length) % mHumanAnimation.Length;
//			setAction(mHumanAnimation[mAnimIndex]);
//		}
//		else if(mEditType == EDIT_TYPE.ET_WEAPON)
//		{
//			mAnimIndex = (mAnimIndex - 1 + mHumanAnimation.Length) % mHumanAnimation.Length;
//			setAction(mHumanAnimation[mAnimIndex]);
//		}
//		//else if(mEditType == EDIT_TYPE.ET_MONSTER)
//		//{
//		//	mAnimIndex = (mAnimIndex - 1 + mMonsterAnimation.Length) % mMonsterAnimation.Length;
//		//	setAction(mMonsterAnimation[mAnimIndex]);
//		//}
//	}
//	protected void onNextAction(GameObject obj)
//	{
//		if (mEditType == EDIT_TYPE.ET_HUMAN)
//		{
//			mAnimIndex = (mAnimIndex + 1) % mHumanAnimation.Length;
//			setAction(mHumanAnimation[mAnimIndex]);
//		}
//		else if (mEditType == EDIT_TYPE.ET_WEAPON)
//		{
//			mAnimIndex = (mAnimIndex + 1) % mHumanAnimation.Length;
//			setAction(mHumanAnimation[mAnimIndex]);
//		}
//		//else if (mEditType == EDIT_TYPE.ET_MONSTER)
//		//{
//		//	mAnimIndex = (mAnimIndex + 1) % mMonsterAnimation.Length;
//		//	setAction(mMonsterAnimation[mAnimIndex]);
//		//}
//	}
//	protected void onLastHumanID(GameObject obj)
//	{
//		int humanID = --mHumanID;
//		MathUtility.clampMin(ref humanID, 1);
//		setHumanID(humanID);
//	}
//	protected void onNextHumanID(GameObject obj)
//	{
//		setHumanID(++mHumanID);
//	}
//}
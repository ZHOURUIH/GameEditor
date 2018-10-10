using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using UnityEngine;

public class WeaponAnim
{
	public string mTextureSet;
	public int mID;
	public string mAction;
	public int mDirection;
}

public class HumanAnim
{
	public string mTextureSet;
	public int mID;
	public string mAction;
	public int mDirection;
}

public class WeaponAtlas
{
	public string mAtlasName;
	public Dictionary<int, Dictionary<string, WeaponAnim>> mAnimSetList;
}

public class HumanAtlas
{
	public string mAtlasName;
	public Dictionary<int, Dictionary<string, HumanAnim>> mAnimSetList;
}

public class ScriptHumanSpriteEditor : LayoutScript
{
	protected txNGUIEditbox mHumanName;
	protected txNGUIText mHumanCount;
	protected txNGUIButton mRefresh;
	protected txNGUIButton mLastHuman;
	protected txNGUIEditbox mCurHumanEdit;
	protected txNGUIButton mNextHuman;
	protected txNGUIButton mLastWeapon;
	protected txNGUIEditbox mCurWeaponEdit;
	protected txNGUIButton mNextWeapon;
	protected txNGUIButton mLastDirection;
	protected txNGUIEditbox mCurDirectionEdit;
	protected txNGUIButton mNextDirection;
	protected txNGUIButton mLastAction;
	protected txNGUIEditbox mCurActionEdit;
	protected txNGUIButton mNextAction;
	protected txNGUIButton mPlayButton;
	protected txNGUIButton mPauseButton;
	protected txNGUIButton mStopButton;
	protected txNGUIEditbox mPlaySpeed;
	protected txNGUICheckBox mPlayLoop;
	protected txUIObject mPauseOperate;
	protected txNGUIButton mLastFrame;
	protected txNGUIButton mNextFrame;
	protected txNGUIText mFrameCount;
	protected HumanSpriteInstance mHumanSpriteInstance;
	protected List<HumanAtlas> mClothResourceList;
	protected List<WeaponAtlas> mWeaponResourceList;
	protected int mHumanIndex;
	protected int mWeaponIndex;
	protected int mDirection;
	protected string[] mActionList;
	protected int mAction;
	protected INGUIAnimation mHumanAnim;
	public ScriptHumanSpriteEditor(string name, GameLayout layout)
		:
		base(name, layout)
	{
		mClothResourceList = new List<HumanAtlas>();
		mWeaponResourceList = new List<WeaponAtlas>();
		mHumanSpriteInstance = new HumanSpriteInstance(this);
		mActionList = new string[] { "attack", "die", "dig", "hit", "jumpAttack", "run", "search", "skill", "stand", "walk" };
		mLayout.setCheckBoxAnchor(false);
	}
	public override void assignWindow()
	{
		newObject(out mHumanName, "HumanName");
		newObject(out mHumanCount, "HumanCount");
		newObject(out mRefresh, "Refresh");
		newObject(out mLastHuman, "LastHuman");
		newObject(out mCurHumanEdit, "CurHumanEdit");
		newObject(out mNextHuman, "NextHuman");
		newObject(out mLastWeapon, "LastWeapon");
		newObject(out mCurWeaponEdit, "CurWeaponEdit");
		newObject(out mNextWeapon, "NextWeapon");
		newObject(out mLastDirection, "LastDirection");
		newObject(out mCurDirectionEdit, "CurDirectionEdit");
		newObject(out mNextDirection, "NextDirection");
		newObject(out mLastAction, "LastAction");
		newObject(out mCurActionEdit, "CurActionEdit");
		newObject(out mNextAction, "NextAction");
		newObject(out mPlayButton, "PlayButton");
		newObject(out mPauseButton, "PauseButton");
		newObject(out mStopButton, "StopButton");
		newObject(out mPauseOperate, "PauseOperate");
		newObject(out mLastFrame, mPauseOperate, "LastFrame");
		newObject(out mNextFrame, mPauseOperate, "NextFrame");
		newObject(out mFrameCount, mPauseOperate, "FrameCount");
		newObject(out mPlaySpeed, "PlaySpeed");
		newObject(out mPlayLoop, "PlayLoop");
		instantiateObject(mRoot, "HumanAvatar");
		mHumanSpriteInstance.assignWindow("HumanAvatar");
	}
	public override void init()
	{
		registeBoxColliderNGUI(mRefresh, onRefreshClick);
		registeBoxColliderNGUI(mLastHuman, onLastHumanClick);
		registeBoxColliderNGUI(mNextHuman, onNextHumanClick);
		registeBoxColliderNGUI(mLastWeapon, onLastWeaponClick);
		registeBoxColliderNGUI(mNextWeapon, onNextWeaponClick);
		registeBoxColliderNGUI(mLastDirection, onLastDirectionClick);
		registeBoxColliderNGUI(mNextDirection, onNextDirectionClick);
		registeBoxColliderNGUI(mLastAction, onLastActionClick);
		registeBoxColliderNGUI(mNextAction, onNextActionClick);
		registeBoxColliderNGUI(mPlayButton, onPlayClick);
		registeBoxColliderNGUI(mPauseButton, onPauseClick);
		registeBoxColliderNGUI(mStopButton, onStopClick);
		registeBoxColliderNGUI(mLastFrame, onLastFrameClick);
		registeBoxColliderNGUI(mNextFrame, onNextFrameClick);
		mPlaySpeed.setInputChangedCallback(onPlaySpeedChanged);
		mPlayLoop.setCheckChangedCallback(onPlayLoopChanged);
		mHumanSpriteInstance.init();
		mPlaySpeed.setText("0.25f");
		mPlayLoop.setChecked(true);
		mHumanAnim = mHumanSpriteInstance.mCloth;
		mHumanAnim.setSpeed(0.25f);
	}
	public override void onReset()
	{
		mHumanSpriteInstance.onReset();
		LayoutTools.ACTIVE_WINDOW(mPauseOperate, false);
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
		;
	}
	// 回调
	//---------------------------------------------------------------------------------------------------------------
	protected void onPlayClick(GameObject obj)
	{
		mHumanAnim.play();
		checkPause();
	}
	protected void onPauseClick(GameObject obj)
	{
		mHumanAnim.pause();
		checkPause();
	}
	protected void onStopClick(GameObject obj)
	{
		mHumanAnim.stop();
		checkPause();
	}
	protected void onPlaySpeedChanged()
	{
		mHumanSpriteInstance.setPlaySpeed(StringUtility.stringToFloat(mPlaySpeed.getText()));
	}
	protected void onPlayLoopChanged()
	{
		mHumanSpriteInstance.setLoop(mPlayLoop.getChecked());	
	}
	// 上一个图集
	protected void onLastHumanClick(GameObject obj)
	{
		if(mClothResourceList.Count == 0)
		{
			return;
		}
		// 首先查找当前特效所在的下标
		setCurClothAtlas((mHumanIndex - 1 + mClothResourceList.Count) % mClothResourceList.Count);
	}
	// 下一个图集
	protected void onNextHumanClick(GameObject obj)
	{
		if (mClothResourceList.Count == 0)
		{
			return;
		}
		setCurClothAtlas((mHumanIndex + 1) % mClothResourceList.Count);
	}
	protected void onLastWeaponClick(GameObject obj)
	{
		if (mWeaponResourceList.Count == 0)
		{
			return;
		}
		// 首先查找当前特效所在的下标
		setCurWeaponAtlas((mWeaponIndex - 1 + mWeaponResourceList.Count) % mWeaponResourceList.Count);
	}
	// 下一个图集
	protected void onNextWeaponClick(GameObject obj)
	{
		if (mWeaponResourceList.Count == 0)
		{
			return;
		}
		setCurWeaponAtlas((mWeaponIndex + 1) % mWeaponResourceList.Count);
	}
	// 当前动作的上一个方向
	protected void onLastDirectionClick(GameObject obj)
	{
		setCurAnim((mDirection - 1 + 8) % 8, mAction);
	}
	// 当前动作的下一个方向
	protected void onNextDirectionClick(GameObject obj)
	{
		setCurAnim((mDirection + 1) % 8, mAction);
	}
	// 当前方向的上一个动作
	protected void onLastActionClick(GameObject obj)
	{
		setCurAnim(mDirection, (mAction - 1 + mActionList.Length) % mActionList.Length);
	}
	// 当前方向的下一个动作
	protected void onNextActionClick(GameObject obj)
	{
		setCurAnim(mDirection, (mAction + 1) % mActionList.Length);
	}
	protected void onRefreshClick(GameObject obj)
	{
		// 从数据库中查找
		mClothResourceList.Clear();
		mWeaponResourceList.Clear();
		List<ClothData> clothDataList;
		mSQLiteCloth.queryAll(out clothDataList);
		int clothCount = clothDataList.Count;
		for(int i = 0; i < clothCount; ++i)
		{
			HumanAtlas clothAtlas = new HumanAtlas();
			clothAtlas.mAtlasName = clothDataList[i].mResource;
			clothAtlas.mAnimSetList = new Dictionary<int, Dictionary<string, HumanAnim>>();
			var animList = clothAtlas.mAnimSetList;
			List<ClothFrameData> frameDataList;
			mSQLiteClothFrame.query(clothDataList[i].mID, out frameDataList);
			int frameDataCount = frameDataList.Count;
			for (int j = 0; j < frameDataCount; ++j)
			{
				HumanAnim anim = new HumanAnim();
				anim.mID = frameDataList[j].mID;
				anim.mAction = frameDataList[j].mAction;
				anim.mDirection = frameDataList[j].mDirection;
				anim.mTextureSet = frameDataList[j].mAction + "_dir" + frameDataList[j].mDirection;
				if(!animList.ContainsKey(anim.mDirection))
				{
					animList.Add(anim.mDirection, new Dictionary<string, HumanAnim>());
				}
				if(!animList[anim.mDirection].ContainsKey(anim.mAction))
				{
					animList[anim.mDirection].Add(anim.mAction, anim);
				}
			}
			mClothResourceList.Add(clothAtlas);
		}
		List<WeaponData> weaponDataList;
		mSQLiteWeapon.queryAll(out weaponDataList);
		int weaponCount = weaponDataList.Count;
		for (int i = 0; i < weaponCount; ++i)
		{
			WeaponAtlas weaponAtlas = new WeaponAtlas();
			weaponAtlas.mAtlasName = weaponDataList[i].mResource;
			weaponAtlas.mAnimSetList = new Dictionary<int, Dictionary<string, WeaponAnim>>();
			var animList = weaponAtlas.mAnimSetList;
			List<WeaponFrameData> frameDataList;
			mSQLiteWeaponFrame.query(weaponDataList[i].mID, out frameDataList);
			int frameDataCount = frameDataList.Count;
			for (int j = 0; j < frameDataCount; ++j)
			{
				WeaponAnim anim = new WeaponAnim();
				anim.mID = frameDataList[j].mID;
				anim.mAction = frameDataList[j].mAction;
				anim.mDirection = frameDataList[j].mDirection;
				anim.mTextureSet = frameDataList[j].mAction + "_dir" + frameDataList[j].mDirection;
				if (!animList.ContainsKey(anim.mDirection))
				{
					animList.Add(anim.mDirection, new Dictionary<string, WeaponAnim>());
				}
				if (!animList[anim.mDirection].ContainsKey(anim.mAction))
				{
					animList[anim.mDirection].Add(anim.mAction, anim);
				}
			}
			mWeaponResourceList.Add(weaponAtlas);
		}
		mHumanCount.setLabel("数量:0/" + mClothResourceList.Count);
		// 设置
		setCurClothAtlas(0);
		setCurWeaponAtlas(0);
	}
	protected void onLastFrameClick(GameObject obj)
	{
		if(mHumanAnim.getPlayState() == PLAY_STATE.PS_PAUSE)
		{
			int curFrame = mHumanAnim.getCurFrameIndex() - 1;
			MathUtility.clampMin(ref curFrame, 0);
			mHumanAnim.setCurFrameIndex(curFrame);
			mFrameCount.setLabel("帧数:" + (mHumanAnim.getCurFrameIndex() + 1) + "/" + mHumanAnim.getTextureFrameCount());
		}
	}
	protected void onNextFrameClick(GameObject obj)
	{
		if (mHumanAnim.getPlayState() == PLAY_STATE.PS_PAUSE)
		{
			int curFrame = mHumanAnim.getCurFrameIndex() + 1;
			MathUtility.clampMax(ref curFrame, mHumanAnim.getTextureFrameCount() - 1);
			mHumanAnim.setCurFrameIndex(curFrame);
			mFrameCount.setLabel("帧数:" + (mHumanAnim.getCurFrameIndex() + 1) + "/" + mHumanAnim.getTextureFrameCount());
		}
	}
	protected void setCurAnim(int direction, int action)
	{
		mDirection = direction;
		mAction = action;
		mCurDirectionEdit.setText("" + mDirection);
		mCurActionEdit.setText(mActionList[mAction]);
		HumanAtlas curClothAtlas = mClothResourceList[mHumanIndex];
		WeaponAtlas curWeaponAtlas = mWeaponResourceList[mWeaponIndex];
		HumanAnim curClothAnim = curClothAtlas.mAnimSetList[mDirection][mActionList[mAction]];
		WeaponAnim curWeaponAnim = curWeaponAtlas.mAnimSetList[mDirection][mActionList[mAction]];
		float playSpeed = StringUtility.stringToFloat(mPlaySpeed.getText());
		mHumanSpriteInstance.play(curClothAnim.mTextureSet, mPlayLoop.getChecked(), playSpeed);
		mHumanSpriteInstance.setClothTexturePosList(findClothPosList(curClothAnim));
		mHumanSpriteInstance.setWeaponTexturePosList(findWeaponPosList(curWeaponAnim));
		mHumanName.setText(curClothAtlas.mAtlasName + "/" + curClothAnim.mTextureSet);
		checkPause();
	}
	protected void setCurClothAtlas(int index)
	{
		if(index < 0 || index >= mClothResourceList.Count)
		{
			return;
		}
		mHumanIndex = index;
		mHumanCount.setLabel("数量:" + (mHumanIndex + 1) + "/" + mClothResourceList.Count);
		UIAtlas atlas = mResourceManager.loadResource<UIAtlas>(CommonDefine.R_ATLAS_TEXTURE_ANIM_PATH + "Human/" + mClothResourceList[mHumanIndex].mAtlasName, true);
		mHumanSpriteInstance.setClothAtlas(atlas);
		setCurAnim(0, 0);
		mCurHumanEdit.setText(mClothResourceList[mHumanIndex].mAtlasName);
	}
	protected void setCurWeaponAtlas(int index)
	{
		if (index < 0 || index >= mWeaponResourceList.Count)
		{
			return;
		}
		mWeaponIndex = index;
		UIAtlas atlas = mResourceManager.loadResource<UIAtlas>(CommonDefine.R_ATLAS_TEXTURE_ANIM_PATH + "Weapon/" + mWeaponResourceList[mWeaponIndex].mAtlasName, true);
		mHumanSpriteInstance.setWeaponAtlas(atlas);
		setCurAnim(0, 0);
		mCurWeaponEdit.setText(mWeaponResourceList[mWeaponIndex].mAtlasName);
	}
	protected List<Vector2> findClothPosList(HumanAnim anim)
	{
		List<ClothFrameData> frameDataList;
		mSQLiteClothFrame.query(anim.mID, anim.mDirection, anim.mAction, out frameDataList);
		if (frameDataList.Count != 1)
		{
			logError("huamn frame error!");
			return null;
		}
		List<Vector2> posList = new List<Vector2>();
		for (int j = 0; j < frameDataList[0].mFrameCount; ++j)
		{
			// 由于坐标系不一致,所以需要将Y方向偏移取反
			posList.Add(new Vector2(frameDataList[0].mPosX[j], -frameDataList[0].mPosY[j]));
		}
		return posList;
	}
	protected List<Vector2> findWeaponPosList(WeaponAnim anim)
	{
		List<WeaponFrameData> frameDataList;
		mSQLiteWeaponFrame.query(anim.mID, anim.mDirection, anim.mAction, out frameDataList);
		if (frameDataList.Count != 1)
		{
			logError("huamn frame error!");
			return null;
		}
		List<Vector2> posList = new List<Vector2>();
		for (int j = 0; j < frameDataList[0].mFrameCount; ++j)
		{
			// 由于坐标系不一致,所以需要将Y方向偏移取反
			posList.Add(new Vector2(frameDataList[0].mPosX[j], -frameDataList[0].mPosY[j]));
		}
		return posList;
	}
	protected void checkPause()
	{
		bool isPause = mHumanAnim.getPlayState() == PLAY_STATE.PS_PAUSE;
		LayoutTools.ACTIVE_WINDOW(mPauseOperate, isPause);
		mFrameCount.setLabel("帧数:" + (mHumanAnim.getCurFrameIndex() + 1) + "/" + mHumanAnim.getTextureFrameCount());
	}
}
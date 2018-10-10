using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using UnityEngine;

public class MonsterAnim
{
	public string mTextureSet;
	public int mID;
	public string mAction;
	public int mDirection;
}

public class MonsterAtlas
{
	public string mAtlasName;
	public Dictionary<int, Dictionary<string, MonsterAnim>> mAnimSetList;
}

public class ScriptMonsterSpriteEditor : LayoutScript
{
	protected txNGUIEditbox mMonsterName;
	protected txNGUIText mMonsterCount;
	protected txNGUIButton mRefresh;
	protected txNGUIButton mLastMonster;
	protected txNGUIEditbox mCurMonsterEdit;
	protected txNGUIButton mNextMonster;
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
	protected MonsterSpriteInstance mMonsterSpriteInstance;
	protected List<MonsterAtlas> mResourceList;
	protected int mMonsterIndex;
	protected int mDirection;
	protected string[] mActionList;
	protected int mAction;
	protected INGUIAnimation mMonsterAnim;
	public ScriptMonsterSpriteEditor(string name, GameLayout layout)
		:
		base(name, layout)
	{
		mResourceList = new List<MonsterAtlas>();
		mMonsterSpriteInstance = new MonsterSpriteInstance(this);
		mActionList = new string[] { "attack", "die", "hit", "stand", "walk" };
		mLayout.setCheckBoxAnchor(false);
	}
	public override void assignWindow()
	{
		newObject(out mMonsterName, "MonsterName");
		newObject(out mMonsterCount, "MonsterCount");
		newObject(out mRefresh, "Refresh");
		
		newObject(out mLastMonster, "LastMonster");
		newObject(out mCurMonsterEdit, "CurMonsterEdit");
		newObject(out mNextMonster, "NextMonster");
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
		instantiateObject(mRoot, "MonsterAvatar");
		mMonsterSpriteInstance.assignWindow("MonsterAvatar");
	}
	public override void init()
	{
		registeBoxColliderNGUI(mRefresh, onRefreshClick);
		registeBoxColliderNGUI(mLastMonster, onLastMonsterClick);
		registeBoxColliderNGUI(mNextMonster, onNextMonsterClick);
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
		mMonsterSpriteInstance.init();
		mPlaySpeed.setText("0.25f");
		mPlayLoop.setChecked(true);
		mMonsterAnim = mMonsterSpriteInstance.mFrame;
		mMonsterAnim.setSpeed(0.25f);
	}
	public override void onReset()
	{
		mMonsterSpriteInstance.onReset();
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
		mMonsterAnim.play();
		checkPause();
	}
	protected void onPauseClick(GameObject obj)
	{
		mMonsterAnim.pause();
		checkPause();
	}
	protected void onStopClick(GameObject obj)
	{
		mMonsterAnim.stop();
		checkPause();
	}
	protected void onPlaySpeedChanged()
	{
		mMonsterSpriteInstance.setPlaySpeed(StringUtility.stringToFloat(mPlaySpeed.getText()));
	}
	protected void onPlayLoopChanged()
	{
		mMonsterSpriteInstance.setLoop(mPlayLoop.getChecked());	
	}
	// 上一个图集
	protected void onLastMonsterClick(GameObject obj)
	{
		if(mResourceList.Count == 0)
		{
			return;
		}
		// 首先查找当前特效所在的下标
		setCurAtlas((mMonsterIndex - 1 + mResourceList.Count) % mResourceList.Count);
	}
	// 下一个图集
	protected void onNextMonsterClick(GameObject obj)
	{
		if (mResourceList.Count == 0)
		{
			return;
		}
		setCurAtlas((mMonsterIndex + 1) % mResourceList.Count);
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
		mResourceList.Clear();
		List<MonsterData> dataList;
		mSQLiteMonster.queryAll(out dataList);
		int count = dataList.Count;
		for(int i = 0; i < count; ++i)
		{
			MonsterAtlas effectAtlas = new MonsterAtlas();
			effectAtlas.mAtlasName = dataList[i].mResource;
			effectAtlas.mAnimSetList = new Dictionary<int, Dictionary<string, MonsterAnim>>();
			var animList = effectAtlas.mAnimSetList;
			List<MonsterFrameData> frameDataList;
			mSQLiteMonsterFrame.query(dataList[i].mID, out frameDataList);
			int frameDataCount = frameDataList.Count;
			for (int j = 0; j < frameDataCount; ++j)
			{
				MonsterAnim anim = new MonsterAnim();
				anim.mID = frameDataList[j].mID;
				anim.mAction = frameDataList[j].mAction;
				anim.mDirection = frameDataList[j].mDirection;
				anim.mTextureSet = frameDataList[j].mAction + "_dir" + frameDataList[j].mDirection;
				if(!animList.ContainsKey(anim.mDirection))
				{
					animList.Add(anim.mDirection, new Dictionary<string, MonsterAnim>());
				}
				if(!animList[anim.mDirection].ContainsKey(anim.mAction))
				{
					animList[anim.mDirection].Add(anim.mAction, anim);
				}
			}
			mResourceList.Add(effectAtlas);
		}
		mMonsterCount.setLabel("数量:0/" + mResourceList.Count);
		// 设置
		setCurAtlas(0);
	}
	protected void onLastFrameClick(GameObject obj)
	{
		if(mMonsterAnim.getPlayState() == PLAY_STATE.PS_PAUSE)
		{
			int curFrame = mMonsterAnim.getCurFrameIndex() - 1;
			MathUtility.clampMin(ref curFrame, 0);
			mMonsterAnim.setCurFrameIndex(curFrame);
			mFrameCount.setLabel("帧数:" + (mMonsterAnim.getCurFrameIndex() + 1) + "/" + mMonsterAnim.getTextureFrameCount());
		}
	}
	protected void onNextFrameClick(GameObject obj)
	{
		if (mMonsterAnim.getPlayState() == PLAY_STATE.PS_PAUSE)
		{
			int curFrame = mMonsterAnim.getCurFrameIndex() + 1;
			MathUtility.clampMax(ref curFrame, mMonsterAnim.getTextureFrameCount() - 1);
			mMonsterAnim.setCurFrameIndex(curFrame);
			mFrameCount.setLabel("帧数:" + (mMonsterAnim.getCurFrameIndex() + 1) + "/" + mMonsterAnim.getTextureFrameCount());
		}
	}
	protected void setCurAnim(int direction, int action)
	{
		mDirection = direction;
		mAction = action;
		mCurDirectionEdit.setText("" + mDirection);
		mCurActionEdit.setText(mActionList[mAction]);
		MonsterAtlas curAtlas = mResourceList[mMonsterIndex];
		MonsterAnim curAnim = curAtlas.mAnimSetList[mDirection][mActionList[mAction]];
		float playSpeed = StringUtility.stringToFloat(mPlaySpeed.getText());
		mMonsterSpriteInstance.play(curAnim.mTextureSet, findPosList(curAnim), mPlayLoop.getChecked(), playSpeed);
		mMonsterName.setText(curAtlas.mAtlasName + "/" + curAnim.mTextureSet);
		checkPause();
	}
	protected void setCurAtlas(int index)
	{
		if(index < 0 || index >= mResourceList.Count)
		{
			return;
		}
		mMonsterIndex = index;
		mCurMonsterEdit.setText(mResourceList[mMonsterIndex].mAtlasName);
		mMonsterCount.setLabel("数量:" + (mMonsterIndex + 1) + "/" + mResourceList.Count);
		UIAtlas atlas = mResourceManager.loadResource<UIAtlas>(CommonDefine.R_ATLAS_TEXTURE_ANIM_PATH + "Monster/" + mResourceList[mMonsterIndex].mAtlasName, true);
		mMonsterSpriteInstance.setAtlas(atlas);
		setCurAnim(0, 0);
	}
	protected List<Vector2> findPosList(MonsterAnim anim)
	{
		List<MonsterFrameData> frameDataList;
		mSQLiteMonsterFrame.query(anim.mID, anim.mDirection, anim.mAction, out frameDataList);
		if (frameDataList.Count != 1)
		{
			logError("monster frame error!");
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
		bool isPause = mMonsterAnim.getPlayState() == PLAY_STATE.PS_PAUSE;
		LayoutTools.ACTIVE_WINDOW(mPauseOperate, isPause);
		mFrameCount.setLabel("帧数:" + (mMonsterAnim.getCurFrameIndex() + 1) + "/" + mMonsterAnim.getTextureFrameCount());
	}
}
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using UnityEngine;

public class EffectAnim
{
	public string mTextureSet;
	public int mID;
	public string mAction;
	public int mDirection;
}

public class EffectAtlas
{
	public string mAtlasName;
	public List<EffectAnim> mAnimSetList;
}

public class ScriptEffectSpriteEditor : LayoutScript
{
	protected txNGUIEditbox mEffectName;
	protected txNGUIText mEffectCount;
	protected txNGUIButton mLastAtlas;
	protected txNGUIButton mNextAtlas;
	protected txNGUIButton mLastAnim;
	protected txNGUIButton mNextAnim;
	protected txNGUIText mAnimCount;
	protected txNGUIButton mRefreshResource;
	protected txNGUIButton mPlayButton;
	protected txNGUIButton mPauseButton;
	protected txNGUIButton mStopButton;
	protected txNGUIEditbox mPlaySpeed;
	protected txNGUICheckBox mPlayLoop;
	protected txNGUIButton mLastFrame;
	protected txNGUIButton mNextFrame;
	protected EffectSpriteInstance mEffectSpriteInstance;
	protected List<EffectAtlas> mResourceList;
	protected int mAtlasIndex;
	protected int mAnimIndex;
	protected INGUIAnimation mCurEffectNode;
	public ScriptEffectSpriteEditor(string name, GameLayout layout)
		:
		base(name, layout)
	{
		mResourceList = new List<EffectAtlas>();
		mEffectSpriteInstance = new EffectSpriteInstance(this);
		mLayout.setCheckBoxAnchor(false);
	}
	public override void assignWindow()
	{
		newObject(out mEffectName, "EffectName");
		newObject(out mEffectCount, "EffectCount");
		newObject(out mLastAtlas, "LastAtlas");
		newObject(out mNextAtlas, "NextAtlas");
		newObject(out mLastAnim, "LastAnim");
		newObject(out mNextAnim, "NextAnim");
		newObject(out mAnimCount, "AnimCount");
		newObject(out mRefreshResource, "RefreshResource");
		newObject(out mPlayButton, "PlayButton");
		newObject(out mPauseButton, "PauseButton");
		newObject(out mStopButton, "StopButton");
		newObject(out mPlaySpeed, "PlaySpeed");
		newObject(out mPlayLoop, "PlayLoop");
		newObject(out mLastFrame, "LastFrame", 0);
		newObject(out mNextFrame, "NextFrame", 0);
		instantiateObject(mRoot, "EffectSprite");
		mEffectSpriteInstance.assignWindow("EffectSprite");
	}
	public override void init()
	{
		registeBoxColliderNGUI(mPlayButton, onPlayClick);
		registeBoxColliderNGUI(mPauseButton, onPauseClick);
		registeBoxColliderNGUI(mStopButton, onStopClick);
		registeBoxColliderNGUI(mLastAtlas, onLastAtlasClick);
		registeBoxColliderNGUI(mNextAtlas, onNextAtlasClick);
		registeBoxColliderNGUI(mLastAnim, onLastAnimClick);
		registeBoxColliderNGUI(mNextAnim, onNextAnimClick);
		registeBoxColliderNGUI(mRefreshResource, onRefreshResourceClick);
		registeBoxColliderNGUI(mLastFrame, onLastFrameClick);
		registeBoxColliderNGUI(mNextFrame, onNextFrameClick);
		mPlaySpeed.setInputChangedCallback(onPlaySpeedChanged);
		mPlayLoop.setCheckChangedCallback(onPlayLoopChanged);
		mEffectSpriteInstance.init();
		mPlaySpeed.setText("0.25f");
		mPlayLoop.setChecked(true);
		mCurEffectNode = mEffectSpriteInstance.mEffectNode;
		mCurEffectNode.setSpeed(0.25f);
	}
	public override void onReset()
	{
		mEffectSpriteInstance.onReset();
		LT.ACTIVE_WINDOW(mLastFrame, false);
		LT.ACTIVE_WINDOW(mNextFrame, false);
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
		mCurEffectNode.play();
		checkPause();
	}
	protected void onPauseClick(GameObject obj)
	{
		mCurEffectNode.pause();
		checkPause();
	}
	protected void onStopClick(GameObject obj)
	{
		mCurEffectNode.stop();
		checkPause();
	}
	protected void onPlaySpeedChanged()
	{
		mEffectSpriteInstance.setPlaySpeed(stringToFloat(mPlaySpeed.getText()));
	}
	protected void onPlayLoopChanged()
	{
		mEffectSpriteInstance.setLoop(mPlayLoop.getChecked());	
	}
	// 上一个图集
	protected void onLastAtlasClick(GameObject obj)
	{
		if(mResourceList.Count == 0)
		{
			return;
		}
		// 首先查找当前特效所在的下标
		setCurAtlas((mAtlasIndex - 1 + mResourceList.Count) % mResourceList.Count);
	}
	// 下一个图集
	protected void onNextAtlasClick(GameObject obj)
	{
		if (mResourceList.Count == 0)
		{
			return;
		}
		setCurAtlas((mAtlasIndex + 1) % mResourceList.Count);
	}
	// 图集中的上一个序列帧
	protected void onLastAnimClick(GameObject obj)
	{
		setCurAnim((mAnimIndex - 1 + mResourceList[mAtlasIndex].mAnimSetList.Count) % mResourceList[mAtlasIndex].mAnimSetList.Count);
	}
	// 图集中的下一个序列帧
	protected void onNextAnimClick(GameObject obj)
	{
		setCurAnim((mAnimIndex + 1) % mResourceList[mAtlasIndex].mAnimSetList.Count);
	}
	protected void onRefreshResourceClick(GameObject obj)
	{
		// 从数据库中查找
		mResourceList.Clear();
		List<EffectData> dataList;
		mSQLiteEffect.queryAll(out dataList);
		int count = dataList.Count;
		for(int i = 0; i < count; ++i)
		{
			EffectAtlas effectAtlas = new EffectAtlas();
			effectAtlas.mAtlasName = dataList[i].mResource;
			effectAtlas.mAnimSetList = new List<EffectAnim>();
			List<EffectFrameData> frameDataList;
			mSQLiteEffectFrame.query(dataList[i].mID, out frameDataList);
			int frameDataCount = frameDataList.Count;
			for (int j = 0; j < frameDataCount; ++j)
			{
				EffectAnim anim = new EffectAnim();
				anim.mID = frameDataList[j].mID;
				anim.mAction = frameDataList[j].mAction;
				anim.mDirection = frameDataList[j].mDirection;
				anim.mTextureSet = frameDataList[j].mAction + "_dir" + frameDataList[j].mDirection;
				effectAtlas.mAnimSetList.Add(anim);
			}
			mResourceList.Add(effectAtlas);
		}
		mEffectCount.setLabel("图集数量:0/" + mResourceList.Count);
		// 设置
		setCurAtlas(0);
	}
	protected void onLastFrameClick(GameObject obj)
	{
		if(mCurEffectNode.getPlayState() == PLAY_STATE.PS_PAUSE)
		{
			int curFrame = mCurEffectNode.getCurFrameIndex() - 1;
			clampMin(ref curFrame, 0);
			mCurEffectNode.setCurFrameIndex(curFrame);
		}
	}
	protected void onNextFrameClick(GameObject obj)
	{
		if (mCurEffectNode.getPlayState() == PLAY_STATE.PS_PAUSE)
		{
			int curFrame = mCurEffectNode.getCurFrameIndex() + 1;
			clampMax(ref curFrame, mCurEffectNode.getTextureFrameCount() - 1);
			mCurEffectNode.setCurFrameIndex(curFrame);
		}
	}
	protected void setCurAnim(int index)
	{
		if (index < 0 || index >= mResourceList[mAtlasIndex].mAnimSetList.Count)
		{
			return;
		}
		mAnimIndex = index;
		EffectAtlas curAtlas = mResourceList[mAtlasIndex];
		EffectAnim curAnim = curAtlas.mAnimSetList[mAnimIndex];
		float playSpeed = stringToFloat(mPlaySpeed.getText());
		mEffectSpriteInstance.play(curAnim.mTextureSet, findPosList(curAnim), mPlayLoop.getChecked(), playSpeed);
		mEffectName.setText(curAtlas.mAtlasName + "/" + curAnim.mTextureSet);
		checkPause();
		mAnimCount.setLabel("序列帧数量:" + (mAnimIndex + 1) + "/" + curAtlas.mAnimSetList.Count);
	}
	protected void setCurAtlas(int index)
	{
		if(index < 0 || index >= mResourceList.Count)
		{
			return;
		}
		mAtlasIndex = index;
		mEffectCount.setLabel("图集数量:" + (mAtlasIndex + 1) + "/" + mResourceList.Count);
		UIAtlas atlas = mResourceManager.loadResource<UIAtlas>(CommonDefine.R_ATLAS_TEXTURE_ANIM_PATH + "Effect/Skill/" + mResourceList[mAtlasIndex].mAtlasName, true);
		mEffectSpriteInstance.setAtlas(atlas);
		setCurAnim(0);
	}
	protected List<Vector2> findPosList(EffectAnim anim)
	{
		List<EffectFrameData> effectDataList;
		mSQLiteEffectFrame.query(anim.mID, anim.mDirection, anim.mAction, out effectDataList);
		if (effectDataList.Count != 1)
		{
			logError("effect frame error!");
			return null;
		}
		List<Vector2> posList = new List<Vector2>();
		for (int j = 0; j < effectDataList[0].mFrameCount; ++j)
		{
			// 由于坐标系不一致,所以需要将Y方向偏移取反
			posList.Add(new Vector2(effectDataList[0].mPosX[j], -effectDataList[0].mPosY[j]));
		}
		return posList;
	}
	protected void checkPause()
	{
		bool isPause = mCurEffectNode.getPlayState() == PLAY_STATE.PS_PAUSE;
		LT.ACTIVE_WINDOW(mLastFrame, isPause);
		LT.ACTIVE_WINDOW(mNextFrame, isPause);
	}
}
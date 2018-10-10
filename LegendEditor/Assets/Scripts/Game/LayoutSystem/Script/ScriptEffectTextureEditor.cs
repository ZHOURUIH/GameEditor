using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using UnityEngine;

public class ScriptEffectTextureEditor : LayoutScript
{
	protected txNGUIEditbox mEffectName;
	protected txNGUIText mEffectCount;
	protected txNGUIButton mLastEffect;
	protected txNGUIButton mNextEffect;
	protected txNGUIButton mLastFolder;
	protected txNGUIButton mNextFolder;
	protected txNGUIButton mRefreshResource;
	protected txNGUIButton mPlayButton;
	protected txNGUIButton mPauseButton;
	protected txNGUIButton mStopButton;
	protected txNGUIEditbox mPlaySpeed;
	protected txNGUICheckBox mPlayLoop;
	protected txNGUIButton mLastFrame;
	protected txNGUIButton mNextFrame;
	protected EffectTextureInstance mEffectTextureInstance;
	protected List<string> mResourceList;
	protected int mEffectIndex;
	protected bool mIsAtlasResource;
	protected INGUIAnimation mCurEffectNode;
	public ScriptEffectTextureEditor(string name, GameLayout layout)
		:
		base(name, layout)
	{
		mResourceList = new List<string>();
		mEffectTextureInstance = new EffectTextureInstance(this);
		mLayout.setCheckBoxAnchor(false);
	}
	public override void assignWindow()
	{
		newObject(out mEffectName, "EffectName");
		newObject(out mEffectCount, "EffectCount");
		newObject(out mLastEffect, "LastEffect");
		newObject(out mNextEffect, "NextEffect");
		newObject(out mLastFolder, "LastFolder");
		newObject(out mNextFolder, "NextFolder");
		newObject(out mRefreshResource, "RefreshResource");
		newObject(out mPlayButton, "PlayButton");
		newObject(out mPauseButton, "PauseButton");
		newObject(out mStopButton, "StopButton");
		newObject(out mPlaySpeed, "PlaySpeed");
		newObject(out mPlayLoop, "PlayLoop");
		newObject(out mLastFrame, "LastFrame", 0);
		newObject(out mNextFrame, "NextFrame", 0);
		instantiateObject(mRoot, "EffectTexture");
		mEffectTextureInstance.assignWindow("EffectTexture");
	}
	public override void init()
	{
		registeBoxColliderNGUI(mPlayButton, onPlayClick);
		registeBoxColliderNGUI(mPauseButton, onPauseClick);
		registeBoxColliderNGUI(mStopButton, onStopClick);
		registeBoxColliderNGUI(mLastEffect, onLastEffectClick);
		registeBoxColliderNGUI(mNextEffect, onNextEffectClick);
		registeBoxColliderNGUI(mLastFolder, onLastFolderClick);
		registeBoxColliderNGUI(mNextFolder, onNextFolderClick);
		registeBoxColliderNGUI(mRefreshResource, onRefreshResourceClick);
		registeBoxColliderNGUI(mLastFrame, onLastFrameClick);
		registeBoxColliderNGUI(mNextFrame, onNextFrameClick);
		mEffectName.setInputChangedCallback(onEffectNameChanged);
		mPlaySpeed.setInputChangedCallback(onPlaySpeedChanged);
		mPlayLoop.setCheckChangedCallback(onPlayLoopChanged);
		mEffectTextureInstance.init();
		mPlaySpeed.setText("0.25f");
		mPlayLoop.setChecked(true);
		mCurEffectNode = mEffectTextureInstance.mEffectNode;
	}
	public override void onReset()
	{
		mEffectTextureInstance.onReset();
		LayoutTools.ACTIVE_WINDOW(mLastFrame, false);
		LayoutTools.ACTIVE_WINDOW(mNextFrame, false);
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
	protected void checkPause()
	{
		bool isPause = mCurEffectNode.getPlayState() == PLAY_STATE.PS_PAUSE;
		LayoutTools.ACTIVE_WINDOW(mLastFrame, isPause);
		LayoutTools.ACTIVE_WINDOW(mNextFrame, isPause);
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
		mEffectTextureInstance.setPlaySpeed(StringUtility.stringToFloat(mPlaySpeed.getText()));
	}
	protected void onPlayLoopChanged()
	{
		mEffectTextureInstance.setLoop(mPlayLoop.getChecked());
	}
	protected void onEffectNameChanged()
	{
		string effectName = mEffectName.getText();
		int count = mResourceList.Count;
		for (int i = 0; i < count; ++i)
		{
			if(mResourceList[i] == effectName)
			{
				setCurEffect(i);
				break;
			}
		}
	}
	protected void onLastEffectClick(GameObject obj)
	{
		if(mResourceList.Count == 0)
		{
			return;
		}
		// 首先查找当前特效所在的下标
		setCurEffect((mEffectIndex - 1 + mResourceList.Count) % mResourceList.Count);
	}
	protected void onNextEffectClick(GameObject obj)
	{
		if (mResourceList.Count == 0)
		{
			return;
		}
		setCurEffect((mEffectIndex + 1) % mResourceList.Count);
	}
	protected void onLastFolderClick(GameObject obj)
	{
		int curIndex = mEffectIndex;
		string curFolder = StringUtility.getFilePath(mResourceList[mEffectIndex]);
		while (true)
		{
			curIndex = (curIndex - 1 + mResourceList.Count) % mResourceList.Count;
			// 查找完了则退出循环
			if (curIndex == mEffectIndex)
			{
				break;
			}
			// 查找下一个资源的目录,如果目录不同,则退出循环
			string folder = StringUtility.getFilePath(mResourceList[curIndex]);
			if(folder != curFolder)
			{
				break;
			}
		}
		setCurEffect(curIndex);
	}
	protected void onNextFolderClick(GameObject obj)
	{
		int curIndex = mEffectIndex;
		string curFolder = StringUtility.getFilePath(mResourceList[mEffectIndex]);
		while (true)
		{
			curIndex = (curIndex + 1) % mResourceList.Count;
			// 查找完了则退出循环
			if (curIndex == mEffectIndex)
			{
				break;
			}
			// 查找下一个资源的目录,如果目录不同,则退出循环
			string folder = StringUtility.getFilePath(mResourceList[curIndex]);
			if (folder != curFolder)
			{
				break;
			}
		}
		setCurEffect(curIndex);
	}
	protected void onRefreshResourceClick(GameObject obj)
	{
		mResourceList.Clear();
		FileUtility.findDirectory(CommonDefine.F_TEXTURE_ANIM_PATH + "Effect", ref mResourceList);
		// 去除路径前缀
		int count = mResourceList.Count;
		for(int i = 0; i < count; ++i)
		{
			mResourceList[i] = mResourceList[i].Substring(CommonDefine.F_TEXTURE_ANIM_PATH.Length);
		}
		// 移除不直接包含图片的文件夹
		List<int> removeItem = new List<int>();
		count = mResourceList.Count;
		for (int i = 0; i < count; ++i)
		{
			List<string> temp = new List<string>();
			FileUtility.findFiles(CommonDefine.F_TEXTURE_ANIM_PATH + mResourceList[i], ref temp, ".png", false);
			if(temp.Count == 0)
			{
				removeItem.Add(i);
			}
		}
		// 应该从后往前删除
		int removeCount = removeItem.Count;
		for(int i = 0; i < removeCount; ++i)
		{
			mResourceList.RemoveAt(removeItem[removeCount - i - 1]);
		}
		GameUtility.sortByFileNumber(mResourceList);
		mEffectCount.setLabel("数量:0/" + mResourceList.Count);
		// 设置
		setCurEffect(0);
	}
	protected void onLastFrameClick(GameObject obj)
	{
		if(mCurEffectNode.getPlayState() == PLAY_STATE.PS_PAUSE)
		{
			int curFrame = mCurEffectNode.getCurFrameIndex() - 1;
			MathUtility.clampMin(ref curFrame, 0);
			mCurEffectNode.setCurFrameIndex(curFrame);
		}
	}
	protected void onNextFrameClick(GameObject obj)
	{
		if (mCurEffectNode.getPlayState() == PLAY_STATE.PS_PAUSE)
		{
			int curFrame = mCurEffectNode.getCurFrameIndex() + 1;
			MathUtility.clampMax(ref curFrame, mCurEffectNode.getTextureFrameCount() - 1);
			mCurEffectNode.setCurFrameIndex(curFrame);
		}
	}
	protected void setCurEffect(int index)
	{
		if(index < 0 || index >= mResourceList.Count)
		{
			return;
		}
		mEffectIndex = index;
		mEffectCount.setLabel("数量:" + (mEffectIndex + 1) + "/" + mResourceList.Count);
		string textureSetName = StringUtility.getFileName(mResourceList[mEffectIndex]);
		string subPath = mResourceList[mEffectIndex].Substring(0, mResourceList[mEffectIndex].Length - textureSetName.Length);
		float playSpeed = StringUtility.stringToFloat(mPlaySpeed.getText());
		mEffectTextureInstance.play(textureSetName, subPath, findPosList(mResourceList[mEffectIndex]), mPlayLoop.getChecked(), playSpeed);
		if (mEffectName.getText() != mResourceList[mEffectIndex])
		{
			mEffectName.setText(mResourceList[mEffectIndex]);
		}
		checkPause();
	}
	protected List<Vector2> findPosList(string path)
	{
		if(mIsAtlasResource)
		{
			path = CommonDefine.F_ATLAS_TEXTURE_ANIM_PATH + path;
			return null;
		}
		else
		{
			path = CommonDefine.F_TEXTURE_ANIM_PATH + path;
			List<string> fileList = new List<string>();
			FileUtility.findFiles(path, ref fileList, ".txt", false);
			int count = fileList.Count;
			if (count == 0)
			{
				return null;
			}
			List<Vector2> posList = new List<Vector2>();
			for(int i = 0; i < count; ++i)
			{
				string posStr = FileUtility.openTxtFile(fileList[i]);
				// 由于坐标系不一致,所以需要将Y方向偏移取反
				Vector2 pos = StringUtility.stringToVector2(posStr);
				pos.y = -pos.y;
				posList.Add(pos);
			}
			return posList;
		}
	}
}
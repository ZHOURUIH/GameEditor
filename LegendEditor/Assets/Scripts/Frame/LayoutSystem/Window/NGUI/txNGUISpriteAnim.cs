using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using UnityEngine;

public class txNGUISpriteAnim : txNGUISprite
{
	protected List<string> mTextureNameList;
	protected string		mTextureSetName;
	protected SpriteAnimCallBack mPlayEndCallback;  // 一个序列播放完时的回调函数,只在非循环播放状态下有效
	protected SpriteAnimCallBack mPlayingCallback;  // 一个序列正在播放时的回调函数
	public AnimControl mControl;
	public txNGUISpriteAnim()
	{
		mType = UI_TYPE.UT_NGUI_SPRITE_ANIM;
		mControl = new AnimControl();
		mTextureNameList = new List<string>();
	}
	public override void init(GameLayout layout, GameObject go, txUIObject parent)
	{
		base.init(layout, go, parent);
		string spriteName = getSpriteName();
		string textureSetName = spriteName.Substring(0, spriteName.LastIndexOf('_'));
		setTextureSet(textureSetName);
	}
	public override void update(float elapsedTime)
	{
		base.update(elapsedTime);
		if (mTextureNameList.Count == 0)
		{
			setSpriteName("");
		}
		mControl.update(elapsedTime);
	}
	public string getTextureSetName() { return mTextureSetName; }
	public int getTextureFrameCount() { return mTextureNameList.Count; }
	public void setTextureSet(string textureSetName)
	{
		mTextureNameList.Clear();
		mTextureSetName = textureSetName;
		if (mSprite != null && mSprite.atlas != null)
		{
			Dictionary<string, UISpriteData> nameList = new Dictionary<string,UISpriteData>();
			List<UISpriteData> sprites = mSprite.atlas.spriteList;
			for (int i = 0, imax = sprites.Count; i < imax; ++i)
			{
				if (sprites[i].name.StartsWith(mTextureSetName))
				{
					nameList.Add(sprites[i].name, sprites[i]);
				}
			}
			int index = 0;
			while(true)
			{
				string name = mTextureSetName + "_" + StringUtility.intToString(++index);
				if(nameList.ContainsKey(name))
				{
					mTextureNameList.Add(name);
				}
				else
				{
					break;
				}
			}
		}
		mControl.setFrameCount(getTextureFrameCount());
	}
	public void setPlayEndCallback(SpriteAnimCallBack callback)
	{
		// 设置前先调用之前的回调通知被中断了
		SpriteAnimCallBack curCallback = mPlayEndCallback;
		mPlayEndCallback = null;
		// 如果回调函数当前不为空,则是中断了更新
		if (curCallback != null)
		{
			curCallback(this, null, true);
		}
		mPlayEndCallback = callback;
	}
	public void setPlayingCallback(SpriteAnimCallBack callback)
	{
		mPlayingCallback = callback;
	}
	//--------------------------------------------------------------------------------------------------------
	protected void onPlaying(AnimControl control, int frame, bool isPlaying)
	{
		mSprite.spriteName = mTextureNameList[mControl.getCurFrameIndex()];
		if (mPlayingCallback != null)
		{
			mPlayingCallback(this, null, false);
		}
	}
	protected void onPlayEnd(AnimControl control, bool isBreak)
	{
		// 播放完毕后根据是否重置下标来判断是否自动隐藏
		if (mControl.getAutoResetIndex())
		{
			setActive(false);
		}
		if (mPlayEndCallback != null)
		{
			mPlayEndCallback(this, null, isBreak);
		}
	}
}
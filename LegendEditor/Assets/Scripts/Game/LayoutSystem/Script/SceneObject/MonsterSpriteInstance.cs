using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using UnityEngine;

public class MonsterSpriteInstance : GameBase
{
	protected LayoutScript mScript;
	public txUIObject mMonsterRoot;
	public txUIObject mFrameParent; // 用来调整角色图片的坐标系
	public txNGUISpriteAnim mFrame;
	public MonsterSpriteInstance(LayoutScript script)
	{
		mScript = script;
	}
	public void assignWindow(string rootName)
	{
		mScript.newObject(out mMonsterRoot, rootName, 1);
		mScript.newObject(out mFrameParent, mMonsterRoot, "FrameParent");
		mScript.newObject(out mFrame, mFrameParent, "Frame");
	}
	public void init()
	{
		mFrame.setAutoHide(false);
		mFrame.setUseTextureSize(true);
	}
	public void onReset()
	{
		;
	}
	public void setPlaySpeed(float speed)
	{
		mFrame.setSpeed(speed);
	}
	public void setLoop(bool loop)
	{
		mFrame.setLoop(loop ? LOOP_MODE.LM_LOOP : LOOP_MODE.LM_ONCE);
	}
	public void setTexturePosList(List<Vector2> posList)
	{
		mFrame.setTexturePosList(posList);
	}
	public string getTextureSetFromFrame(string frameName)
	{
		return frameName.Substring(0, frameName.LastIndexOf("_"));
	}
	public void setAtlas(UIAtlas atlas)
	{
		mFrame.setAtlas(atlas);
	}
	public void play(string monster, List<Vector2> posList, bool forcePlay = false)
	{
		play(monster, posList, mFrame.getLoop() == LOOP_MODE.LM_LOOP, mFrame.getSpeed(), forcePlay);
	}
	public void play(string monster, List<Vector2> posList, bool loop, float animSpeed, bool forcePlay = false)
	{
		// 如果动作名相同,并且不是强制重新播放,则不播放动作
		if(mFrame.getTextureSet() == monster && mFrame.getPlayState() == PLAY_STATE.PS_PLAY && !forcePlay)
		{
			return;
		}
		mFrame.setTextureSet(monster);
		mFrame.stop(true, false);
		mFrame.play();
		setTexturePosList(posList);
		setLoop(loop);
		setPlaySpeed(animSpeed);
		mFrame.addPlayingCallback(onAnimationPlaying);
	}
	//-------------------------------------------------------------------------------------------------------
	protected void onAnimationPlaying(INGUIAnimation window, bool isBreak)
	{
		// 由于原坐标系是以图片左上角为原点,unity是以图片中心为原点,所以需要根据图片大小再做父节点偏移
		Vector2 parentPos = new Vector2(mFrame.getWindowSize().x / 2.0f, -mFrame.getWindowSize().y / 2.0f);
		LT.MOVE_WINDOW(mFrameParent, parentPos);
	}
	protected void onAnimationPlayEnd(INGUIAnimation window, bool isBreak)
	{
		if (isBreak)
		{
			return;
		}
		mFrame.stop(true, false);
	}
}
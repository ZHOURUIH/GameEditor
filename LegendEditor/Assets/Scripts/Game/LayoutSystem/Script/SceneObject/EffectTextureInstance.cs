﻿using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using UnityEngine;

public class EffectTextureInstance : GameBase
{
	protected LayoutScript mScript;
	public txUIObject mEffectRoot;
	public txUIObject mParentNode; // 用来调整角色图片的坐标系
	public txNGUITextureAnim mEffectNode;
	public EffectTextureInstance(LayoutScript script)
	{
		mScript = script;
	}
	public void assignWindow(string rootName)
	{
		mScript.newObject(out mEffectRoot, rootName, 1);
		mScript.newObject(out mParentNode, mEffectRoot, "ParentNode");
		mScript.newObject(out mEffectNode, mParentNode, "EffectNode");
	}
	public void init()
	{
		mEffectNode.setAutoHide(false);
		mEffectNode.setUseTextureSize(true);
	}
	public void onReset()
	{
		;
	}
	public void setPlaySpeed(float speed)
	{
		mEffectNode.setSpeed(speed);
	}
	public void setLoop(bool loop)
	{
		mEffectNode.setLoop(loop ? LOOP_MODE.LM_LOOP : LOOP_MODE.LM_ONCE);
	}
	public void setTexturePosList(List<Vector2> posList)
	{
		mEffectNode.setTexturePosList(posList);
	}
	public void play(string effect, List<Vector2> posList, bool forcePlay = false)
	{
		play(effect, mEffectNode.getSubPath(), posList, forcePlay);
	}
	public void play(string effect, string subPath, List<Vector2> posList, bool forcePlay = false)
	{
		play(effect, subPath, posList, mEffectNode.getLoop() == LOOP_MODE.LM_LOOP, mEffectNode.getSpeed(), forcePlay);
	}
	public void play(string effect, string subPath, List<Vector2> posList, bool loop, float animSpeed, bool forcePlay = false)
	{
		// 如果动作名相同,并且不是强制重新播放,则不播放动作
		if(mEffectNode.getTextureSet() == effect && mEffectNode.getSubPath() == subPath && mEffectNode.getPlayState() == PLAY_STATE.PS_PLAY && !forcePlay)
		{
			logInfo("play same animation!");
			return;
		}
		mEffectNode.setTextureSet(effect, subPath);
		mEffectNode.stop(true, false);
		mEffectNode.play();
		setTexturePosList(posList);
		setLoop(loop);
		setPlaySpeed(animSpeed);
		mEffectNode.addPlayingCallback(onAnimationPlaying);
	}
	public void move(Vector3 dest)
	{
		LT.MOVE_WINDOW(mEffectRoot, dest);
	}
	//-------------------------------------------------------------------------------------------------------
	protected void onAnimationPlaying(INGUIAnimation window, bool isBreak)
	{
		// 由于原坐标系是以图片左上角为原点,unity是以图片中心为原点,所以需要根据图片大小再做父节点偏移
		Vector2 parentPos = new Vector2(mEffectNode.getWindowSize().x / 2.0f, -mEffectNode.getWindowSize().y / 2.0f);
		LT.MOVE_WINDOW(mParentNode, parentPos);
	}
	protected void onAnimationPlayEnd(INGUIAnimation window, bool isBreak)
	{
		if (isBreak)
		{
			return;
		}
		mEffectNode.stop(true, false);
	}
}
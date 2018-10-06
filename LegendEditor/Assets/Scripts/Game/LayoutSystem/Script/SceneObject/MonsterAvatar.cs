//using System;
//using System.Collections.Generic;
//using System.Linq;
//using System.Text;
//using UnityEngine;

//public enum ATTACK_STATE
//{
//	AS_READY,
//	AS_ATTACKING,
//	AS_DONE,
//}

//public class MonsterAvatar
//{
//	public LayoutScript mScript;
//	public txUIObject mMonsterRoot;	// 怪物的根节点
//	public txNGUITextureAnim mFrame;
//	public int mMonsterGUID;	// 怪物实例ID
//	public int mTypeGUID;       // 怪物类型ID
//	public string mMonsterLabel;
//	protected ActionEnd mActionEnd;
//	protected Actioning mActioning;
//	protected List<Vector2> mPosList;
//	public MonsterAvatar(LayoutScript script)
//	{
//		mScript = script;
//	}
//	public void assignWindow(string rootName)
//	{
//		mScript.newObject(out mMonsterRoot, rootName, 1);
//		mScript.newObject(out mFrame, mMonsterRoot, "Frame");
//	}	
//	public void init(int instanceID, int typeID, string label)
//	{
//		mMonsterGUID = instanceID;
//		mTypeGUID = typeID;
//		mMonsterLabel = label;
//	}
//	public void onReset()
//	{
//		;
//	}
//	public void play(MonsterAnimInfo info, ActionEnd actionEnd, Actioning actioning, bool loop, float animSpeed, bool forcePlay = false)
//	{
//		// 如果动作名相同,并且不是强制重新播放,则不播放动作
//		if (mFrame.getTextureSet() == info.mMonsterAnim && !forcePlay)
//		{
//			return;
//		}
//		mPosList = info.mMonsterPos;
//		mActionEnd = actionEnd;
//		mActioning = actioning;
//		mFrame.setTextureSet(info.mMonsterAnim);
//		mFrame.stop(true, false);
//		mFrame.addPlayingCallback(onAnimationPlaying);
//		mFrame.addPlayEndCallback(onAnimationPlayEnd);
//		mFrame.play();
//		if(animSpeed > 0.0f)
//		{
//			mFrame.setInterval(0.03f / animSpeed);
//		}
//		mFrame.setLoop(loop ? LOOP_MODE.LM_LOOP : LOOP_MODE.LM_ONCE);
//	}
//	public void setHP(int hp)
//	{
//		;
//	}
//	public void setMaxHP(int maxHP)
//	{
//		;
//	}
//	public void setLevel(int level)
//	{
//		;
//	}
//	public void setPosition(Vector2 pos)
//	{
//		;
//	}
//	public void move(Vector3 dest)
//	{
//		LayoutTools.MOVE_WINDOW(mMonsterRoot, dest);
//	}
//	//-----------------------------------------------------------------------------------
//	protected void onAnimationPlaying(INGUIAnimation window, bool isBreak)
//	{
//		int curIndex = mFrame.getCurFrameIndex();
//		LayoutTools.MOVE_WINDOW(mFrame, mPosList[curIndex]);
//		if (mActioning != null)
//		{
//			mActioning(curIndex);
//		}
//	}
//	protected void onAnimationPlayEnd(INGUIAnimation window, bool isBreak)
//	{
//		if (mActionEnd != null)
//		{
//			mActionEnd(isBreak);
//		}
//		if (isBreak)
//		{
//			return;
//		}
//		mFrame.stop(true, false);
//	}
//}
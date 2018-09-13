using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using UnityEngine;

public enum ATTACK_STATE
{
	AS_READY,
	AS_ATTACKING,
	AS_DONE,
}

public class MonsterAvatar
{
	public ScriptScene mScript;
	public txUIObject mMonsterRoot;	// 怪物的根节点
	public txNGUITextureAnim mFrame;
	public int mMonsterGUID;	// 怪物实例ID
	public int mTypeGUID;       // 怪物类型ID
	public string mMonsterLabel;
	public MonsterAvatar(ScriptScene script)
	{
		mScript = script;
	}
	public void assignWindow(string rootName)
	{
		mScript.newObject(out mMonsterRoot, rootName, 1);
		mScript.newObject(out mFrame, mMonsterRoot, "Frame");
	}	
	public void init(int instanceID, int typeID, string label)
	{
		mMonsterGUID = instanceID;
		mTypeGUID = typeID;
		mMonsterLabel = label;
	}
	public void onReset()
	{
		;
	}
	public void play(HumanAnimInfo info, ActionEnd actionEnd, Actioning actioning, bool loop)
	{
		mFrame.mControl.stop();
		mFrame.mControl.play();
	}
	public void setHP(int hp)
	{
		;
	}
	public void setMaxHP(int maxHP)
	{
		;
	}
	public void setLevel(int level)
	{
		;
	}
	public void setPosition(Vector2 pos)
	{
		;
	}
	public void setDirection(int direction)
	{
		;
	}
}
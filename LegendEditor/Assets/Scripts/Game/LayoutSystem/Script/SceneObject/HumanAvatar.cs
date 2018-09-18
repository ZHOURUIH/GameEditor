using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using UnityEngine;

public delegate void ActionEnd(bool isBreak);
public delegate void Actioning(int frame);
public enum HUMAN_ACTION
{
	HA_ATTACK,
	HA_DEATH,
	HA_HIT,
	HA_RUN,
	HA_SKILL,
	HA_STAND,
}

public class HumanAvatar
{
	protected ScriptObjectEditor mScript;
	public txUIObject mHumanRoot;
	public txNGUITextureAnim mHuman;
	public txNGUITextureAnim mWeapon;
	public WeaponHit mWeaponHit;
	public BoxCollider mWeaponBoxCollider;
	protected ActionEnd mActionEnd;
	protected Actioning mActioning;
	protected List<string> mHumanFrameList;
	protected List<string> mWeaponFrameList;
	protected List<Vector2> mWeaponPosList;
	protected List<Vector2> mHumanPosList;
	protected int mCharGUID;
	public HumanAvatar(ScriptObjectEditor script)
	{
		mScript = script;
	}
	public void assignWindow(string rootName)
	{
		mScript.newObject(out mHumanRoot, rootName, 1);
		mScript.newObject(out mHuman, mHumanRoot, "Human");
		mScript.newObject(out mWeapon, mHumanRoot, "Weapon");
	}
	public void init(int guid)
	{
		mCharGUID = guid;
		mHuman.mControl.setAutoHide(false);
		mHuman.mControl.setInterval(0.1f);
		mWeaponHit = mWeapon.mObject.GetComponent<WeaponHit>();
		mWeaponBoxCollider = mWeapon.mObject.GetComponent<BoxCollider>();
	}
	public void onReset()
	{
		;
	}
	public void play(HumanAnimInfo info, ActionEnd actionEnd, Actioning actioning, CharacterControlMyself control, bool loop)
	{
		mHumanFrameList = info.mHumanFrame;
		mWeaponFrameList = info.mWeaponFrame;
		mWeaponPosList = info.mWeaponPos;
		mHumanPosList = info.mHumanPos;
		mActionEnd = actionEnd;
		mActioning = actioning;
		mHuman.mControl.stop(true, false);
		mHuman.setPlayingCallback(onAnimationPlaying);
		mHuman.setPlayEndCallback(onAnimationPlayEnd);
		mHuman.mControl.play();
		mWeaponHit.mController = control;
	}
	public void move(Vector3 delta)
	{
		LayoutTools.MOVE_WINDOW(mHumanRoot, mHumanRoot.getPosition() + delta);
	}
	public void enableWeaponBoxCollider(bool enable)
	{
		mWeaponBoxCollider.enabled = enable;
		if (mWeaponBoxCollider.enabled)
		{
			mWeaponBoxCollider.size = mWeapon.getWindowSize();
		}
	}
	//-------------------------------------------------------------------------------------------------------
	protected void onAnimationPlaying(txNGUITextureAnim window, object userData, bool isBreak)
	{
		int curIndex = mHuman.mControl.getCurFrameIndex();
		// 设置当前帧
		mWeapon.mControl.setCurFrameIndex(curIndex);
		LayoutTools.MOVE_WINDOW(mWeapon, mWeaponPosList[curIndex]);
		LayoutTools.MOVE_WINDOW(mHuman, mHumanPosList[curIndex]);
		if (mActioning != null)
		{
			mActioning(curIndex);
		}
	}
	protected void onAnimationPlayEnd(txNGUITextureAnim window, object userData, bool isBreak)
	{
		if(mActionEnd != null)
		{
			mActionEnd(isBreak);
		}
		if (isBreak)
		{
			return;
		}
		mHuman.mControl.stop(true, false);
	}
}
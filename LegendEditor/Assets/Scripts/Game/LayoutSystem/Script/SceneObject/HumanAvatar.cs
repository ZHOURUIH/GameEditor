//using System;
//using System.Collections.Generic;
//using System.Linq;
//using System.Text;
//using UnityEngine;

//public delegate void ActionEnd(bool isBreak);
//public delegate void Actioning(int frame);

//public class HumanAvatar
//{
//	protected LayoutScript mScript;
//	public txUIObject mHumanRoot;
//	public txUIObject mHumanParent; // 用来调整角色图片的坐标系
//	public txUIObject mWeaponParent; // 用来调整武器图片的坐标系
//	public txNGUISpriteAnim mHuman;
//	public txNGUISpriteAnim mWeapon;
//	public WeaponHit mWeaponHit;
//	public BoxCollider mWeaponBoxCollider;
//	protected ActionEnd mActionEnd;
//	protected Actioning mActioning;
//	protected List<Vector2> mWeaponPosList;
//	protected List<Vector2> mHumanPosList;
//	protected int mCharGUID;
//	public HumanAvatar(LayoutScript script)
//	{
//		mScript = script;
//	}
//	public void assignWindow(string rootName)
//	{
//		mScript.newObject(out mHumanRoot, rootName, 1);
//		mScript.newObject(out mHumanParent, mHumanRoot, "HumanParent", 1);
//		mScript.newObject(out mWeaponParent, mHumanRoot, "WeaponParent", 1);
//		mScript.newObject(out mHuman, mHumanParent, "Human");
//		mScript.newObject(out mWeapon, mWeaponParent, "Weapon");
//	}
//	public void init(int guid)
//	{
//		mCharGUID = guid;
//		mHuman.setAutoHide(false);
//		mHuman.setUseTextureSize(true);
//		mWeapon.setUseTextureSize(true);
//		mWeaponHit = mWeapon.mObject.GetComponent<WeaponHit>();
//		mWeaponBoxCollider = mWeapon.mObject.GetComponent<BoxCollider>();
//	}
//	public void onReset()
//	{
//		;
//	}
//	public void play(HumanAnimInfo info, ActionEnd actionEnd, Actioning actioning, bool loop, float animSpeed, bool forcePlay = false)
//	{
//		// 如果动作名相同,并且不是强制重新播放,则不播放动作
//		if(mHuman.getTextureSet() == info.mHumanAnim && !forcePlay)
//		{
//			return;
//		}
//		mWeaponPosList = info.mWeaponPos;
//		mHumanPosList = info.mHumanPos;
//		mActionEnd = actionEnd;
//		mActioning = actioning;
//		mHuman.setTextureSet(info.mHumanAnim);
//		mHuman.stop(true, false);
//		mHuman.addPlayingCallback(onAnimationPlaying);
//		mHuman.addPlayEndCallback(onAnimationPlayEnd);
//		mHuman.play();
//		mHuman.setLoop(loop ? LOOP_MODE.LM_LOOP : LOOP_MODE.LM_ONCE);
//		if(animSpeed > 0.0f)
//		{
//			mHuman.setInterval(0.033f / animSpeed);
//		}
//		mWeapon.setTextureSet(info.mWeaponAnim);
//		// 当方向大于4时,需要将武器深度设置到角色下面
//		if(info.mDirection > 4)
//		{
//			mHuman.setDepth(2);
//			mWeapon.setDepth(1);
//		}
//		else
//		{
//			mHuman.setDepth(1);
//			mWeapon.setDepth(2);
//		}
//	}
//	public void move(Vector3 dest)
//	{
//		LayoutTools.MOVE_WINDOW(mHumanRoot, dest);
//	}
//	public void enableWeaponBoxCollider(bool enable)
//	{
//		mWeaponBoxCollider.enabled = enable;
//		if (mWeaponBoxCollider.enabled)
//		{
//			mWeaponBoxCollider.size = mWeapon.getWindowSize();
//		}
//	}
//	public txNGUISpriteAnim getHuman() { return mHuman; }
//	public txNGUISpriteAnim getWeapon() { return mWeapon; }
//	//-------------------------------------------------------------------------------------------------------
//	protected void onAnimationPlaying(INGUIAnimation window, bool isBreak)
//	{
//		// 根据角色当前帧设置武器当前帧
//		int curIndex = mHuman.getCurFrameIndex();
//		mWeapon.setCurFrameIndex(curIndex);
//		// 由于坐标系不一致,所以需要将Y方向偏移取反
//		// 由于原坐标系是以图片左上角为原点,unity是以图片中心为原点,所以需要根据图片大小再做父节点偏移
//		Vector2 newWeaponPos = new Vector2(mWeaponPosList[curIndex].x, -mWeaponPosList[curIndex].y);
//		Vector2 weaponParentPos = new Vector2(mWeapon.getWindowSize().x / 2.0f, -mWeapon.getWindowSize().y / 2.0f);
//		LayoutTools.MOVE_WINDOW(mWeaponParent, weaponParentPos);
//		LayoutTools.MOVE_WINDOW(mWeapon, newWeaponPos);
//		Vector2 newHumanPos = new Vector2(mHumanPosList[curIndex].x, -mHumanPosList[curIndex].y);
//		Vector2 humanParentPos = new Vector2(mHuman.getWindowSize().x / 2.0f, -mHuman.getWindowSize().y / 2.0f);
//		LayoutTools.MOVE_WINDOW(mHumanParent, humanParentPos);
//		LayoutTools.MOVE_WINDOW(mHuman, newHumanPos);
//		if (mActioning != null)
//		{
//			mActioning(curIndex);
//		}
//	}
//	protected void onAnimationPlayEnd(INGUIAnimation window, bool isBreak)
//	{
//		if(mActionEnd != null)
//		{
//			mActionEnd(isBreak);
//		}
//		if (isBreak)
//		{
//			return;
//		}
//		mHuman.stop(true, false);
//	}
//}
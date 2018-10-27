using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using UnityEngine;

public class HumanSpriteInstance : GameBase
{
	protected LayoutScript mScript;
	public txUIObject mHumanRoot;
	public txUIObject mClothParent; // 用来调整角色图片的坐标系
	public txNGUISpriteAnim mCloth;
	public txUIObject mWeaponParent; // 用来调整角色图片的坐标系
	public txNGUISpriteAnim mWeapon;
	public HumanSpriteInstance(LayoutScript script)
	{
		mScript = script;
	}
	public void assignWindow(string rootName)
	{
		mScript.newObject(out mHumanRoot, rootName, 1);
		mScript.newObject(out mClothParent, mHumanRoot, "ClothParent");
		mScript.newObject(out mCloth, mClothParent, "Cloth");
		mScript.newObject(out mWeaponParent, mHumanRoot, "WeaponParent");
		mScript.newObject(out mWeapon, mWeaponParent, "Weapon");
	}
	public void init()
	{
		mCloth.setAutoHide(false);
		mCloth.setUseTextureSize(true);
		mWeapon.setAutoHide(false);
		mWeapon.setUseTextureSize(true);
	}
	public void onReset()
	{
		;
	}
	public void setPlaySpeed(float speed)
	{
		mCloth.setSpeed(speed);
	}
	public void setLoop(bool loop)
	{
		mCloth.setLoop(loop ? LOOP_MODE.LM_LOOP : LOOP_MODE.LM_ONCE);
	}
	public void setClothTexturePosList(List<Vector2> clothPosList)
	{
		mCloth.setTexturePosList(clothPosList);
	}
	public void setWeaponTexturePosList(List<Vector2> weaponPosList)
	{
		mWeapon.setTexturePosList(weaponPosList);
	}
	public string getTextureSetFromFrame(string frameName)
	{
		return frameName.Substring(0, frameName.LastIndexOf("_"));
	}
	public void setClothAtlas(UIAtlas clothAtlas)
	{
		mCloth.setAtlas(clothAtlas);
	}
	public void setWeaponAtlas(UIAtlas weaponAtlas)
	{
		mWeapon.setAtlas(weaponAtlas);
	}
	public void play(string animSet, bool forcePlay = false)
	{
		play(animSet, mCloth.getLoop() == LOOP_MODE.LM_LOOP, mCloth.getSpeed(), forcePlay);
	}
	public void play(string animSet, bool loop, float animSpeed, bool forcePlay = false)
	{
		// 如果动作名相同,并且不是强制重新播放,则不播放动作
		if(mCloth.getTextureSet() == animSet && mCloth.getPlayState() == PLAY_STATE.PS_PLAY && !forcePlay)
		{
			return;
		}
		mCloth.setTextureSet(animSet);
		mWeapon.setTextureSet(animSet);
		mCloth.stop(true, false);
		mCloth.play();
		setLoop(loop);
		setPlaySpeed(animSpeed);
		mCloth.addPlayingCallback(onAnimationPlaying);
		// 当方向大于4时,需要将武器深度设置到角色下面
		int direction = getLastNumber(animSet);
		if (direction <= 4 && direction > 0)
		{
			mCloth.setDepth(1);
			mWeapon.setDepth(2);
		}
		else
		{
			mCloth.setDepth(2);
			mWeapon.setDepth(1);
		}
	}
	//-------------------------------------------------------------------------------------------------------
	protected void onAnimationPlaying(INGUIAnimation window, bool isBreak)
	{
		mWeapon.setCurFrameIndex(mCloth.getCurFrameIndex());
		// 由于原坐标系是以图片左上角为原点,unity是以图片中心为原点,所以需要根据图片大小再做父节点偏移
		Vector2 clothParentPos = new Vector2(mCloth.getWindowSize().x / 2.0f, -mCloth.getWindowSize().y / 2.0f);
		LT.MOVE_WINDOW(mClothParent, clothParentPos);
		Vector2 weaponParentPos = new Vector2(mWeapon.getWindowSize().x / 2.0f, -mWeapon.getWindowSize().y / 2.0f);
		LT.MOVE_WINDOW(mWeaponParent, weaponParentPos);
	}
	protected void onAnimationPlayEnd(INGUIAnimation window, bool isBreak)
	{
		if (isBreak)
		{
			return;
		}
		mCloth.stop(true, false);
	}
}
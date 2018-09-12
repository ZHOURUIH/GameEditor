using UnityEngine;
using System.Collections;
using System.Collections.Generic;

public class CharacterData : CharacterBaseData
{
	public CharacterData()
	{
		mMoney = 0;
		mLevel = 0;
		mOccupation = 0;
		mDirection = 0;
		mSpeed = 0;
		mClothID = 0;
		mWeaponID = 0;
		mSceneID = 0;
		mPosX = 0.0f;
		mPosY = 0.0f;
	}
	public int mMoney;
	public int mLevel;
	public int mOccupation;
	public int mDirection;
	public float mSpeed;
	public short mClothID;
	public short mWeaponID;
	public int mSceneID;
	public float mPosX;
	public float mPosY;
}

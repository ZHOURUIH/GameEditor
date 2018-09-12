using UnityEngine;
using System.Collections;
using System;

public class CharacterAvatar : GameComponent
{
	CharacterOther mPlayer;
	CharacterData mData;
	public HumanAvatar mHumanAvatar;
	public CharacterAvatar(Type type, string name)
		:
		base(type, name)
	{ }
	public override void init(ComponentOwner owner)
	{
		base.init(owner);
		mPlayer = owner as CharacterOther;
		mData = mPlayer.getCharacterData();
		mHumanAvatar = mScriptScene.createHumanAvatar(mData.mGUID);
	}
	public override void destroy()
	{
		base.destroy();
	}
	public override void update(float elapsedTime)
	{
		base.update(elapsedTime);
	}
	//-----------------------------------------------------------------------------------------------------------------------
	protected override bool isType(Type type) { return type == typeof(CharacterAvatar); }
	protected override void setBaseType() { mBaseType = typeof(CharacterAvatar); }
}

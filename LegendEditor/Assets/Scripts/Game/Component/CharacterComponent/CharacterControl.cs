using UnityEngine;
using System.Collections;
using System;

public class CharacterControl : GameComponent
{
	CharacterOther mPlayer;
	public CharacterControl(Type type, string name)
		:
		base(type, name)
	{ }
	public override void init(ComponentOwner owner)
	{
		base.init(owner);
		mPlayer = owner as CharacterOther;
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
	protected override bool isType(Type type) { return type == typeof(CharacterControl); }
	protected override void setBaseType() { mBaseType = typeof(CharacterControl); }
}

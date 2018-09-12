using UnityEngine;
using System.Collections;
using System.Collections.Generic;
public class Character : MovableObject
{
	protected CHARACTER_TYPE	mCharacterType;// 角色类型
	protected CharacterData		mCharacterData;	//玩家数据

	public Character(CHARACTER_TYPE type, string name)
		:
		base(name)
	{
		mCharacterType = type;
		mCharacterData = new CharacterData();
		mCharacterData.mName = mName;
	}
	public override void init()
	{
		base.init();
	}
	public void setID(int guid)
	{
		mCharacterData.mGUID = guid;
	}
	public override void initComponents()
	{
       base.initComponents();
	}
	public override void update(float elaspedTime)
	{
		// 先更新自己的所有组件
		base.updateComponents(elaspedTime);
	}
	public virtual void notifyComponentChanged(GameComponent component) {}
	public CharacterData getCharacterData() { return mCharacterData; }
	public CHARACTER_TYPE getType() { return mCharacterType; }
	//---------------------------------------------------------------------------------------------------------
}

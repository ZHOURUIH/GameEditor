using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using UnityEngine;

public class Monster : ComponentOwner
{
	public MonsterEnemyData mData;
	//public MonsterAvatar mMonsterAvatar;
	public Monster(string name)
		:base(name)
	{
		mData = new MonsterEnemyData();
		mData.mName = name;
	}
	public virtual void init(int typeID, int instanceID)
	{
		initComponents();
		mData.mTypeID = typeID;
		mData.mInstanceID = instanceID;
		//mMonsterAvatar = mScriptObjectEditor.createMonsterAvatar(mData.mInstanceID, mData.mTypeID, "Deer", mData.mName);
	}
	public override void initComponents()
	{
		;
	}
	public void setLevel(int level)
	{
		mData.mLevel = level;
		//mMonsterAvatar.setLevel(mData.mLevel);
	}
	public void setHP(int hp)
	{
		mData.mHP = hp;
		//mMonsterAvatar.setHP(mData.mHP);
	}
	public void setMaxHP(int maxHP)
	{
		mData.mMaxHP = maxHP;
		//mMonsterAvatar.setMaxHP(mData.mMaxHP);
	}
	public void setPosition(Vector2 pos)
	{
		mData.mPosition = pos;
		//mMonsterAvatar.setPosition(mData.mPosition);
	}
	public void setDirection(int direction)
	{
		mData.mDirection = direction;
		//mMonsterAvatar.setDirection(mData.mDirection);
	}
}
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using UnityEngine;

public class MonsterEnemyData
{
	public int mTypeID;			// 怪物的类型ID
	public int mInstanceID;		// 当前怪物实例的ID
	public string mName;
	public int mLevel;
	public int mHP;
	public int mMaxHP;
	public int mDirection;
	public Vector2 mPosition;
	public MonsterEnemyData()
	{
		;
	}
}
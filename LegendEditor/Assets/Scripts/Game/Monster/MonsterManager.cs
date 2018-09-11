using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

public class MonsterManager : FrameComponent
{
	Dictionary<int, Monster> mMonsterList;
	public MonsterManager(string name)
		: base(name)
	{
		mMonsterList = new Dictionary<int, Monster>();
	}
	public override void init()
	{
		;
	}
	public override void destroy()
	{
		base.destroy();
	}
	public override void update(float elapsedTime)
	{
		base.update(elapsedTime);
	}
	public Monster createMonster(int typeID, int instanceID)
	{
		if(mMonsterList.ContainsKey(instanceID))
		{
			return mMonsterList[instanceID];
		}
		// 从数据库中查询该类型的怪物的名字
		MonsterData monsterData;
		mSQLite.mSQLiteMonster.query(typeID, out monsterData);
		if(monsterData == null)
		{
			return null;
		}
		Monster monster = new Monster(monsterData.mLabel);
		monster.init(typeID, instanceID);
		mMonsterList.Add(instanceID, monster);
		return monster;
	}
}
using UnityEngine;
using System.Collections;
using System.Collections.Generic;

public class CharacterMyself : CharacterOther
{
	public CharacterMyself(string name)
		:base(name)
	{}
	public override void update(float elapsedTime)
	{
		base.update(elapsedTime);
	}
}

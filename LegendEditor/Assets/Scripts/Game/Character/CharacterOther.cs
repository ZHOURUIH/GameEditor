using UnityEngine;
using System.Collections;
using System.Collections.Generic;

public class CharacterOther : Character
{
	protected HumanAvatar mAvatar;
	protected HumanAnimation mAnimation;
	public CharacterOther(CHARACTER_TYPE type, string name)
		:
		base(type, name)
	{
        ;		
	}
	public override void init()
	{
		// 先创建角色渲染实例,再执行初始化
		mAvatar = mScriptObjectEditor.createHumanAvatar(mCharacterData.mGUID);
		base.init();
	}
	public override void initComponents()
	{
		base.initComponents();
		mAnimation = addComponent<HumanAnimation>("HumanAnimation", true);
	}
	public override void update(float elapsedTime)
	{
		base.update(elapsedTime);
	}
	public HumanAnimation getAnimation() { return mAnimation; }
	public HumanAvatar getAvatar() { return mAvatar; }
}
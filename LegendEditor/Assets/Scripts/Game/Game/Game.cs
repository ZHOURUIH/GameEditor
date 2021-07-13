using UnityEngine;
using System;
using System.Collections;
using System.Collections.Generic;
using System.Threading;
using System.Net;

public class Game : GameFramework
{
	//-------------------------------------------------------------------------------------------------------------
	protected override void initComponent()
	{
		base.initComponent();
		registeComponent<GameConfig>();
		registeComponent<LogSystem>();
		registeComponent<DataCenter>();
		registeComponent<Effect2DManager>();
		registeComponent<SceneCamera>();
		registeComponent<MapManager>();
	}
	protected override void notifyBase()
	{
		base.notifyBase();
		// 所有类都构造完成后通知GameBase
		GameBase frameBase = new GameBase();
		frameBase.notifyConstructDone();
	}
	protected override void registe()
	{
		LayoutRegister.registeAllLayout();
		SQLiteRegister.registeAllTable();
		CharacterRegister.registeAllCharacter();
	}
	protected override void launch()
	{
		base.launch();
		CommandGameSceneManagerEnter cmd = FrameBase.newCmd(out cmd, false);
		cmd.mSceneType = typeof(StartScene);
		FrameBase.pushCommand(cmd, FrameBase.mGameSceneManager);
	}
}
using UnityEngine;
using System;
using System.Collections;
using System.Collections.Generic;

public class GameSceneRegister : GameBase
{
	public static void registerAllGameScene()
	{
		registeGameScene<StartScene>(GAME_SCENE_TYPE.GST_START);
	}
	//-------------------------------------------------------------------------------------------------------------
	protected static void registeGameScene<T>(GAME_SCENE_TYPE type) where T : GameScene
	{
		mGameSceneManager.registeGameScene(typeof(T), type);
	}
}
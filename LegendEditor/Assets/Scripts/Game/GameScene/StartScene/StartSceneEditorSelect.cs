using UnityEngine;
using System;
using System.Collections;
using System.Collections.Generic;

public class StartSceneEditorSelect : SceneProcedure
{
	public StartSceneEditorSelect(GameScene gameScene)
		:base(gameScene){}
	protected override void onInit(SceneProcedure lastProcedure, string intent)
	{
		QualitySettings.vSyncCount = 0;
		Application.targetFrameRate = 30;
		changeProcedure<StartSceneSceneEditor>();
	}
	protected override void onUpdate(float elapsedTime)
	{
		;
	}
	protected override void onExit(SceneProcedure nextProcedure)
	{
		;
	}
	protected override void onKeyProcess(float elapsedTime)
	{
		;
	}
}
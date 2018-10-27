using UnityEngine;
using System;
using System.Collections;
using System.Collections.Generic;

public class StartSceneEditorSelect : SceneProcedure
{
	public StartSceneEditorSelect(PROCEDURE_TYPE type, GameScene gameScene)
		:
	base(type, gameScene)
	{
		;
	}
	protected override void onInit(SceneProcedure lastProcedure, string intent)
	{
		QualitySettings.vSyncCount = 0;
		Application.targetFrameRate = 30;
		mLayoutSubPrefabManager.loadAll(false);
		LT.LOAD_NGUI_SHOW(LAYOUT_TYPE.LT_EDITOR_SELECT, 0);
	}
	protected override void onUpdate(float elapsedTime)
	{
		;
	}
	protected override void onExit(SceneProcedure nextProcedure)
	{
		LT.UNLOAD_LAYOUT(LAYOUT_TYPE.LT_EDITOR_SELECT);
	}
	protected override void onKeyProcess(float elapsedTime)
	{
		;
	}
}
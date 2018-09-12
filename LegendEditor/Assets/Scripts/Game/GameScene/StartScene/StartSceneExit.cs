using UnityEngine;
using System.Collections;

public class StartSceneExit : SceneProcedure
{
	public StartSceneExit(PROCEDURE_TYPE type, GameScene gameScene)
		:
		base(type, gameScene)
	{}
	protected override void onInit(SceneProcedure lastProcedure, string intent)
    {
		// 隐藏该场景的所有布局
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
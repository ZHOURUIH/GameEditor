using UnityEngine;
using System.Collections;

public class StartScene : GameScene
{
	public StartScene(string name)
		:base(name){}
	public override void assignStartExitProcedure()
	{
		mStartProcedure = typeof(StartSceneEditorSelect);
		mExitProcedure = typeof(StartSceneExit);
	}
	public override void createSceneProcedure()
	{
		addProcedure<StartSceneEditorSelect>();
		addProcedure<StartSceneSceneEditor>();
		addProcedure<StartSceneExit>();
		if (mSceneProcedureList.Count != (int)PROCEDURE.P_START_MAX - (int)PROCEDURE.P_START_MIN - 1)
		{
			Debug.LogError("error : not all procedure added!");
		}
	}
}

using UnityEngine;
using System.Collections;

public class StartScene : GameScene
{
	public StartScene(GAME_SCENE_TYPE type, string name)
		:
		base(type, name)
	{
		;
	}
	public override void assignStartExitProcedure()
	{
		mStartProcedure = PROCEDURE_TYPE.PT_START_EDITOR_SELECT;
		mExitProcedure = PROCEDURE_TYPE.PT_START_EXIT;
	}
	public override void createSceneProcedure()
	{
		addProcedure<StartSceneEditorSelect>(PROCEDURE_TYPE.PT_START_EDITOR_SELECT);
		addProcedure<StartSceneObjectEditor>(PROCEDURE_TYPE.PT_START_OBJECT_EDITOR);
		addProcedure<StartSceneEffectTextureEditor>(PROCEDURE_TYPE.PT_START_EFFECT_TEXTURE_EDITOR);
		addProcedure<StartSceneEffectSpriteEditor>(PROCEDURE_TYPE.PT_START_EFFECT_SPRITE_EDITOR);
		addProcedure<StartSceneSceneEditor>(PROCEDURE_TYPE.PT_START_SCENE_EDITOR);
		addProcedure<StartSceneExit>(PROCEDURE_TYPE.PT_START_EXIT);
		if (mSceneProcedureList.Count != (int)PROCEDURE_TYPE.PT_START_MAX - (int)PROCEDURE_TYPE.PT_START_MIN - 1)
		{
			Debug.LogError("error : not all procedure added!");
		}
	}
}

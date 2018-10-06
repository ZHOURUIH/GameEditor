using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using UnityEngine;

public class ScriptEditorSelect : LayoutScript
{
	txNGUIButton mObjectEditor;
	txNGUIButton mEffectTextureEditor;
	txNGUIButton mEffectSpriteEditor;
	txNGUIButton mSceneEditor;
	public ScriptEditorSelect(string name, GameLayout layout)
		:
		base(name, layout)
	{
		;
	}
	public override void assignWindow()
	{
		newObject(out mObjectEditor, "ObjectEditor");
		newObject(out mEffectTextureEditor, "EffectTextureEditor");
		newObject(out mEffectSpriteEditor, "EffectSpriteEditor");
		newObject(out mSceneEditor, "SceneEditor");
	}
	public override void init()
	{
		registeBoxColliderNGUI(mObjectEditor, onObjectEditorClick);
		registeBoxColliderNGUI(mEffectTextureEditor, onEffectTextureEditorClick);
		registeBoxColliderNGUI(mEffectSpriteEditor, onEffectSpriteEditorClick);
		registeBoxColliderNGUI(mSceneEditor, onSceneEditorClick);
	}
	public override void onReset()
	{
		;
	}
	public override void onShow(bool immediately, string param)
	{
		;
	}
	public override void onHide(bool immediately, string param)
	{
		;
	}
	public override void update(float elapsedTime)
	{
		;
	}
	//---------------------------------------------------------------------------------------
	protected void onObjectEditorClick(GameObject go)
	{
		CommandGameSceneChangeProcedure cmd = newCmd(out cmd);
		cmd.mProcedure = PROCEDURE_TYPE.PT_START_OBJECT_EDITOR;
		pushCommand(cmd, mGameSceneManager.getCurScene());
	}
	protected void onEffectTextureEditorClick(GameObject go)
	{
		CommandGameSceneChangeProcedure cmd = newCmd(out cmd);
		cmd.mProcedure = PROCEDURE_TYPE.PT_START_EFFECT_TEXTURE_EDITOR;
		pushCommand(cmd, mGameSceneManager.getCurScene());
	}
	protected void onEffectSpriteEditorClick(GameObject go)
	{
		CommandGameSceneChangeProcedure cmd = newCmd(out cmd);
		cmd.mProcedure = PROCEDURE_TYPE.PT_START_EFFECT_SPRITE_EDITOR;
		pushCommand(cmd, mGameSceneManager.getCurScene());
	}
	protected void onSceneEditorClick(GameObject go)
	{
		CommandGameSceneChangeProcedure cmd = newCmd(out cmd);
		cmd.mProcedure = PROCEDURE_TYPE.PT_START_SCENE_EDITOR;
		pushCommand(cmd, mGameSceneManager.getCurScene());
	}
}
using UnityEngine;
using System;
using System.Collections;
using System.Collections.Generic;

public class LayoutRegister : GameBase
{
	public static void registeAllLayout()
	{
		registeLayout<ScriptObjectEditor>(LAYOUT_TYPE.LT_OBJECT_EDITOR, "UIObjectEditor");
		registeLayout<ScriptSceneEditor>(LAYOUT_TYPE.LT_SCENE_EDITOR, "UISceneEditor");
		registeLayout<ScriptEditorSelect>(LAYOUT_TYPE.LT_EDITOR_SELECT, "UIEditorSelect");
		registeLayout<ScriptEffectTextureEditor>(LAYOUT_TYPE.LT_EFFECT_TEXTURE_EDITOR, "UIEffectTextureEditor");
		registeLayout<ScriptEffectSpriteEditor>(LAYOUT_TYPE.LT_EFFECT_SPRITE_EDITOR, "UIEffectSpriteEditor");
		if (mLayoutManager.getLayoutCount() < (int)LAYOUT_TYPE.LT_MAX)
		{
			UnityUtility.logError("error : not all script added! max count : " + (int)LAYOUT_TYPE.LT_MAX + ", added count :" + mLayoutManager.getLayoutCount());
		}
	}
	public static void onScriptChanged(LayoutScript script, bool created = true)
	{
		// 只有布局与脚本唯一对应的才能使用变量快速访问
		if (mLayoutManager.getScriptMappingCount(script.GetType()) > 1)
		{
			return;
		}
		if (assign(ref mScriptObjectEditor, script, created)) return;
		if (assign(ref mScriptSceneEditor, script, created)) return;
		if (assign(ref mScriptEditorSelect, script, created)) return;
		if (assign(ref mScriptEffectTextureEditor, script, created)) return;
		if (assign(ref mScriptEffectSpriteEditor, script, created)) return;
	}
	//----------------------------------------------------------------------------------------------------------------------------------------------------------------
	protected static void registeLayout<T>(LAYOUT_TYPE layout, string name) where T : LayoutScript
	{
		mLayoutManager.registeLayout(typeof(T), layout, name);
	}
	protected static bool assign<T>(ref T thisScript, LayoutScript value, bool created) where T : LayoutScript
	{
		if (typeof(T) == value.GetType())
		{
			thisScript = created ? value as T : null;
			return true;
		}
		else
		{
			return false;
		}
	}
}
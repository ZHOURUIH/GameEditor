using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using UnityEngine;

public class ScriptSceneEditor : LayoutScript
{
	txUIObject mEditorRoot;
	txNGUIEditbox mSceneNameEdit;
	txNGUIEditbox mWidthEdit;
	txNGUIEditbox mHeightEdit;
	txNGUIButton mCreateNewScene;
	txUIObject mSceneRoot;
	List<txNGUITexture> mTiles;
	public ScriptSceneEditor(string name, GameLayout layout)
		:
		base(name, layout)
	{
		mTiles = new List<txNGUITexture>();
	}
	public override void assignWindow()
	{
		newObject(out mEditorRoot, "EditorRoot");
		newObject(out mSceneNameEdit, mEditorRoot, "SceneNameEdit");
		newObject(out mWidthEdit, mEditorRoot, "WidthEdit");
		newObject(out mHeightEdit, mEditorRoot, "HeightEdit");
		newObject(out mCreateNewScene, mEditorRoot, "CreateNewScene");
		newObject(out mSceneRoot, "SceneRoot");
	}
	public override void init()
	{
		registeBoxColliderNGUI(mCreateNewScene, onCreateNewSceneClick);
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
	public void createTiles(string sceneName, int width, int height)
	{
		clearTiles();
		for (int i = 0; i < width; ++i)
		{
			for (int j = 0; j < height; ++j)
			{
				string name = "" + i + "_" + j;
				txNGUITexture tile = createObject<txNGUITexture>(mSceneRoot, name);
				tile.setTextureName(CommonDefine.R_GAME_TEXTURE_PATH + "Scene/" + sceneName + "/" + name);
				// 设置图片位置和大小
				Vector2 textureSize = new Vector2(tile.getTexture().width, tile.getTexture().height);
				tile.setWindowSize(textureSize);
				LayoutTools.MOVE_WINDOW(tile, new Vector3(textureSize.x * i + textureSize.x / 2.0f, -textureSize.y * j - textureSize.y / 2.0f, 0.0f));
				mTiles.Add(tile);
			}
		}
	}
	public void clearTiles()
	{
		foreach(var item in mTiles)
		{
			destroyObject(item, true);
		}
		mTiles.Clear();
	}
	protected void onCreateNewSceneClick(GameObject go)
	{
		int width = StringUtility.stringToInt(mWidthEdit.getText());
		int height = StringUtility.stringToInt(mHeightEdit.getText());
		createTiles(mSceneNameEdit.getText(), width, height);
	}
}
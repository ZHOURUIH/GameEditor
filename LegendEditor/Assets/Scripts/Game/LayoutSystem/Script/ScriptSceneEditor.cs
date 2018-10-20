using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using UnityEngine;

public class ScriptSceneEditor : LayoutScript
{
	txNGUIPanel mEditorRoot;
	txNGUIEditbox mSceneNameEdit;
	txNGUIButton mCreateNewScene;
	txNGUIText mWidthLabel;
	txNGUIText mHeightLabel;
	txUIObject mSceneRoot;
	txNGUIPanel mBackRoot;
	txUIObject mMiddleRoot;
	txNGUIPanel mObjRoot;
	List<txNGUIPanel> mMidPanelList;
	List<txNGUITexture> mTiles;
	public ScriptSceneEditor(string name, GameLayout layout)
		:
		base(name, layout)
	{
		mTiles = new List<txNGUITexture>();
		mMidPanelList = new List<txNGUIPanel>();
	}
	public override void assignWindow()
	{
		newObject(out mEditorRoot, "EditorRoot");
		newObject(out mSceneNameEdit, mEditorRoot, "SceneNameEdit");
		newObject(out mCreateNewScene, mEditorRoot, "CreateNewScene");
		newObject(out mWidthLabel, mEditorRoot, "SceneWidth");
		newObject(out mHeightLabel, mEditorRoot, "SceneHeight");
		newObject(out mSceneRoot, "SceneRoot");
		newObject(out mBackRoot, mSceneRoot, "BackRoot");
		newObject(out mMiddleRoot, mSceneRoot, "MiddleRoot");
		newObject(out mObjRoot, mSceneRoot, "ObjRoot");
	}
	public override void init()
	{
		registeBoxColliderNGUI(mCreateNewScene, onCreateNewSceneClick);
	}
	public override void onReset()
	{
		mWidthLabel.setLabel("宽:0");
		mHeightLabel.setLabel("高:0");
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
	public void createScene(SceneMap map)
	{
		clearTiles();
		int tileCount = map.mTileList.Length;
		for(int i = 0; i < tileCount; ++i)
		{
			int y = i % map.mHeader.mHeight;
			int x = i / map.mHeader.mHeight;
			Vector2 halfMap = new Vector2(map.mHeader.mWidth * 48 / 2.0f, map.mHeader.mHeight * 32 / 2.0f);
			MapTile tile = map.mTileList[i];
			// 偶数行偶数列才渲染大地砖
			if(x % 2 == 0 && y % 2 == 0)
			{
				string backTexName = CommonDefine.R_GAME_TEXTURE_PATH + "MapTexture/Tiles/" + (tile.mBngImgIdx - 1);
				Texture backTex = mResourceManager.loadResource<Texture>(backTexName, false);
				if (backTex != null)
				{
					txNGUITexture backTile = createObject<txNGUITexture>(mBackRoot, "back_" + x + "_" + y);
					backTile.setTexture(backTex, true);
					Vector2 posOffset = backTile.getTextureSize() / 2.0f;
					posOffset += new Vector2(48 * x - halfMap.x, halfMap.y - 32 * (y + 1));
					LayoutTools.MOVE_WINDOW(backTile, posOffset);
					backTile.setDepth(1);
					mTiles.Add(backTile);
				}
			}

			string midTexName = CommonDefine.R_GAME_TEXTURE_PATH + "MapTexture/SmTiles/" + (tile.mMidImgIdx - 1);
			Texture midTex = mResourceManager.loadResource<Texture>(midTexName, false);
			if(midTex != null)
			{
				int materialIndex = i / 5000;
				if (materialIndex >= mMidPanelList.Count)
				{
					txNGUIPanel panel = createObject<txNGUIPanel>(mMiddleRoot, "MidPanel" + materialIndex);
					panel.setDepth(2);
					mMidPanelList.Add(panel);
				}
				txNGUITexture midTile = createObject<txNGUITexture>(mMidPanelList[materialIndex], "mid_" + x + "_" + y);
				midTile.setTexture(midTex, true);
				Vector2 posOffset = midTile.getTextureSize() / 2.0f;
				posOffset += new Vector2(48 * x - halfMap.x, halfMap.y - 32 * y);
				LayoutTools.MOVE_WINDOW(midTile, posOffset);
				midTile.setDepth(2);
				mTiles.Add(midTile);
			}
			
			string objTexName = CommonDefine.R_GAME_TEXTURE_PATH + "MapTexture/Objects" + (tile.mObjFileIdx + 1) + "/" + (tile.mObjImgIdx - 1);
			Texture objTex = mResourceManager.loadResource<Texture>(objTexName, false);
			if (objTex != null)
			{
				txNGUITexture objTile = createObject<txNGUITexture>(mObjRoot, "obj_" + x + "_" + y);
				objTile.setTexture(objTex, true);
				Vector2 posOffset = objTile.getTextureSize() / 2.0f;
				posOffset += new Vector2(48 * x - halfMap.x, halfMap.y - 32 * y);
				LayoutTools.MOVE_WINDOW(objTile, posOffset);
				objTile.setDepth(3);
				mTiles.Add(objTile);
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
		string mapFileName = CommonDefine.F_STREAMING_ASSETS_PATH + "Map/" + mSceneNameEdit.getText()+ ".map";
		SceneMap sceneMap = new SceneMap();
		sceneMap.readFile(mapFileName);
		createScene(sceneMap);
		mWidthLabel.setLabel("宽:" + sceneMap.mHeader.mWidth);
		mHeightLabel.setLabel("高:" + sceneMap.mHeader.mHeight);
	}
}
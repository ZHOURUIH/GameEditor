using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using UnityEngine;

public class ScriptSceneEditor : LayoutScript
{
	txUIObject mEditorRoot;
	txNGUIEditbox mSceneNameEdit;
	txNGUIButton mCreateNewScene;
	txUIObject mSceneRoot;
	txUIObject mBackRoot;
	txUIObject mMiddleRoot;
	txUIObject mObjRoot;
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
		newObject(out mCreateNewScene, mEditorRoot, "CreateNewScene");
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
	public void createScene(SceneMap map)
	{
		clearTiles();
		int tileCount = map.mTileList.Length;
		for(int i = 0; i < tileCount; ++i)
		{
			int y = i % map.mHeader.mHeight;
			int x = i / map.mHeader.mHeight;
			MapTile tile = map.mTileList[i];
			// 偶数行偶数列才渲染大地砖
			if(x % 2 == 0 && y % 2 == 0)
			{
				string backTexName = CommonDefine.R_GAME_TEXTURE_PATH + "MapTexture/Tiles/" + tile.mBngImgIdx;
				Texture backTex = mResourceManager.loadResource<Texture>(backTexName, false);
				if (backTex != null)
				{
					txNGUITexture backTile = createObject<txNGUITexture>(mBackRoot, "back_" + x + "_" + y);
					backTile.setTexture(backTex);
					Vector2 textureSize = new Vector2(backTile.getTexture().width, backTile.getTexture().height);
					backTile.setWindowSize(textureSize);
					LayoutTools.MOVE_WINDOW(backTile, new Vector3(48 * x + 96 / 2, 32 * y + 64 / 2));
					backTile.setDepth(1);
					mTiles.Add(backTile);
				}
			}

			string midTexName = CommonDefine.R_GAME_TEXTURE_PATH + "MapTexture/SmTiles/" + tile.mMidImgIdx;
			Texture midTex = mResourceManager.loadResource<Texture>(midTexName, false);
			if(midTex != null)
			{
				txNGUITexture midTile = createObject<txNGUITexture>(mMiddleRoot, "mid_" + x + "_" + y);
				midTile.setTexture(midTex);
				Vector2 textureSize = new Vector2(midTile.getTexture().width, midTile.getTexture().height);
				midTile.setWindowSize(textureSize);
				LayoutTools.MOVE_WINDOW(midTile, new Vector3(48 * x + 48 / 2, 32 * y + 32 / 2));
				midTile.setDepth(2);
				mTiles.Add(midTile);
			}
			
			string objTexName = CommonDefine.R_GAME_TEXTURE_PATH + "MapTexture/Objects" + (tile.mObjFileIdx + 1) + "/" + (tile.mObjImgIdx - 1);
			Texture objTex = mResourceManager.loadResource<Texture>(objTexName, false);
			if (objTex != null)
			{
				txNGUITexture objTile = createObject<txNGUITexture>(mObjRoot, "obj_" + x + "_" + y);
				objTile.setTexture(objTex);
				Vector2 textureSize = new Vector2(objTile.getTexture().width, objTile.getTexture().height);
				objTile.setWindowSize(textureSize);
				Vector2 posOffset = new Vector2();
				posOffset += new Vector2(textureSize.x / 2.0f, textureSize.y / 2.0f);
				posOffset += new Vector2(48 * (x - map.mHeader.mWidth / 2.0f), 32 * (map.mHeader.mHeight / 2.0f - y));
				LayoutTools.MOVE_WINDOW(objTile, posOffset);
				objTile.setDepth(10);
				mTiles.Add(objTile);
			}
		}
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
		string mapFileName = CommonDefine.F_STREAMING_ASSETS_PATH + "Map/" + mSceneNameEdit.getText()+ ".map";
		SceneMap sceneMap = new SceneMap();
		sceneMap.readFile(mapFileName);
		createScene(sceneMap);
	}
}
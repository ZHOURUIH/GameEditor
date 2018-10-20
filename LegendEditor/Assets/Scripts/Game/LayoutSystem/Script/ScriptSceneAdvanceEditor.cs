using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using UnityEngine;

public class PanelInfo
{
	public txNGUIPanel mPanel;
	public int mAtlasIndex;
	public int mObjectCount;
}

public class ScriptSceneAdvanceEditor : LayoutScript
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
	Dictionary<int, PanelInfo> mBackPanelList;
	Dictionary<int, List<int>> mBackAtlasList;	// 根据图集下标查找有哪些panel包含该图集
	List<txNGUISprite> mTilesSprite;
	public ScriptSceneAdvanceEditor(string name, GameLayout layout)
		:
		base(name, layout)
	{
		mTilesSprite = new List<txNGUISprite>();
		mBackPanelList = new Dictionary<int, PanelInfo>();
		mBackAtlasList = new Dictionary<int, List<int>>();
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
	public void createScene(SceneMapAdvance map)
	{
		DateTime time = DateTime.Now;
		clearTiles();
		Vector2 halfMap = new Vector2(map.mHeader.mWidth * 48 / 2.0f, map.mHeader.mHeight * 32 / 2.0f);
		UIAtlas midAtlas = mResourceManager.loadResource<UIAtlas>(CommonDefine.R_ATLAS_PATH + "Map/SmTiles", true);
		Dictionary<int, Dictionary<int, UIAtlas>> objAtlasIndexMap = new Dictionary<int, Dictionary<int, UIAtlas>>();
		Dictionary<int, UIAtlas> bngAtlasIndexMap = new Dictionary<int, UIAtlas>();
		string bigTilePath = CommonDefine.R_ATLAS_PATH + "Map/Tiles/";
		string objectImagePreString = CommonDefine.R_ATLAS_PATH + "Map/Objects";
		int maxPanelIndex = -1;
		int tileCount = map.mTileList.Length;
		for (int i = 0; i < tileCount; ++i)
		{
			int y = i % map.mHeader.mHeight;
			int x = i / map.mHeader.mHeight;
			string tileSuffix = x + "_" + y;
			MapTileAdvance tile = map.mTileList[i];
			// 偶数行偶数列才渲染大地砖
			if (x % 2 == 0 && y % 2 == 0)
			{
				// 大地砖图集
				UIAtlas bngAtlas = null;
				if(tile.mBngAtlasIndex != 0xFF)
				{
					if (!bngAtlasIndexMap.ContainsKey(tile.mBngAtlasIndex))
					{
						bngAtlas = mResourceManager.loadResource<UIAtlas>(bigTilePath + tile.mBngAtlasIndex, true);
						bngAtlasIndexMap.Add(tile.mBngAtlasIndex, bngAtlas);
					}
					else
					{
						bngAtlas = bngAtlasIndexMap[tile.mBngAtlasIndex];
					}
					// 大地砖图片窗口
					string bngSpriteName = "" + (tile.mBngImgIdx - 1);
					if (bngAtlas != null && bngAtlas.GetSprite(bngSpriteName) != null)
					{
						if(!mBackAtlasList.ContainsKey(tile.mBngAtlasIndex))
						{
							mBackAtlasList.Add(tile.mBngAtlasIndex, new List<int>());
						}
						// 一个panel中只允许有一个Drawcall
						// 一个panel满时,就应该放入新的panel中
						// 查找该图集存放在哪些panel中
						int panelIndex = -1;
						List<int> panelIndexList = mBackAtlasList[tile.mBngAtlasIndex];
						int panelCount = panelIndexList.Count;
						for(int j = 0; j < panelCount; ++j)
						{
							// 找到一个包含该图集,且数量未达上限的panel
							if(mBackPanelList[panelIndexList[j]].mObjectCount < 5000)
							{
								panelIndex = panelIndexList[j];
								break;
							}
						}
						// 没有找到合适的panel,则创建新的panel
						if(panelIndex == -1)
						{
							panelIndex = maxPanelIndex + 1;
							maxPanelIndex = panelIndex;
							PanelInfo panelInfo = new PanelInfo();
							panelInfo.mPanel = createObject<txNGUIPanel>(mBackRoot, "BackPanel" + panelIndex);
							panelInfo.mObjectCount = 0;
							panelInfo.mAtlasIndex = tile.mBngAtlasIndex;
							mBackPanelList.Add(panelIndex, panelInfo);
							mBackAtlasList[tile.mBngAtlasIndex].Add(panelIndex);
						}
						// panel中的窗口计数+1
						mBackPanelList[panelIndex].mObjectCount += 1;
						txNGUISprite backTile = createObject<txNGUISprite>(mBackPanelList[panelIndex].mPanel, "back_" + tileSuffix);
						backTile.setAtlas(bngAtlas);
						backTile.setSpriteName(bngSpriteName, true);
						Vector2 posOffset = backTile.getSpriteSize() / 2.0f;
						posOffset += new Vector2(48 * x - halfMap.x, halfMap.y - 32 * (y + 1));
						backTile.setLocalPosition(posOffset);
						backTile.setDepth(1);
						mTilesSprite.Add(backTile);
					}
				}
			}
			// 小地砖图片窗口
			string midSpriteName = "" + (tile.mMidImgIdx - 1);
			if (midAtlas.GetSprite(midSpriteName) != null)
			{
				txNGUISprite midTile = createObject<txNGUISprite>(mMiddleRoot, "mid_" + tileSuffix);
				midTile.setAtlas(midAtlas);
				midTile.setSpriteName(midSpriteName, true);
				Vector2 posOffset = midTile.getSpriteSize() / 2.0f;
				posOffset += new Vector2(48 * x - halfMap.x, halfMap.y - 32 * y);
				midTile.setLocalPosition(posOffset);
				mTilesSprite.Add(midTile);
			}
			// 加载对象图集
			UIAtlas atlas = null;
			if (!objAtlasIndexMap.ContainsKey(tile.mObjFileIdx + 1))
			{
				objAtlasIndexMap.Add(tile.mObjFileIdx + 1, new Dictionary<int, UIAtlas>());
			}
			if(tile.mObjAtlasIndex != 0xFF)
			{
				if (!objAtlasIndexMap[tile.mObjFileIdx + 1].ContainsKey(tile.mObjAtlasIndex))
				{
					atlas = mResourceManager.loadResource<UIAtlas>(objectImagePreString + (tile.mObjFileIdx + 1) + "/" + tile.mObjAtlasIndex, true);
					objAtlasIndexMap[tile.mObjFileIdx + 1].Add(tile.mObjAtlasIndex, atlas);
				}
				else
				{
					atlas = objAtlasIndexMap[tile.mObjFileIdx + 1][tile.mObjAtlasIndex];
				}
				// 创建对象图片窗口
				string spriteName = "" + (tile.mObjImgIdx - 1);
				if (atlas != null && atlas.GetSprite(spriteName) != null)
				{
					txNGUISprite objTile = createObject<txNGUISprite>(mObjRoot, "obj_" + tileSuffix);
					objTile.setAtlas(atlas);
					objTile.setSpriteName(spriteName, true);
					Vector2 posOffset = objTile.getSpriteSize() / 2.0f;
					posOffset += new Vector2(48 * x - halfMap.x, halfMap.y - 32 * y);
					objTile.setLocalPosition(posOffset);
					objTile.setDepth(3);
					mTilesSprite.Add(objTile);
				}
			}
		}
		logInfo("创建地图完成, 耗时:" + (int)((DateTime.Now - time).TotalSeconds) + "秒", LOG_LEVEL.LL_FORCE);
	}
	public void clearTiles()
	{
		foreach (var item in mTilesSprite)
		{
			destroyObject(item, true);
		}
		mTilesSprite.Clear();
	}
	protected void onCreateNewSceneClick(GameObject go)
	{
		string mapFileName = CommonDefine.F_STREAMING_ASSETS_PATH + "Map/" + mSceneNameEdit.getText() + ".amap";
		SceneMapAdvance sceneMap = new SceneMapAdvance();
		sceneMap.readFile(mapFileName);
		createScene(sceneMap);
		mWidthLabel.setLabel("宽:" + sceneMap.mHeader.mWidth);
		mHeightLabel.setLabel("高:" + sceneMap.mHeader.mHeight);
	}
}
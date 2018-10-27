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

public class WindowPool
{
	protected Dictionary<Type, List<txUIObject>> mInusedList;
	protected Dictionary<Type, List<txUIObject>> mUnusedList;
	protected LayoutScript mScript;
	protected txUIObject mPoolNode;
	public WindowPool(LayoutScript script)
	{
		mScript = script;
		mInusedList = new Dictionary<Type, List<txUIObject>>();
		mUnusedList = new Dictionary<Type, List<txUIObject>>();
	}
	public void assignWindow()
	{
		mPoolNode = mScript.createObject<txUIObject>("WindowSpritePool");
	}
	public void destroy()
	{
		mScript.destroyObject(mPoolNode, true);
		mPoolNode = null;
	}
	public T createWindow<T>(string name, txUIObject parent) where T : txUIObject, new()
	{
		txUIObject window = null;
		Type type = typeof(T);
		// 从未使用列表中获取
		if(mUnusedList.ContainsKey(type) && mUnusedList[type].Count > 0)
		{
			window = mUnusedList[type][mUnusedList[type].Count - 1];
			mUnusedList[type].RemoveAt(mUnusedList[type].Count - 1);
		}
		// 未使用列表中没有就创建新窗口
		if(window == null)
		{
			window = mScript.createObject<T>(name);
		}
		// 加入到已使用列表中
		if(!mInusedList.ContainsKey(type))
		{
			mInusedList.Add(type, new List<txUIObject>());
		}
		mInusedList[type].Add(window);
		window.setActive(true);
		window.setName(name);
		window.setParent(parent);
		return window as T;
	}
	public void destroyWindow<T>(T window) where T : txUIObject
	{
		if(window == null)
		{
			return;
		}
		Type type = typeof(T);
		if(!mUnusedList.ContainsKey(type))
		{
			mUnusedList.Add(type, new List<txUIObject>());
		}
		mUnusedList[type].Add(window);
		mInusedList[type].Remove(window);
		window.setActive(false);
		window.setParent(mPoolNode);
	}
}

public class TileWindow
{
	public txNGUISprite mBackTile;
	public txNGUISprite mMidTile;
	public txNGUISprite mObjectTile;
	public txNGUISpriteAnim mEffect;
	public void setActive(bool active)
	{
		if(mBackTile != null && mBackTile.isActive() != active)
		{
			mBackTile.setActive(active);
		}
		if (mMidTile != null && mMidTile.isActive() != active)
		{
			mMidTile.setActive(active);
		}
		if (mObjectTile != null && mObjectTile.isActive() != active)
		{
			mObjectTile.setActive(active);
		}
		if(mEffect != null && mEffect.isActive() != active)
		{
			mEffect.setActive(active);
		}
	}
	public void destroy(WindowPool pool)
	{
		pool.destroyWindow(mBackTile);
		pool.destroyWindow(mMidTile);
		pool.destroyWindow(mObjectTile);
		pool.destroyWindow(mEffect);
	}
}

public class ScriptSceneAdvanceEditor : LayoutScript
{
	protected txNGUIPanel mEditorRoot;
	protected txNGUIEditbox mSceneNameEdit;
	protected txNGUIButton mCreateNewScene;
	protected txNGUIText mWidthLabel;
	protected txNGUIText mHeightLabel;
	protected txNGUIEditbox mViewWidthEdit;
	protected txNGUIEditbox mViewHeightEdit;
	protected txNGUIText mViewWidthLabel;
	protected txNGUIText mViewHeightLabel;
	protected txUIObject mSceneRoot;
	protected txNGUIPanel mBackRoot;
	protected txNGUIPanel mMiddleRoot;
	protected txNGUIPanel mObjRoot;
	protected Dictionary<int, PanelInfo> mBackPanelList;
	protected Dictionary<int, List<int>> mBackAtlasList;	// 根据图集下标查找有哪些panel包含该图集
	protected Vector2 mCurViewPos;						// 使用地图的坐标系,X轴向右,Y轴向下
	protected SceneMapAdvance mSceneMap;
	protected WindowPool mWindowPool;
	protected TileWindow[] mTileArray;			// 当前场景所有的窗口
	protected Dictionary<int, MapTileAdvance> mVisibleTiles; // 存放当前可见的窗口
	protected UIAtlas midAtlas;
	protected Dictionary<int, Dictionary<int, UIAtlas>> mObjAtlasIndexMap;		// 存放所有使用过的地图对象图集,切换地图也不清空
	protected Dictionary<int, UIAtlas> mBngAtlasIndexMap;						// 存放所有使用过的地图大地砖图集,切换地图也不清空
	protected Dictionary<int, Dictionary<int, UIAtlas>> mEffectAtlasIndexMap;   // 存放所有使用过的特效图集,切换地图也不清空
	protected string mBackTilePath;
	protected string mObjectImagePreString;
	protected string mEffectPath;
	protected Vector2 mHalfMap;
	protected const int MAX_WINDOW_PER_PANEL = 5000;
	protected int mMoveSpeed = 10;
	public ScriptSceneAdvanceEditor(string name, GameLayout layout)
		:
		base(name, layout)
	{
		mBackPanelList = new Dictionary<int, PanelInfo>();
		mBackAtlasList = new Dictionary<int, List<int>>();
		mWindowPool = new WindowPool(this);
		mVisibleTiles = new Dictionary<int, MapTileAdvance>();
		mObjAtlasIndexMap = new Dictionary<int, Dictionary<int, UIAtlas>>();
		mBngAtlasIndexMap = new Dictionary<int, UIAtlas>();
		mEffectAtlasIndexMap = new Dictionary<int, Dictionary<int, UIAtlas>>();
		mBackTilePath = CommonDefine.R_ATLAS_PATH + "Map/Tiles/";
		mEffectPath = CommonDefine.R_ATLAS_TEXTURE_ANIM_PATH + "Effect/Map/";
		mObjectImagePreString = CommonDefine.R_ATLAS_PATH + "Map/Objects";
	}
	public override void assignWindow()
	{
		newObject(out mEditorRoot, "EditorRoot");
		newObject(out mSceneNameEdit, mEditorRoot, "SceneNameEdit");
		newObject(out mCreateNewScene, mEditorRoot, "CreateNewScene");
		newObject(out mWidthLabel, mEditorRoot, "SceneWidth");
		newObject(out mHeightLabel, mEditorRoot, "SceneHeight");
		newObject(out mViewWidthEdit, mEditorRoot, "ViewWidthEdit");
		newObject(out mViewHeightEdit, mEditorRoot, "ViewHeightEdit");
		newObject(out mViewWidthLabel, mEditorRoot, "ViewWidthLabel");
		newObject(out mViewHeightLabel, mEditorRoot, "ViewHeightLabel");
		newObject(out mSceneRoot, "SceneRoot");
		newObject(out mBackRoot, mSceneRoot, "BackRoot");
		newObject(out mMiddleRoot, mSceneRoot, "MiddleRoot");
		newObject(out mObjRoot, mSceneRoot, "ObjRoot");
		mWindowPool.assignWindow();
	}
	public override void init()
	{
		registeBoxColliderNGUI(mCreateNewScene, onCreateNewSceneClick);
		midAtlas = mResourceManager.loadResource<UIAtlas>(CommonDefine.R_ATLAS_PATH + "Map/SmTiles", true);
	}
	public override void onReset()
	{
		mWidthLabel.setLabel("宽:0");
		mHeightLabel.setLabel("高:0");
		mViewWidthEdit.setText("32");
		mViewHeightEdit.setText("32");
		mCurViewPos = Vector2.zero;
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
		if(mSceneMap != null)
		{
			bool changed = false;
			// 方向键用来移动地图
			if (getKeyCurrentUp(KeyCode.UpArrow))
			{
				mCurViewPos.y += 1 * mMoveSpeed;
				changed = true;
			}
			if (getKeyCurrentUp(KeyCode.DownArrow))
			{
				mCurViewPos.y -= 1 * mMoveSpeed;
				changed = true;
			}
			if (getKeyCurrentUp(KeyCode.LeftArrow))
			{
				mCurViewPos.x += 1 * mMoveSpeed;
				changed = true;
			}
			if (getKeyCurrentUp(KeyCode.RightArrow))
			{
				mCurViewPos.x -= 1 * mMoveSpeed;
				changed = true;
			}
			if (changed)
			{
				refreshMap();
				setViewPos(mCurViewPos);
			}
		}
	}
	public void setViewPos(Vector2 tileViewPos)
	{
		tileViewPos.x -= mSceneMap.mHeader.mWidth / 2.0f;
		tileViewPos.y -= mSceneMap.mHeader.mHeight / 2.0f;
		LT.MOVE_WINDOW(mSceneRoot, new Vector3(-tileViewPos.x * 48, tileViewPos.y * 32));
	}
	public void refreshMap()
	{
		logInfo("view center : " + mCurViewPos, LOG_LEVEL.LL_FORCE);
		// 先判断是否有窗口已经从可见变为不可见
		int viewWidth = stringToInt(mViewWidthEdit.getText());
		int viewHeight = stringToInt(mViewHeightEdit.getText());
		List<int> removedIndex = new List<int>();
		foreach(var item in mVisibleTiles)
		{
			int x = tileIndexToTileX(item.Key);
			int y = tileIndexToTileY(item.Key);
			// 判断是否已经超出视野范围
			if (!isInRange(x, mCurViewPos.x - viewWidth / 2, mCurViewPos.x + viewWidth /2)
				|| !isInRange(y, mCurViewPos.y - viewHeight / 2, mCurViewPos.y + viewHeight / 2))
			{
				mTileArray[item.Key].setActive(false);
				removedIndex.Add(item.Key);
			}
		}
		int removedCount = removedIndex.Count;
		for(int i = 0; i < removedCount; ++i)
		{
			mVisibleTiles.Remove(removedIndex[i]);
		}
		// 查找从不可见变为可见的窗口
		for (int i = 0; i < viewWidth; ++i)
		{
			for (int j = 0; j < viewHeight; ++j)
			{
				int curTilePosX = i + (int)mCurViewPos.x - viewWidth / 2;
				int curTilePosY = j + (int)mCurViewPos.y - viewHeight / 2;
				int tileIndex = tilePosToTileIndex(curTilePosX, curTilePosY);
				if (isInRange(curTilePosX, (int)mCurViewPos.x - viewWidth / 2, (int)mCurViewPos.x + viewWidth / 2)
				&& isInRange(curTilePosY, (int)mCurViewPos.y - viewHeight / 2, (int)mCurViewPos.y + viewHeight / 2)
				&& isInRange(curTilePosX, 0, mSceneMap.mHeader.mWidth - 1)
				&& isInRange(curTilePosY, 0, mSceneMap.mHeader.mHeight - 1)
				&& !mVisibleTiles.ContainsKey(tileIndex))
				{
					// 变为可见的窗口如果不存在,则需要创建
					if (mTileArray[tileIndex] == null)
					{
						mTileArray[tileIndex] = new TileWindow();
						createTile(mTileArray[tileIndex], mSceneMap.mTileList[tileIndex]);
					}
					mTileArray[tileIndex].setActive(true);
					mVisibleTiles.Add(tileIndex, mSceneMap.mTileList[tileIndex]);
				}
			}
		}
	}
	public void createTile(TileWindow tileWindow, MapTileAdvance tile)
	{
		// 只有真正要使用该地砖时才解析
		if(!tile.mParsed)
		{
			tile.parseTile();
		}
		int x = tileIndexToTileX(tile.mIndex);
		int y = tileIndexToTileY(tile.mIndex);
		string tileSuffix = x + "_" + y;
		// 偶数行偶数列才渲染大地砖
		if (tileWindow.mBackTile == null && x % 2 == 0 && y % 2 == 0)
		{
			// 大地砖图集
			UIAtlas bngAtlas = getBackTileAtlas(tile);
			// 大地砖图片窗口
			string bngSpriteName = "" + (tile.mBngImgIdx - 1);
			if (bngAtlas != null && bngAtlas.GetSprite(bngSpriteName) != null)
			{
				if (!mBackAtlasList.ContainsKey(tile.mBngAtlasIndex))
				{
					mBackAtlasList.Add(tile.mBngAtlasIndex, new List<int>());
				}
				// 一个panel中只允许有一个Drawcall
				// 一个panel满时,就应该放入新的panel中
				// 查找该图集存放在哪些panel中
				int panelIndex = -1;
				List<int> panelIndexList = mBackAtlasList[tile.mBngAtlasIndex];
				int panelCount = panelIndexList.Count;
				for (int j = 0; j < panelCount; ++j)
				{
					// 找到一个包含该图集,且数量未达上限的panel
					if (mBackPanelList[panelIndexList[j]].mObjectCount < MAX_WINDOW_PER_PANEL)
					{
						panelIndex = panelIndexList[j];
						break;
					}
				}
				// 没有找到合适的panel,则创建新的panel
				if (panelIndex == -1)
				{
					panelIndex = GameUtility.makeID();
					PanelInfo panelInfo = new PanelInfo();
					panelInfo.mPanel = createObject<txNGUIPanel>(mBackRoot, "BackPanel" + panelIndex);
					panelInfo.mObjectCount = 0;
					panelInfo.mAtlasIndex = tile.mBngAtlasIndex;
					mBackPanelList.Add(panelIndex, panelInfo);
					mBackAtlasList[tile.mBngAtlasIndex].Add(panelIndex);
				}
				// panel中的窗口计数+1
				mBackPanelList[panelIndex].mObjectCount += 1;
				tileWindow.mBackTile = mWindowPool.createWindow<txNGUISprite>("back_" + tileSuffix, mBackPanelList[panelIndex].mPanel);
				tileWindow.mBackTile.setAtlas(bngAtlas);
				tileWindow.mBackTile.setSpriteName(bngSpriteName, true);
				Vector2 posOffset = tileWindow.mBackTile.getSpriteSize() / 2.0f;
				posOffset += new Vector2(48 * x - mHalfMap.x, mHalfMap.y - 32 * y - 32);
				tileWindow.mBackTile.setLocalPosition(posOffset);
				tileWindow.mBackTile.setDepth(1);
				if (tile.mHasBng)
				{
					tileWindow.mBackTile.getSprite().color = new Color(0.5f, 0.0f, 0.0f);
				}
				else
				{
					tileWindow.mBackTile.getSprite().color = Color.white;
				}
			}
		}
		if(tileWindow.mMidTile == null)
		{
			// 小地砖图片窗口
			string midSpriteName = "" + (tile.mMidImgIdx - 1);
			if (midAtlas.GetSprite(midSpriteName) != null)
			{
				tileWindow.mMidTile = mWindowPool.createWindow<txNGUISprite>("mid_" + tileSuffix, mMiddleRoot);
				tileWindow.mMidTile.setAtlas(midAtlas);
				tileWindow.mMidTile.setSpriteName(midSpriteName, true);
				Vector2 posOffset = tileWindow.mMidTile.getSpriteSize() / 2.0f;
				posOffset += new Vector2(48 * x - mHalfMap.x, mHalfMap.y - 32 * y);
				tileWindow.mMidTile.setLocalPosition(posOffset);
			}
		}
		if (tileWindow.mObjectTile == null)
		{
			if (!tile.mHasAni)
			{
				// 加载对象图集
				UIAtlas atlas = getObjTileAtlas(tile);
				// 创建对象图片窗口
				string spriteName = "" + (tile.mObjImgIdx - 1);
				if (atlas != null && atlas.GetSprite(spriteName) != null)
				{
					tileWindow.mObjectTile = mWindowPool.createWindow<txNGUISprite>("obj_" + tileSuffix, mObjRoot);
					txNGUISprite objTile = tileWindow.mObjectTile;
					objTile.setAtlas(atlas);
					objTile.setSpriteName(spriteName, true);
					Vector2 posOffset = objTile.getSpriteSize() / 2.0f;
					posOffset += new Vector2(48 * x - mHalfMap.x, mHalfMap.y - 32 * y);
					objTile.setLocalPosition(posOffset);
					objTile.setDepth(y);
					if(!tile.mCanWalk)
					{
						BoxCollider collider = objTile.getBoxCollider(true);
						collider.size = objTile.getSpriteSize();
					}
					objTile.setHandleInput(!tile.mCanWalk);
					if(tile.mHasObj)
					{
						objTile.getSprite().color = new Color(0.5f, 0.0f, 0.0f);
					}
					else
					{
						objTile.getSprite().color = Color.white;
					}
				}
			}
		}
		if(tileWindow.mEffect == null)
		{
			if (tile.mHasAni)
			{
				// 加载对象图集
				UIAtlas atlas = getEffectAtlas(tile);
				// 创建对象图片窗口
				if (atlas != null)
				{
					tileWindow.mEffect = mWindowPool.createWindow<txNGUISpriteAnim>("effect_" + tileSuffix, mObjRoot);
					tileWindow.mEffect.setAtlas(atlas);
					tileWindow.mEffect.setTextureSet(intToString(tile.mObjImgIdx - 1));
					tileWindow.mEffect.play();
					tileWindow.mEffect.setLoop(LOOP_MODE.LM_LOOP);
					tileWindow.mEffect.setSpeed(0.5f);
					Vector2 posOffset = new Vector2(0.0f, tileWindow.mEffect.getSpriteSize().y * 1.5f);
					posOffset += new Vector2(48 * x - mHalfMap.x, mHalfMap.y - 32 * y);
					tileWindow.mEffect.setLocalPosition(posOffset);
					tileWindow.mEffect.setDepth(y);
				}
			}
		}
	}
	public void clearSceneInfo()
	{
		if (mTileArray != null)
		{
			int count = mTileArray.Length;
			for (int i = 0; i < count; ++i)
			{
				if (mTileArray[i] != null)
				{
					mTileArray[i].destroy(mWindowPool);
				}
			}
		}
		foreach (var item in mBackPanelList)
		{
			destroyObject(item.Value.mPanel, true);
		}
		mBackPanelList.Clear();
		mBackAtlasList.Clear();
		mVisibleTiles.Clear();
	}
	//------------------------------------------------------------------------------------------------------------------------------------
	protected void onCreateNewSceneClick(GameObject go)
	{
		clearSceneInfo();
		string mapFileName = CommonDefine.F_STREAMING_ASSETS_PATH + "Map/" + mSceneNameEdit.getText() + ".amap";
		mSceneMap = new SceneMapAdvance();
		mSceneMap.readFile(mapFileName);
		mWidthLabel.setLabel("宽:" + mSceneMap.mHeader.mWidth);
		mHeightLabel.setLabel("高:" + mSceneMap.mHeader.mHeight);
		mTileArray = new TileWindow[mSceneMap.mHeader.mWidth * mSceneMap.mHeader.mHeight];
		mHalfMap = new Vector2(mSceneMap.mHeader.mWidth * 48 / 2.0f, mSceneMap.mHeader.mHeight * 32 / 2.0f);
		mCurViewPos = new Vector2(mSceneMap.mHeader.mWidth / 2.0f, mSceneMap.mHeader.mHeight / 2.0f);
		refreshMap();
		setViewPos(mCurViewPos);
	}
	protected UIAtlas getBackTileAtlas(MapTileAdvance tile)
	{
		UIAtlas bngAtlas = null;
		if (tile.mBngAtlasIndex != 0xFF)
		{
			if (!mBngAtlasIndexMap.ContainsKey(tile.mBngAtlasIndex))
			{
				bngAtlas = mResourceManager.loadResource<UIAtlas>(mBackTilePath + tile.mBngAtlasIndex, true);
				mBngAtlasIndexMap.Add(tile.mBngAtlasIndex, bngAtlas);
			}
			else
			{
				bngAtlas = mBngAtlasIndexMap[tile.mBngAtlasIndex];
			}
		}
		return bngAtlas;
	}
	protected UIAtlas getObjTileAtlas(MapTileAdvance tile)
	{
		UIAtlas atlas = null;
		if (!mObjAtlasIndexMap.ContainsKey(tile.mObjFileIdx + 1))
		{
			mObjAtlasIndexMap.Add(tile.mObjFileIdx + 1, new Dictionary<int, UIAtlas>());
		}
		if (tile.mObjAtlasIndex != 0xFF)
		{
			if (!mObjAtlasIndexMap[tile.mObjFileIdx + 1].ContainsKey(tile.mObjAtlasIndex))
			{
				atlas = mResourceManager.loadResource<UIAtlas>(mObjectImagePreString + (tile.mObjFileIdx + 1) + "/" + tile.mObjAtlasIndex, true);
				mObjAtlasIndexMap[tile.mObjFileIdx + 1].Add(tile.mObjAtlasIndex, atlas);
			}
			else
			{
				atlas = mObjAtlasIndexMap[tile.mObjFileIdx + 1][tile.mObjAtlasIndex];
			}
		}
		return atlas;
	}
	protected UIAtlas getEffectAtlas(MapTileAdvance tile)
	{
		UIAtlas atlas = null;
		int fileIndex = tile.mObjFileIdx + 1;
		if (!mEffectAtlasIndexMap.ContainsKey(fileIndex))
		{
			mEffectAtlasIndexMap.Add(fileIndex, new Dictionary<int, UIAtlas>());
		}
		if (tile.mHasAni)
		{
			int objImageIndex = tile.mObjImgIdx - 1;
			if (!mEffectAtlasIndexMap[fileIndex].ContainsKey(objImageIndex))
			{
				atlas = mResourceManager.loadResource<UIAtlas>(mEffectPath + fileIndex + "/" + objImageIndex, true);
				mEffectAtlasIndexMap[fileIndex].Add(objImageIndex, atlas);
			}
			else
			{
				atlas = mEffectAtlasIndexMap[fileIndex][objImageIndex];
			}
		}
		return atlas;
	}
	protected int tileIndexToTileX(int index)
	{
		return index / mSceneMap.mHeader.mHeight;
	}
	protected int tileIndexToTileY(int index)
	{
		return index % mSceneMap.mHeader.mHeight;
	}
	protected int tilePosToTileIndex(int x, int y)
	{
		return x * mSceneMap.mHeader.mHeight + y;
	}
}
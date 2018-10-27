using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using UnityEngine;

public class TileTextureWindow
{
	public txNGUITexture mBackTile;
	public txNGUITexture mMidTile;
	public txNGUITexture mObjectTile;
	public void setActive(bool active)
	{
		if (mBackTile != null && mBackTile.isActive() != active)
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
	}
	public void destroy(WindowPool pool)
	{
		pool.destroyWindow(mBackTile);
		pool.destroyWindow(mMidTile);
		pool.destroyWindow(mObjectTile);
	}
}

public class ScriptSceneEditor : LayoutScript
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
	protected Dictionary<int, txNGUIPanel> mBackPanelList;
	protected Vector2 mCurViewPos;                      // 使用地图的坐标系,X轴向右,Y轴向下
	protected SceneMap mSceneMap;
	protected WindowPool mWindowPool;
	protected TileTextureWindow[] mTileArray;          // 当前场景所有的窗口
	protected Dictionary<int, MapTile> mVisibleTiles; // 存放当前可见的窗口
	protected string mBackTilePath;
	protected string mMidTilePath;
	protected string mObjectImagePreString;
	protected Vector2 mHalfMap;
	protected const int MAX_WINDOW_PER_PANEL = 5000;
	protected int mMoveSpeed = 10;
	public ScriptSceneEditor(string name, GameLayout layout)
		:
		base(name, layout)
	{
		mBackPanelList = new Dictionary<int, txNGUIPanel>();
		mWindowPool = new WindowPool(this);
		mVisibleTiles = new Dictionary<int, MapTile>();
		mBackTilePath = CommonDefine.R_GAME_TEXTURE_PATH + "MapTexture/Tiles/";
		mMidTilePath = CommonDefine.R_GAME_TEXTURE_PATH + "MapTexture/SmTiles/";
		mObjectImagePreString = CommonDefine.R_GAME_TEXTURE_PATH + "MapTexture/Objects";
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
		if (mSceneMap != null)
		{
			bool changed = false;
			Vector3 moveDelta = Vector3.zero;
			// 方向键用来移动地图
			if (getKeyCurrentUp(KeyCode.UpArrow))
			{
				mCurViewPos.y += 1 * mMoveSpeed;
				changed = true;
				moveDelta.y = 32 * mMoveSpeed;
			}
			if (getKeyCurrentUp(KeyCode.DownArrow))
			{
				mCurViewPos.y -= 1 * mMoveSpeed;
				changed = true;
				moveDelta.y = -32 * mMoveSpeed;
			}
			if (getKeyCurrentUp(KeyCode.LeftArrow))
			{
				mCurViewPos.x += 1 * mMoveSpeed;
				changed = true;
				moveDelta.x = -48 * mMoveSpeed;
			}
			if (getKeyCurrentUp(KeyCode.RightArrow))
			{
				mCurViewPos.x -= 1 * mMoveSpeed;
				changed = true;
				moveDelta.x = 48 * mMoveSpeed;
			}
			if (changed)
			{
				refreshMap();
				LT.MOVE_WINDOW(mSceneRoot, mSceneRoot.getPosition() + moveDelta);
			}
		}
	}
	public void refreshMap()
	{
		logInfo("view center : " + mCurViewPos, LOG_LEVEL.LL_FORCE);
		// 先判断是否有窗口已经从可见变为不可见
		int viewWidth = stringToInt(mViewWidthEdit.getText());
		int viewHeight = stringToInt(mViewHeightEdit.getText());
		List<int> removedIndex = new List<int>();
		foreach (var item in mVisibleTiles)
		{
			int x = tileIndexToTileX(item.Key);
			int y = tileIndexToTileY(item.Key);
			// 判断是否已经超出视野范围
			if (!isInRange(x, mCurViewPos.x - viewWidth / 2, mCurViewPos.x + viewWidth / 2)
				|| !isInRange(y, mCurViewPos.y - viewHeight / 2, mCurViewPos.y + viewHeight / 2))
			{
				mTileArray[item.Key].setActive(false);
				removedIndex.Add(item.Key);
			}
		}
		int removedCount = removedIndex.Count;
		for (int i = 0; i < removedCount; ++i)
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
						mTileArray[tileIndex] = new TileTextureWindow();
						createTile(mTileArray[tileIndex], mSceneMap.mTileList[tileIndex]);
					}
					mTileArray[tileIndex].setActive(true);
					mVisibleTiles.Add(tileIndex, mSceneMap.mTileList[tileIndex]);
				}
			}
		}
	}
	public void createTile(TileTextureWindow tileWindow, MapTile tile)
	{
		// 只有真正要使用该地砖时才解析
		if (!tile.mParsed)
		{
			tile.parseTile();
		}
		int x = tileIndexToTileX(tile.mIndex);
		int y = tileIndexToTileY(tile.mIndex);
		string tileSuffix = x + "_" + y;
		// 偶数行偶数列才渲染大地砖
		if (tileWindow.mBackTile == null && x % 2 == 0 && y % 2 == 0)
		{
			// 大地砖图片窗口
			string backTexName = mBackTilePath + (tile.mBngImgIdx - 1);
			Texture backTex = mResourceManager.loadResource<Texture>(backTexName, false);
			if (backTex != null)
			{
				// 一个panel满时,就应该放入新的panel中
				int panelIndex = tile.mIndex / MAX_WINDOW_PER_PANEL;
				if (!mBackPanelList.ContainsKey(panelIndex))
				{
					mBackPanelList.Add(panelIndex, createObject<txNGUIPanel>(mBackRoot, "BackPanel" + panelIndex));
				}
				tileWindow.mBackTile = mWindowPool.createWindow<txNGUITexture>("back_" + tileSuffix, mBackPanelList[panelIndex]);
				tileWindow.mBackTile.setTexture(backTex, true);
				tileWindow.mBackTile.setMaterial("NGUIDefault", false);
				Vector2 posOffset = tileWindow.mBackTile.getTextureSize() / 2.0f;
				posOffset += new Vector2(48 * x - mHalfMap.x, mHalfMap.y - 32 * y - 32);
				tileWindow.mBackTile.setLocalPosition(posOffset);
				tileWindow.mBackTile.setDepth(1);
				if (tile.mHasBng)
				{
					tileWindow.mBackTile.setColor(new Color(0.5f, 0.0f, 0.0f));
				}
				else
				{
					tileWindow.mBackTile.setColor(Color.white);
				}
				tileWindow.mBackTile.getUITexture().mUserData = charArrayToHexString(tile.mTileBuffer, tile.mTileBuffer.Length);
			}
		}
		if (tileWindow.mMidTile == null)
		{
			// 小地砖图片窗口
			string midTexName = mMidTilePath + (tile.mMidImgIdx - 1);
			Texture midTex = mResourceManager.loadResource<Texture>(midTexName, false);
			if (midTex != null)
			{
				tileWindow.mMidTile = mWindowPool.createWindow<txNGUITexture>("mid_" + tileSuffix, mMiddleRoot);
				tileWindow.mMidTile.setTexture(midTex, true);
				tileWindow.mMidTile.setMaterial("NGUIDefault", false);
				Vector2 posOffset = tileWindow.mMidTile.getTextureSize() / 2.0f;
				posOffset += new Vector2(48 * x - mHalfMap.x, mHalfMap.y - 32 * y);
				tileWindow.mMidTile.setLocalPosition(posOffset);
				tileWindow.mMidTile.setColor(Color.white);
			}
		}
		if (tileWindow.mObjectTile == null)
		{
			// 普通物体
			if(!tile.mHasAni)
			{
				string objTexName = mObjectImagePreString + (tile.mObjFileIdx + 1) + "/" + (tile.mObjImgIdx - 1);
				Texture objTex = mResourceManager.loadResource<Texture>(objTexName, false);
				if (objTex != null)
				{
					tileWindow.mObjectTile = mWindowPool.createWindow<txNGUITexture>("obj_" + tileSuffix, mObjRoot);
					tileWindow.mObjectTile.setTexture(objTex, true);
					tileWindow.mObjectTile.setMaterial("NGUIDefault", false);
					Vector2 posOffset = tileWindow.mObjectTile.getTextureSize() / 2.0f;
					posOffset += new Vector2(48 * x - mHalfMap.x, mHalfMap.y - 32 * y);
					tileWindow.mObjectTile.setLocalPosition(posOffset);
					tileWindow.mObjectTile.setDepth(3);
					if (tile.mHasObj)
					{
						tileWindow.mObjectTile.setColor(new Color(0.5f, 0.0f, 0.0f));
					}
					else
					{
						tileWindow.mObjectTile.setColor(Color.white);
					}
					tileWindow.mObjectTile.getUITexture().mUserData = charArrayToHexString(tile.mTileBuffer, tile.mTileBuffer.Length);
				}
			}
			// 带序列帧的物体
			else
			{
				string objTexName = mObjectImagePreString + (tile.mObjFileIdx + 1) + "/" + (tile.mObjImgIdx - 1);
				Texture objTex = mResourceManager.loadResource<Texture>(objTexName, false);
				if (objTex != null)
				{
					tileWindow.mObjectTile = mWindowPool.createWindow<txNGUITexture>("obj_" + tileSuffix, mObjRoot);
					tileWindow.mObjectTile.setTexture(objTex, true);
					tileWindow.mObjectTile.setMaterial("Multiple", false);
					Vector2 posOffset = new Vector2(0.0f, tileWindow.mObjectTile.getTextureSize().y * 1.5f);
					posOffset += new Vector2(48 * x - mHalfMap.x, mHalfMap.y - 32 * y);
					tileWindow.mObjectTile.setLocalPosition(posOffset);
					tileWindow.mObjectTile.setDepth(3);
				}
			}
		}
	}
	public void clearSceneInfo()
	{
		// 销毁所有对象
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
		// 销毁为了分批次而创建的panel
		foreach (var item in mBackPanelList)
		{
			destroyObject(item.Value, true);
		}
		mBackPanelList.Clear();
		mVisibleTiles.Clear();
	}
	protected void onCreateNewSceneClick(GameObject go)
	{
		clearSceneInfo();
		string mapFileName = CommonDefine.F_STREAMING_ASSETS_PATH + "Map/" + mSceneNameEdit.getText()+ ".map";
		mSceneMap = new SceneMap();
		mSceneMap.readFile(mapFileName);
		mWidthLabel.setLabel("宽:" + mSceneMap.mHeader.mWidth);
		mHeightLabel.setLabel("高:" + mSceneMap.mHeader.mHeight);
		mTileArray = new TileTextureWindow[mSceneMap.mHeader.mWidth * mSceneMap.mHeader.mHeight];
		mHalfMap = new Vector2(mSceneMap.mHeader.mWidth * 48 / 2.0f, mSceneMap.mHeader.mHeight * 32 / 2.0f);
		mCurViewPos = new Vector2(mSceneMap.mHeader.mWidth / 2.0f, mSceneMap.mHeader.mHeight / 2.0f);
		refreshMap();
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